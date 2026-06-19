#ifndef NEBULAE_RENDERSYSTEMS_GL_RENDERSYSTEM_OGL_H_
#define NEBULAE_RENDERSYSTEMS_GL_RENDERSYSTEM_OGL_H_

// #include <GL/config.h>

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/RenderStream/RenderStream.h>


namespace Nebulae
{

class ProgramObject;

typedef std::vector<ProgramObject*> ProgramObjectList;

/**
 * RenderSystem_OGL.
 */
class RenderSystem_OGL : public RenderSystem
{
private:
  std::string m_name;
  HDC m_hDC;
  HGLRC m_hRC;
  uint32 m_OperationMode;

  ProgramObject* m_boundProgram; ///< The current bound link program.
  ProgramObjectList m_programObjects;

  HardwareShader* m_vertexShader;
  HardwareShader* m_fragmentShader;

public:
  RenderSystem_OGL( FileSystemPtr fileSystem, WindowPtr window );
  virtual ~RenderSystem_OGL();

  virtual const std::string& GetName() const;

  // functions.
  virtual bool Initiate() override;
  virtual void Clear() override;
  virtual void SetClearColour( float r, float g, float b, float a ) override;
  virtual void SwapBuffers() override;
  virtual void SetBlendingState( bool enable ) override;
  virtual void SetDepthTest( bool enable ) override;

  // pipeline functions
  virtual void SetVertexBuffers( int iSlot, HardwareBuffer* pBuffers, size_t iStride, size_t iOffset ) override;
  virtual void SetInputLayout( InputLayout* pInputLayout ) override;
  virtual void SetIndexBuffer( HardwareBuffer* pBuffer, size_t iOffset ) override;
  virtual void SetShaders( HardwareShader* vertexShader, HardwareShader* fragmentShader ) override;
  virtual void SetOperationType( OperationType eType ) override;
  virtual void Draw( std::size_t iVertexCount, std::size_t iStartVertexLocation ) override;
  virtual void DrawIndexed( std::size_t iIndexCount, std::size_t iStartIndexLocation,
                            std::size_t iBaseVertexLocation ) override;

  // shader binding functions
  virtual void SetBufferBinding( uint32 iTarget, uint32 iCount, HardwareBuffer* pBuffer ) override;
  virtual void SetSamplerBinding( uint32 iTarget, uint32 iIndex, Sampler* pImpl ) override;

  // stream API functions
  virtual bool ReflectProgram( ProgramObject& prog, UniformDefinitionMap& out ) override;
  virtual void ExecuteStream( const RenderStream& stream ) override;

private:
  virtual HardwareBufferImpl* CreateBufferImpl( const Flags<HardwareBufferUsage>& usage, size_t sizeInBytes,
                                                HardwareBufferBinding bindFlags, void* sysMem ) override;
  virtual HardwareShaderImpl* CreateShaderImpl( const std::string& strFileName, HardwareShaderType eType ) override;
  virtual InputLayoutImpl* CreateInputLayoutImpl( const VertexDeceleration* pVertexDecl,
                                                  const HardwareShader* pVertexShader ) override;
  virtual Sampler::Impl* CreateSamplerImpl() override;
  virtual TextureImpl* CreateTextureImpl( const std::string& strFileName ) override;

  virtual UniformDefinitionBase GetUniformImpl( const char* name ) const override;

  virtual void SetUniformImpl( const UniformDefinition<float>& def, const float& value ) override;
  virtual void SetUniformImpl( const UniformDefinition<int32>& def, const int32& value ) override;
  virtual void SetUniformImpl( const UniformDefinition<Vector2>& def, const Vector2& value ) override;
  virtual void SetUniformImpl( const UniformDefinition<Vector4>& def, const Vector4& value ) override;
  virtual void SetUniformImpl( const UniformDefinition<Matrix3>& def, const Matrix3& value ) override;
  virtual void SetUniformImpl( const UniformDefinition<Matrix4>& def, const Matrix4& value ) override;
  // virtual void SetUniformImpl( const UniformDefinition<Sampler>& def, const Sampler& value ) override;
  virtual void SetUniformImpl( const UniformDefinition<Texture*>& def, const Texture* value ) override;
}; // RenderSystem

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GL_RENDERSYSTEM_OGL_H_
