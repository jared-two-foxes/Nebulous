#ifndef __PASSIMPL_OPENGL_H__
#define __PASSIMPL_OPENGL_H__

#include <Config.h>

#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

namespace Nebulae
{

class HardwareShader;

class ProgramObject
{
protected:
	GLuint               m_handle;
  HardwareShader*      m_vertexShader;
  HardwareShader*      m_fragmentShader;
  UniformDefinitionMap m_uniformDefinitions;

  public:
	  ProgramObject( HardwareShader* vertexShader, HardwareShader* fragmentShader );
	  virtual ~ProgramObject();

	  GLuint          GetHandle() const;
    HardwareShader* GetVertexShader() const;
    HardwareShader* GetFragmentShader() const;
    
    const UniformDefinition FindUniformByName( const char* name ) const;

	  virtual bool Load();
	  void         UseProgram();
    static void  initiateFunctions();
	  
};

}

#endif // __PASSIMPL_OPENGL_H__