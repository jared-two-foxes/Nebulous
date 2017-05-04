#ifndef __NEBULAE_WIN32WINDOW_H__
#define __NEBULAE_WIN32WINDOW_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

/** A simple wrapper of a HWND that utilized the Nebulae::Window interface.
	*/
class Win32Window : public Window
{
private:
  HWND         m_hWnd;
  HDC          m_hDC;
  std::wstring m_strClassName;
  Win32Window* m_pParent;
    
  public:
		Win32Window( const std::wstring& strClassName, Win32Window* pParent );
		virtual ~Win32Window();
    virtual bool Initiate( void* creationData );
    virtual void Destroy();
    virtual void Show();

		HWND GetWnd() const;
		HDC GetDC() const;
    Win32Window* GetParent() const;

}; //Win32Window

}

#endif // __WIN32_WINDOW_H__