#ifndef NEBULAE_ALPHA_INPUTLAYOUT_INPUTLAYOUTIMPL_H_
#define NEBULAE_ALPHA_INPUTLAYOUT_INPUTLAYOUTIMPL_H_

/// #include <Nebulae/Common/Common.h>

namespace Nebulae
{

class HardwareShader;
class VertexDeceleration;

class InputLayoutImpl
{
protected:
  const VertexDeceleration* m_vertexDecl;
  const HardwareShader* m_vertexShader;

public:
  InputLayoutImpl( const VertexDeceleration* pVertexDecl, const HardwareShader* pVertexShader );
  virtual ~InputLayoutImpl();

  const VertexDeceleration* GetVertexDecleration() const;
  const HardwareShader* GetVertexShader() const;

  virtual bool Load();
  virtual bool Unload();
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_INPUTLAYOUT_INPUTLAYOUTIMPL_H_