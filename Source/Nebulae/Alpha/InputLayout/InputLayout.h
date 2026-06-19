#ifndef NEBULAE_ALPHA_INPUTLAYOUT_INPUTLAYOUT_H_
#define NEBULAE_ALPHA_INPUTLAYOUT_INPUTLAYOUT_H_

#include <Nebulae/Alpha/Resource/Resource.h>
#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class HardwareShader;
class InputLayoutImpl;
class VertexDeceleration;

/** InputLayout.
 */
class InputLayout : public Resource
{
protected:
  InputLayoutImpl* m_impl{ nullptr };

public:
  InputLayout( const std::string& strName, RenderSystem* renderDevice );
  ~InputLayout() override;

  void SetImpl( InputLayoutImpl* impl );
  InputLayoutImpl* GetImpl() const;

protected:
  bool LoadImpl_( File* is ) override;
  bool UnloadImpl_() override;
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_INPUTLAYOUT_INPUTLAYOUT_H_