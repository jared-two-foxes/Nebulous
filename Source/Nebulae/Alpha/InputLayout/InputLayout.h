#ifndef __INPUTLAYOUT_H__
#define __INPUTLAYOUT_H__

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
  InputLayoutImpl* m_impl;

public:
  InputLayout( const std::string& strName, RenderSystem* renderDevice );
  ~InputLayout();

  void SetImpl( InputLayoutImpl* impl );
  InputLayoutImpl* GetImpl() const;

protected:
  virtual bool LoadImpl_( File* is ) override;
  virtual bool UnloadImpl_() override;
};

} // namespace Nebulae

#endif // __INPUTLAYOUT_H__