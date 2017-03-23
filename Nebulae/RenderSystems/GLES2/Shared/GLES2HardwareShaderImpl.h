#ifndef __NEBULAE_GLES2HARDWARESHADERIMPL_H__
#define __NEBULAE_GLES2HARDWARESHADERIMPL_H__

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
		
    //getters
    GLuint GetHandle() const { return m_iHandle; }

    //functions
    virtual bool Load( File& is ) override;

  };

} //Nebulae

#endif // __NEBULAE_GLES2HARDWARESHADERIMPL_H__