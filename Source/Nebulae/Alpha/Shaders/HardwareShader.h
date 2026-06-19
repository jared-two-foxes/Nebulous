#ifndef NEBULAE_ALPHA_SHADERS_HARDWARESHADER_H_
#define NEBULAE_ALPHA_SHADERS_HARDWARESHADER_H_

#include <Nebulae/Alpha/Resource/Resource.h>

namespace Nebulae
{

class HardwareShaderImpl;

enum HardwareShaderType
{
  VERTEX_SHADER,
  PIXEL_SHADER
};

/** HardwareShader.
 */
class HardwareShader : public Resource
{
protected:
  HardwareShaderImpl* m_impl;

public:
  HardwareShader( const std::string& name, RenderSystem* renderer );
  virtual ~HardwareShader();

  /** Get a the lower-level implementation of the hardware shader (Used by the RenderSystem). */
  HardwareShaderImpl* GetImpl() const;

  void SetImpl( HardwareShaderImpl* impl );


private:
  bool LoadImpl_( File* is ) override;
  bool UnloadImpl_() override;
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_SHADERS_HARDWARESHADER_H_