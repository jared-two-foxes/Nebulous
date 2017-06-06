#ifndef __NEBULAE_WIN32_PLATFORM_H__
#define __NEBULAE_WIN32_PLATFORM_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Platform/Platform.h>

namespace Nebulae
{

	class ClassRegisterationUtility;
	
	/**
	 * Win32Platform. 
	 */
	class Win32Platform : public Platform
	{
	private:
		ClassRegisterationUtility* m_pRegisterationUtility;
    std::vector<WindowPtr >    m_windows;

	  public:
		  Win32Platform();
		  virtual ~Win32Platform();

		  virtual bool Initiate();
		  virtual void Destroy();
		  virtual bool MessagePump();
		  virtual void SetWorkingDirectory( const char* directory );

      virtual WindowPtr CreateApplicationWindow( int x, int y, int w, int h, WindowPtr pParent = NULL );
      virtual void DestroyWindow( WindowPtr window );
      WindowPtr FindWindowFromHandle( HWND handle );

	}; //Win32Platform

} //Nebulae

#endif // __NEBULAE_WIN32_PLATFORM_H__