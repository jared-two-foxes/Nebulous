#ifndef NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2INPUTLAYOUTIMPL_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2INPUTLAYOUTIMPL_H_

#include <GLES2/config.h>

#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>

namespace Nebulae
{

class ProgramObject;

/**
 *  GLES2InputLayoutImpl.
 */
class GLES2InputLayoutImpl : public InputLayoutImpl
{
private:
  std::vector<uint32> m_boundIndices;

public:
  GLES2InputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader );
  virtual ~GLES2InputLayoutImpl();

  // functions
  virtual bool Load() override;
  void Unbind();
  void Bind( ProgramObject* programObject );

private:
  GLint BindAttribute( GLuint iHandle, const char* attribute_name, GLint iSize, GLenum iFormat, int32 iStride,
                       std::size_t offset );
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2INPUTLAYOUTIMPL_H_