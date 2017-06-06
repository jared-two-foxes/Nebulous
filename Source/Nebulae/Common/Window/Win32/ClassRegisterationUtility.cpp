#include "ClassRegisterationUtility.h"

using namespace Nebulae;


ClassRegisterationUtility::ClassRegisterationUtility()
{}

ClassRegisterationUtility::~ClassRegisterationUtility()
{}


bool 
ClassRegisterationUtility::Register( const std::wstring& strClassName, WNDPROC lpfnWndProc, HINSTANCE hInstance, HICON hIcon )
{
  // Check that the class is not already registered?
  NE_ASSERT( !IsClassRegistered(strClassName), "Windows class named %s is already registered", strClassName.c_str() )();
  if( IsClassRegistered(strClassName) )
  {
    // Haven't done any form of checking of the contents of the currently registered window class.
    return true;
  }

	// Setup WNDCLASEX structure
	WNDCLASSEX wcex;
	ZeroMemory( &wcex, sizeof( WNDCLASSEX ) );
  wcex.cbSize        = sizeof( WNDCLASSEX );
  wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wcex.lpfnWndProc   = lpfnWndProc;
  wcex.hIcon         = hIcon;
  wcex.hInstance     = hInstance;
  wcex.lpszClassName = strClassName.c_str();

	// Register the class
	ATOM ret = ::RegisterClassEx( &wcex );
  if( ret != 0 )
  {
	  // Store the class name so that it can be unregistered later if required.
	  m_registeredClasses.push_back( OSClassRegistration() );
    OSClassRegistration& entry = m_registeredClasses.back();
    entry.name     = strClassName;
    entry.instance = hInstance;
  }

	return (ret != 0);
}

void 
ClassRegisterationUtility::Unregister( const std::wstring& strClassName )
{
  std::vector<OSClassRegistration >::iterator it = std::find_if( m_registeredClasses.begin(), m_registeredClasses.end(), [&](OSClassRegistration& entry) {
    return (entry.name == strClassName);
  });

  if( it != m_registeredClasses.end() )
  {
    ::UnregisterClass( it->name.c_str(), it->instance );
  }
}


bool 
ClassRegisterationUtility::IsClassRegistered( const std::wstring& className )
{
  std::vector<OSClassRegistration >::iterator it = std::find_if( m_registeredClasses.begin(), m_registeredClasses.end(), [&](OSClassRegistration& entry) {
    return (entry.name == className);
  });

  return (it != m_registeredClasses.end());
}