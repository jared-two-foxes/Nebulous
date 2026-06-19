#ifndef NEBULAE_BETA_MATERIAL_MATERIALSERIALIZER_H_
#define NEBULAE_BETA_MATERIAL_MATERIALSERIALIZER_H_

#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Material;
class Pass;
class RenderSystem;

class MaterialSerializer
{
public:
  typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
  typedef std::weak_ptr<Material> MaterialPtr;

private:
  RenderSystemPtr m_renderDevice;

public:
  explicit MaterialSerializer( RenderSystemPtr renderDevice );

  bool Serialize( File& istream, Material* material );

private:
  bool ProcessPass( Json::Value& value, Material* material );
  bool ProcessShader( Json::Value& value, HardwareShaderType shaderType, Material* material, Pass* pass );
};

} // namespace Nebulae

#endif // NEBULAE_BETA_MATERIAL_MATERIALSERIALIZER_H_