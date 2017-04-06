
#include "RenderSystem.h"

#include <Nebulae/Common/Window/Window.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>

using namespace Nebulae;


RenderSystem::RenderSystem( FileSystemPtr fileSystem, WindowPtr window )
  : m_fileSystem(fileSystem),
    m_window(window), 
    m_bufferFactory(nullptr),
    m_layoutFactory(nullptr),
    m_renderTextureFactory(nullptr),
    m_samplerFactory(nullptr),
    m_shaderFactory(nullptr),
    m_textureFactory(nullptr)
{
}

RenderSystem::~RenderSystem()
{
  if( m_bufferFactory ) {
    delete m_bufferFactory;
  }
  if( m_layoutFactory ) {
    delete m_layoutFactory;
  }
  if( m_renderTextureFactory ) {
    delete m_renderTextureFactory;
  }
  if( m_samplerFactory ) {
    delete m_samplerFactory;
  }
  if( m_shaderFactory ) {
    delete m_shaderFactory;
  }
  if( m_textureFactory ) {
    delete m_textureFactory;
  }
}

const RenderSystem::WindowPtr
RenderSystem::GetWindow() const
{ return m_window; }

bool 
RenderSystem::Initiate()
{
  m_bufferFactory        = new BufferFactory( *this );
  m_layoutFactory        = new LayoutFactory( *this );
  m_renderTextureFactory = new RenderTextureFactory( *this );
  m_samplerFactory       = new SamplerFactory( *this );
  m_shaderFactory        = new ShaderFactory( *this );
  m_textureFactory       = new TextureFactory( *this );

  return true;
}

bool         
RenderSystem::IsInitialized() const
{ 
  return
    m_bufferFactory != NULL &&
    m_layoutFactory != NULL &&
    m_renderTextureFactory != NULL &&
    m_samplerFactory != NULL &&
    m_shaderFactory != NULL &&
    m_textureFactory != NULL;
}

void 
RenderSystem::SetBlendingState( bool enable )
{}

void
RenderSystem::SetDepthTest( bool enable )
{}

void 
RenderSystem::BeginScissorClipping( Point ul, Point lr )
{}

void 
RenderSystem::EndScissorClipping()
{}

void 
RenderSystem::BeginStencilClipping( Point inner_ul, Point inner_lr, Point outer_ul, Point outer_lr )
{}

void 
RenderSystem::EndStencilClipping()
{}

HardwareBuffer* 
RenderSystem::CreateBuffer( const std::string& identifier, const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem, bool shouldLoad )
{ 
  HardwareBuffer* buffer = m_bufferFactory->Create( identifier.c_str() );
  if( buffer && !buffer->IsLoaded() ) {
    buffer->SetImpl( CreateBufferImpl(usage,sizeInBytes,bindFlags,sysMem) );
    if( shouldLoad ) {
      buffer->Load( NULL ); //< buffers do not come from file therefore pass a NULL filestream pointer.
    }
  }

  return buffer;
}

InputLayout*    
RenderSystem::CreateInputLayout( const std::string& identifier, const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader, bool shouldLoad )
{ 
  InputLayout* inputLayout = m_layoutFactory->Create( identifier.c_str() );
  if( inputLayout && !inputLayout->IsLoaded() ) {
    inputLayout->SetImpl( CreateInputLayoutImpl(pVertexDecl, pVertexShader) );
    if( shouldLoad ) {
      inputLayout->Load( NULL ); //< inputLayout's do not come from file therefore pass a NULL filestream pointer.
    }
  }

  return inputLayout;
}

Sampler*        
RenderSystem::CreateSampler( const std::string& identifier, bool shouldLoad )
{ 
  Sampler* sampler = m_samplerFactory->Create( identifier.c_str() );
  if( sampler && !sampler->IsLoaded() ) {
    sampler->SetImpl( CreateSamplerImpl() );
    if( shouldLoad ) {
      sampler->Load( NULL ); //< sampler's do not come from file therefore pass a NULL filestream pointer.
    }
  }

  return sampler;
}

RenderTexture* 
RenderSystem::CreateRenderTexture( const std::string& identifier, int32 width, int32 height, bool shouldLoad )
{ 
  if( width == -1 )  width  = m_window->GetWidth();
  if( height == -1 ) height = m_window->GetHeight();

  RenderTexture* renderTexture = m_renderTextureFactory->Create( identifier.c_str() );
  if( renderTexture && !renderTexture->IsLoaded() ) {
    renderTexture->SetImpl( CreateRenderTextureImpl(width, height) );
    if( shouldLoad ) {
      renderTexture->Load( NULL ); //< RenderTextures's do not come from file therefore pass a NULL filestream pointer.
    }
  }
	 
  return renderTexture;
}

