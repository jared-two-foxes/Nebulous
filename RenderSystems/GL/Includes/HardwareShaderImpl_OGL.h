#ifndef NEBULAE_RENDERSYSTEMS_GL_HARDWARESHADERIMPL_OGL_H_
#define NEBULAE_RENDERSYSTEMS_GL_HARDWARESHADERIMPL_OGL_H_

#include <Config.h>

#include <Nebulae/Alpha/Shaders/HardwareShaderImpl.h>

namespace Nebulae
{
/** HardwareShaderImpl_OGL.
 */
class HardwareShaderImpl_OGL : public HardwareShaderImpl
{
protected:
  GLuint m_iHandle;

public:
  HardwareShaderImpl_OGL( const std::string& strName, HardwareShaderType eType );
  virtual ~HardwareShaderImpl_OGL();

  // getters
  GLuint GetHandle() const { return m_iHandle; }

  // functions
  virtual bool Load( File& is ) override;

  static void initiateFunctions();
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GL_HARDWARESHADERIMPL_OGL_H_