#ifndef __NEBULAE_WINDOWREGISTRATIONUTILITY_H__
#define __NEBULAE_WINDOWREGISTRATIONUTILITY_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

	class ClassRegisterationUtility
  ///
  ///	Windows requires a class to be "registered" with the operating system which acts as a basic 'template' for basic
	///	functionality of each HWND.  This includes event handling, icons, etc.
  ///
	///	ClassRegistrationUtility provides a convenient interface of registering and unregistering classes with the os.
	///
	{
  private:
    struct OSClassRegistration
    {
      std::wstring name;
      HINSTANCE    instance;
    };

  private:
    std::vector<OSClassRegistration > m_registeredClasses;

	  public:
		  ClassRegisterationUtility();
		  ~ClassRegisterationUtility();

      /**
		   *	Register a window class with the operating system.  This must be done with any 'manual' class that you
       *  wish to use before it can be created.
       *
       *  @param className
       *    The name with which to register (and later identify) the template.
       *
       *  @param lpfnWndProc
       *    The callback function to use to recieve operating events that relate to the registered window
       *    type.
       *
       *  @param hInstance
       *    The handle the process with which the registration applies.
       *
       *  @param hIcon
       *    The icon to use for the window once created.
       *
       *  @return
       *    true when the registration was successful, otherwise false.  See 
       *    https://msdn.microsoft.com/en-us/library/windows/desktop/ms633587(v=vs.85).aspx for reasons why
       *    the registration could fail.
       */
      bool Register( const std::wstring& strClassName, WNDPROC lpfnWndProc, HINSTANCE hInstance, HICON hIcon );

      /**
       *  Remove a previously registered HWND template.    
       *
       *  @param className
       *    The name of the template to deregister.
       */
		  void Unregister( const std::wstring& strClassName );


      bool IsClassRegistered( const std::wstring& className );

	}; // ClassRegistrationUtility

} // Nebulae

#endif // __NEBULAE_WINDOWREGISTRATIONUTILITY_H__