HardwareShader* 
RenderSystem::CreateShader( const std::string& identifier, HardwareShaderType eType, bool shouldLoad )
{ 
  HardwareShader* shader = m_shaderFactory->Create( identifier.c_str() );
  if( shader && !shader->IsLoaded() ) {
    shader->SetImpl( CreateShaderImpl( identifier, eType ) );
    if( shouldLoad ) {
      shader->Load( m_fileSystem->Open(NE_DEFAULT_ROOTDEVICE, identifier) );
    }
  }

  return shader;
}

Texture*        
RenderSystem::CreateTexture( const std::string& identifier, bool shouldLoad )
{ 
  Texture* texture = m_textureFactory->Create( identifier.c_str() );
  if( texture && !texture->IsLoaded() ) {
    texture->SetImpl( CreateTextureImpl(identifier) );
    if( shouldLoad ) {
      texture->Load( m_fileSystem->Open(NE_DEFAULT_ROOTDEVICE,identifier) );
    }
  }

  return texture;
}

HardwareBuffer* 
RenderSystem::FindBufferByName( const std::string& identifier ) const
{ return m_bufferFactory->FindByName( identifier.c_str() ); }

InputLayout*    
RenderSystem::FindInputLayoutByName( const std::string& identifier ) const
{ return m_layoutFactory->FindByName( identifier.c_str() ); }

InputLayout*    
RenderSystem::FindInputLayoutByUsage( VertexDeceleration* decleration, HardwareShader* vertexShader ) const
{
  const std::vector<InputLayout* >& layouts = m_layoutFactory->GetResources();
  for( std::vector<InputLayout* >::const_iterator itr = layouts.begin(); itr != layouts.end(); ++itr )
  {
    InputLayoutImpl* impl = (*itr)->GetImpl();

    //@todo.  Add a comparison operation for VertexDecleration objects.
    if( *impl->GetVertexDecleration() == *decleration && impl->GetVertexShader() == vertexShader )
    {
      return (*itr);
    }
  }

  return nullptr;
}

RenderTexture* 
RenderSystem::FindRenderTextureByName( const std::string& identifier ) const
{ return m_renderTextureFactory->FindByName( identifier.c_str() ); }

HardwareShader* 
RenderSystem::FindShaderByName( const std::string& identifier ) const
{ return m_shaderFactory->FindByName( identifier.c_str() ); }

Sampler*        
RenderSystem::FindSamplerByName( const std::string& identifier ) const
{ return m_samplerFactory->FindByName( identifier.c_str() ); }

Texture*        
RenderSystem::FindTextureByName( const std::string& identifier ) const
{ return m_textureFactory->FindByName( identifier.c_str() ); }

Texture*        
RenderSystem::FindTextureByIdentifier( int32 identifier ) const
{ return m_textureFactory->FindByIdentifier( identifier ); }

void 
RenderSystem::SetClearColour( float r, float g, float b, float a ) 
{}

void 
RenderSystem::SwapBuffers()
{}

void 
RenderSystem::SetVertexBuffers( int iSlot, /*int iCount,*/ HardwareBuffer* pBufferImpl, std::size_t iStride, std::size_t iOffset )
{}

void 
RenderSystem::SetInputLayout( InputLayout* pInputLayoutImpl )
{}

void 
RenderSystem::SetIndexBuffer( HardwareBuffer* pBuffer, size_t iOffset )
{}

void
RenderSystem::SetRenderTexture( RenderTexture* pRenderTexture )
{}

void 
RenderSystem::SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader )
{}

void 
RenderSystem::SetOperationType( OperationType eType )
{}

void 
RenderSystem::Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation )
{}

void 
RenderSystem::DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation, std::size_t iBaseVertexLocation )
{}

UniformDefinition 
RenderSystem::GetUniformByName( const char* name ) const
{ return UniformDefinition(); }

void 
RenderSystem::SetBufferBinding( uint32 iTarget, uint32 iIndex, HardwareBuffer* pImpl )
{}

void 
RenderSystem::SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* sampler )
{}

void
RenderSystem::SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* texture )
{}

void RenderSystem::SetUniformBinding( UniformDefinition& definition, void* value )
{}

HardwareBufferImpl* 
RenderSystem::CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem )
{ return nullptr; }

RenderTextureImpl*
RenderSystem::CreateRenderTextureImpl( int32 width, int32 height )
{ return nullptr; }

HardwareShaderImpl*
RenderSystem::CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType )
{ return nullptr; }

InputLayoutImpl* 
RenderSystem::CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader )
{ return nullptr; }

Sampler::Impl* 
RenderSystem::CreateSamplerImpl()
{ return nullptr; }

TextureImpl* 
RenderSystem::CreateTextureImpl( const std::string& strName )
{ return nullptr; }
