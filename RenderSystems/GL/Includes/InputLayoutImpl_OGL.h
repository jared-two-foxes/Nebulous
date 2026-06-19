#ifndef NEBULAE_RENDERSYSTEMS_GL_INPUTLAYOUTIMPL_OGL_H_
#define NEBULAE_RENDERSYSTEMS_GL_INPUTLAYOUTIMPL_OGL_H_

#include <Nebulae/Alpha/InputLayout/InputLayoutImpl.h>


namespace Nebulae
{

class ProgramObject;

class InputLayoutImpl_OGL : public InputLayoutImpl
{
public:
  InputLayoutImpl_OGL( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader );
  virtual ~InputLayoutImpl_OGL();

  virtual bool Load() override;
  void Bind( ProgramObject* programObject );
  static void initiateFunctions();
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GL_INPUTLAYOUTIMPL_OGL_H_