#ifndef NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2HARDWARESHADERIMPL_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2HARDWARESHADERIMPL_H_

#include <GLES2/config.h>

#include <Nebulae/Alpha/Shaders/HardwareShaderImpl.h>

namespace Nebulae
{
/**
 *  GLES2HardwareShaderImpl.
 */
class GLES2HardwareShaderImpl : public HardwareShaderImpl
{
protected:
  GLuint m_iHandle;

public:
  GLES2HardwareShaderImpl( const std::string& strName, HardwareShaderType eType );
  virtual ~GLES2HardwareShaderImpl();

  // getters
  GLuint GetHandle() const { return m_iHandle; }

  // functions
  virtual bool Load( File& is ) override;
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2HARDWARESHADERIMPL_H_