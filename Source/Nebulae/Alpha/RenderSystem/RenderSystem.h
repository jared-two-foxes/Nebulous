#ifndef NEBULAE_ALPHA_RENDERSYSTEM_H__
#define NEBULAE_ALPHA_RENDERSYSTEM_H__

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/RenderSystem/OperationType.h>
#include <Nebulae/Alpha/Resource/ResourceFactory.h>
#include <Nebulae/Alpha/RenderTexture/RenderTexture.h>
#include <Nebulae/Alpha/Sampler/Sampler.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/Texture.h>


namespace Nebulae {


class FileSystem;
class Window;


/** RenderSystem.
  */
class RenderSystem
{
public:
  typedef std::shared_ptr<FileSystem > FileSystemPtr;
  typedef std::shared_ptr<Window >     WindowPtr;

private:
  typedef ResourceFactory<HardwareBuffer > BufferFactory;
  typedef ResourceFactory<InputLayout >    LayoutFactory;
  typedef ResourceFactory<RenderTexture >  RenderTextureFactory;
  typedef ResourceFactory<Sampler >        SamplerFactory;
  typedef ResourceFactory<HardwareShader > ShaderFactory;
  typedef ResourceFactory<Texture >        TextureFactory;

protected:
  RenderSystemType       m_type;
  FileSystemPtr          m_fileSystem; 
  WindowPtr              m_window;
  BufferFactory*         m_bufferFactory;
  LayoutFactory*         m_layoutFactory;
  RenderTextureFactory*  m_renderTextureFactory;
  SamplerFactory*        m_samplerFactory;
  ShaderFactory*         m_shaderFactory;
  TextureFactory*        m_textureFactory;

  public:
    RenderSystem( FileSystemPtr fileSystem, WindowPtr window );
    virtual ~RenderSystem();

    virtual const std::string& GetName() const = 0;
    const WindowPtr GetWindow() const;

    //functions.
    virtual bool Initiate();
    bool         IsInitialized() const;
    virtual void Clear() = 0;
    virtual void SetClearColour( float r, float g, float b, float a );
    virtual void SwapBuffers();		
    virtual void SetBlendingState( bool enable );
    virtual void SetDepthTest( bool enable );
    virtual void BeginScissorClipping( Point ul, Point lr );
    virtual void EndScissorClipping();
    virtual void BeginStencilClipping( Point inner_ul, Point inner_lr, Point outer_ul, Point outer_lr );
    virtual void EndStencilClipping();


    //factory functions.
    HardwareBuffer* CreateBuffer( const std::string& identifier, const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem, bool shouldLoad = true );
    InputLayout*    CreateInputLayout( const std::string& identifier, const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader, bool shouldLoad = true );
    RenderTexture*  CreateRenderTexture( const std::string& identifier, int32 width = -1, int32 height = -1, bool shouldLoad = true );
    HardwareShader* CreateShader( const std::string& identifier, HardwareShaderType type, bool shouldLoad = true );
    Sampler*        CreateSampler( const std::string& identifier, bool shouldLoad = true );
    Texture*        CreateTexture( const std::string& identifier, bool shouldLoad = true );

    //accessors functions.
    HardwareBuffer* FindBufferByName( const std::string& identifier ) const;
    InputLayout*    FindInputLayoutByName( const std::string& identifier ) const;
    InputLayout*    FindInputLayoutByUsage( VertexDeceleration* decleration, HardwareShader* vertexShader ) const;
    RenderTexture*  FindRenderTextureByName( const std::string& identifier ) const;
    HardwareShader* FindShaderByName( const std::string& identifier ) const;
    Sampler*        FindSamplerByName( const std::string& identifier ) const;
    Texture*        FindTextureByName( const std::string& identifier ) const;
    Texture*        FindTextureByIdentifier( int32 identifier ) const;

    //pipeline functions.
    virtual void SetVertexBuffers( int iSlot, HardwareBuffer* pBuffers, std::size_t iStride, std::size_t iOffset );
    virtual void SetInputLayout( InputLayout* pInputLayout );
    virtual void SetIndexBuffer( HardwareBuffer* pBuffer, size_t iOffset );
    virtual void SetRenderTexture( RenderTexture* pRenderTexture );
    virtual void SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader );
    virtual void SetOperationType( OperationType eType );
    virtual void Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation );
    virtual void DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation, std::size_t iBaseVertexLocation );

    //shader binding functions.
    virtual UniformDefinition GetUniformByName( const char* name ) const;

    virtual void SetBufferBinding( uint32 iTarget, uint32 iCount, HardwareBuffer* pBuffer );
    virtual void SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* pImpl );
    virtual void SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* pImpl );
    virtual void SetUniformBinding( UniformDefinition& definition, void* value );


  protected:
    virtual HardwareBufferImpl* CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem );
    virtual HardwareShaderImpl* CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType );
    virtual InputLayoutImpl*    CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader );
    virtual RenderTextureImpl*  CreateRenderTextureImpl( int32 width, int height );
    virtual Sampler::Impl*      CreateSamplerImpl();
    virtual TextureImpl*        CreateTextureImpl( const std::string& strFileName );

}; //RenderSystem

} // Nebulae

#endif // NEBULAE_ALPHA_RENDERSYSTEM_H__