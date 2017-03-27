#ifndef NEBULAE_OGL_RENDERSYSTEM_H__
#define NEBULAE_OGL_RENDERSYSTEM_H__

//#include <GL/config.h>

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>


namespace Nebulae
{
 
class ProgramObject;

typedef std::vector<ProgramObject* > ProgramObjectList;

/** 
 * RenderSystem_OGL.
 */
class RenderSystem_OGL : public RenderSystem
{
private:
	std::string				 m_name;
  HDC                m_hDC;
	HGLRC              m_hRC;
	uint32             m_OperationMode;

	ProgramObject*     m_boundProgram;    ///< The current bound link program.
  ProgramObjectList  m_programObjects;

  HardwareShader*    m_vertexShader;
  HardwareShader*    m_fragmentShader;

  public:
  	RenderSystem_OGL( FileSystemPtr fileSystem, WindowPtr window );
  	virtual ~RenderSystem_OGL();
  	
    virtual const std::string& GetName() const;
    
    //functions.
    virtual bool Initiate() override;
  	virtual void Clear() override;
    virtual void SetClearColour( float r, float g, float b, float a ) override;
    virtual void SwapBuffers() override;
    virtual void SetBlendingState( bool enable ) override;
    virtual void SetDepthTest( bool enable ) override;

    //pipeline functions
    virtual void SetVertexBuffers( int iSlot, HardwareBuffer* pBuffers, size_t iStride, size_t iOffset ) override;
    virtual void SetInputLayout( InputLayout* pInputLayout ) override;
    virtual void SetIndexBuffer( HardwareBuffer* pBuffer, size_t iOffset ) override;
    virtual void SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader ) override;
  	virtual void SetOperationType( OperationType eType ) override;
  	virtual void Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation ) override;
  	virtual void DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation, std::size_t iBaseVertexLocation ) override;

    //shader binding functions 
    virtual UniformDefinition GetUniformByName( const char* name ) const override;
    virtual void SetBufferBinding( uint32 iTarget, uint32 iCount, HardwareBuffer* pBuffer ) override;
  	virtual void SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* pImpl ) override;
  	virtual void SetTextureBinding( uint32 iTarget, uint32 iIndex, Texture* pImpl ) override;
  	virtual void SetUniformBinding( UniformDefinition& definition, void* value ) override;

  private:
    virtual HardwareBufferImpl* CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem ) override;
    virtual HardwareShaderImpl* CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType ) override;
		virtual InputLayoutImpl*    CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader ) override;
    virtual Sampler::Impl*      CreateSamplerImpl() override;
    virtual TextureImpl*        CreateTextureImpl( const std::string& strFileName ) override;

}; //RenderSystem

} // Nebulae

#endif // __NEBULAE_RENDERSYSTEM_H__