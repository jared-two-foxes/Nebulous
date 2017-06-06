#ifndef NEBULAE_BETA_MATERIALSERIALIZER_H__
#define NEBULAE_BETA_MATERIALSERIALIZER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>

namespace Nebulae {

class Material;
class Pass;
class RenderSystem;
  
class MaterialSerializer
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;
  typedef std::weak_ptr<Material>        MaterialPtr;

private:
  RenderSystemPtr m_renderDevice;
  
  public:
    explicit MaterialSerializer( RenderSystemPtr renderDevice );

    bool Serialize( File& istream, Material* material );

 private:
    bool ProcessPass( Json::Value& value, Material* material );
    bool ProcessShader( Json::Value& value, HardwareShaderType shaderType, Material* material, Pass* pass );
  
};

}

#endif 