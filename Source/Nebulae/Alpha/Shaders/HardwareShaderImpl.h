#ifndef NEBULAE_ALPHA_HARDWARESHADERIMPL_H__
#define NEBULAE_ALPHA_HARDWARESHADERIMPL_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

namespace Nebulae
{

class HardwareShaderImpl
{
protected:
  std::string          m_fileName;           ///< Filename that this shader represents.
  HardwareShaderType   m_type;               ///< Enum of shader type.
  UniformDefinitionMap m_uniformDefinitions; ///< 

public:
  HardwareShaderImpl( const std::string& strName, HardwareShaderType eType );
  virtual ~HardwareShaderImpl();

  HardwareShaderType GetType() const;
  const UniformDefinitionMap& GetUniformDefinitions() const;

  virtual bool Load( File& is );
  virtual bool Unload();

};

}

#endif //NEBULAE_ALPHA_HARDWARESHADERIMPL_H__