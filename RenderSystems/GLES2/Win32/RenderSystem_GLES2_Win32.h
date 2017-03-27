#ifndef NEBULAE_GLES2_RENDERSYSTEM_H__
#define NEBULAE_GLES2_RENDERSYSTEM_H__


#include <GLES2/config.h>

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>


namespace Nebulae {
 
class ProgramObject;

class RenderSystem_GLES2_Win32 : public RenderSystem {

  public:
    typedef std::vector<ProgramObject* > ProgramObjectList;

  private:
		std::string				 m_name;
    EGLDisplay         m_display;
    EGLSurface         m_surface;
    uint32             m_OperationMode;
    ProgramObject*     m_boundProgram;    ///< The current bound link program.
    ProgramObjectList  m_programObjects;
    HardwareShader*    m_vertexShader;
    HardwareShader*    m_fragmentShader;
    InputLayout*       m_inputLayout;     ///< The currently bound input layout.
    RenderTexture*     m_renderTexture;
    
    std::vector<Rect > m_scissor_clipping_rects;

    public:
      RenderSystem_GLES2_Win32( FileSystemPtr fileSystem, WindowPtr window );
      virtual ~RenderSystem_GLES2_Win32();
      
      virtual const std::string& GetName() const override;

      //functions.
      virtual bool Initiate() override;
      virtual void Clear() override;
      virtual void SetClearColour( float r, float g, float b, float a ) override;
      virtual void SwapBuffers() override;
      virtual void SetBlendingState( bool enable ) override;
      virtual void SetDepthTest( bool enable ) override;
      virtual void BeginScissorClipping( Point ul, Point lr ) override;
      virtual void EndScissorClipping() override;
      virtual void BeginStencilClipping( Point inner_ul, Point inner_lr, Point outer_ul, Point outer_lr ) override;
      virtual void EndStencilClipping() override;

      //pipeline functions.
      virtual void SetVertexBuffers( int iSlot, HardwareBuffer* pBuffers, size_t iStride, size_t iOffset ) override;
      virtual void SetInputLayout( InputLayout* pInputLayout ) override;
      virtual void SetIndexBuffer( HardwareBuffer* pBuffer, size_t iOffset ) override;
      virtual void SetRenderTexture( RenderTexture* pRenderTexture ) override;
      virtual void SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader ) override;
      virtual void SetOperationType( OperationType eType ) override;
      virtual void Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation ) override;
      virtual void DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation, std::size_t iBaseVertexLocation ) override;

      //shader binding functions.
      virtual UniformDefinition GetUniformByName( const char* name ) const override;
      virtual void SetBufferBinding( uint32 iTarget, uint32 iCount, HardwareBuffer* pBuffer ) override;
      virtual void SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* sampler ) override;
      virtual void SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* texture ) override;
      virtual void SetUniformBinding( UniformDefinition& definition, void* value ) override;

    private:
      virtual HardwareBufferImpl* CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem ) override;
      virtual InputLayoutImpl*    CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader ) override;
      virtual RenderTextureImpl*  CreateRenderTextureImpl( int32 width, int height ) override;
      virtual Sampler::Impl*      CreateSamplerImpl() override;
      virtual HardwareShaderImpl* CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType ) override;
      virtual TextureImpl*        CreateTextureImpl( const std::string& strFileName ) override;

}; //RenderSystem_GLES2_Win32

} // Nebulae

#endif // NEBULAE_GLES2_RENDERSYSTEM_H__