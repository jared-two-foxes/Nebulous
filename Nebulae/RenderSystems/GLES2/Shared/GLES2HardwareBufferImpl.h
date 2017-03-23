#ifndef __NEBULAE_GLES2HARDWAREBUFFERIMPL_H__
#define __NEBULAE_GLES2HARDWAREBUFFERIMPL_H__

#include <GLES2/config.h>

#include <Nebulae/Alpha/Buffer/HardwareBufferImpl.h>

namespace Nebulae
{

/** GLES2HardwareBufferImpl.
	*/
class GLES2HardwareBufferImpl : public HardwareBufferImpl
{
protected:
		GLuint m_iHandle;
		GLenum m_iOpenGlBindFlags;
		GLenum m_iOpenGlUsage;

	public:
		GLES2HardwareBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem );
		virtual ~GLES2HardwareBufferImpl();

		//functions
		void Bind();
		virtual bool  Load() override;
    virtual void  WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer ) override;
    virtual void* Lock( std::size_t offset, size_t length, LockOptions options ) override;
    virtual void  Unlock() override;

		//getters
		GLuint GetHandle() const { return m_iHandle; }

  private:
    GLenum GetGLUsage( const Flags<HardwareBufferUsage>& usage );

};

} //Nebulae

#endif // __NEBULAE_GLES2HARDWAREBUFFERIMPL_H__