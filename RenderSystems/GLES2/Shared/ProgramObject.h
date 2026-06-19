#ifndef NEBULAE_RENDERSYSTEMS_GLES2_SHARED_PROGRAMOBJECT_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_SHARED_PROGRAMOBJECT_H_

#include <GLES2/config.h>

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

  const UniformDefinition FindUniformByName( const char* name ) const;

  bool Load();
  void UseProgram();

  static void initiateFunctions();
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GLES2_SHARED_PROGRAMOBJECT_H_