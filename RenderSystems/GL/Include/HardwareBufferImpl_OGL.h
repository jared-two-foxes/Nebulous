#ifndef __HARDWAREBUFFERIMPL_OGL_H__
#define __HARDWAREBUFFERIMPL_OGL_H__

#include <GL/config.h>

#include <Nebulae/Alpha/Buffer/HardwareBufferImpl.h>


namespace Nebulae
{

	/** HardwareBufferImpl_OGL.
	 */
	class HardwareBufferImpl_OGL : public HardwareBufferImpl
	{
	protected:
		GLuint m_iHandle;
		GLenum m_iOpenGlBindFlags;
		GLenum m_iOpenGlUsage;

	public:
		HardwareBufferImpl_OGL( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* sysMem );
		virtual ~HardwareBufferImpl_OGL();

		void Bind();
		virtual bool  Load() override;
		virtual void  WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer ) override;
		virtual void* Lock( std::size_t offset, size_t length, LockOptions options ) override;
		virtual void  Unlock() override;

		GLuint GetHandle() const { return m_iHandle; }

		static void initiateFunctions();

	};

} 

#endif // __TEXTUREIMPL_OGL_H__