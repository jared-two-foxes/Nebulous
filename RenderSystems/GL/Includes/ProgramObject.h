#ifndef NEBULAE_RENDERSYSTEMS_GL_PROGRAMOBJECT_H_
#define NEBULAE_RENDERSYSTEMS_GL_PROGRAMOBJECT_H_

#include <Config.h>

#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

namespace Nebulae
{

class HardwareShader;

class ProgramObject
{
protected:
  GLuint m_handle;
  HardwareShader* m_vertexShader;
  HardwareShader* m_fragmentShader;
  UniformDefinitionMap m_uniformDefinitions;

public:
  ProgramObject( HardwareShader* vertexShader, HardwareShader* fragmentShader );
  virtual ~ProgramObject();

  GLuint GetHandle() const;
  HardwareShader* GetVertexShader() const;
  HardwareShader* GetFragmentShader() const;

  const UniformDefinitionBase FindUniformByName( const char* name ) const;
  const UniformDefinitionMap& GetUniformDefinitions() const { return m_uniformDefinitions; }

  virtual bool Load();
  void UseProgram();
  static void initiateFunctions();
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GL_PROGRAMOBJECT_H_
