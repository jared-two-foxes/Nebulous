#include "Win32Window.h"

using namespace Nebulae;


Win32Window::Win32Window( const std::wstring& strClassName, Win32Window* pParent )
: Window(),
	m_hWnd( NULL ),
	m_hDC( NULL ),
  m_strClassName( strClassName ),
  m_pParent( pParent )
{}

Win32Window::~Win32Window()
{
	if( IsWindow(m_hWnd) ) {
		Destroy();
	}
}

bool 
Win32Window::Initiate( void* creationData )
{
	HINSTANCE hInstance = ::GetModuleHandle( NULL );
	DWORD     dwStyle   = (WS_CLIPCHILDREN | WS_CLIPSIBLINGS), dwExStyle = NULL;
  HWND      hParent   = m_pParent ? m_pParent->GetWnd() : NULL;

  // Setup the window styles.
  if( m_pParent ) {
    dwStyle |= WS_CHILDWINDOW;
  } else {
    dwStyle |= WS_OVERLAPPEDWINDOW;
  }

  const Point position = GetPosition();
  const Point size     = GetSize();

  // Adjust window dimensions so that client area equals the passed in width & height values.
  RECT rc;
  ::SetRect( &rc, position.x, position.y, size.x, size.y );
  ::AdjustWindowRectEx( &rc, dwStyle, FALSE, dwExStyle );

  int x = m_pParent == 0 ? CW_USEDEFAULT : position.x;
  int y = m_pParent == 0 ? CW_USEDEFAULT : position.y;

	//create OS window
	m_hWnd = ::CreateWindowEx( dwExStyle, m_strClassName.c_str(), GetCaption(), dwStyle, 
    x, y, rc.right - rc.left, rc.bottom - rc.top, 
    hParent, NULL, hInstance, creationData );

	//Grab the HDC for the created window.  
	//Not sure if this is valid on control windows.
  m_hDC = ::GetDC( m_hWnd );  

	//return success.
	return true;
}

void 
Win32Window::Destroy()
{
  if( !m_hWnd ) return;

  SetWindowLongPtr( m_hWnd, GWLP_USERDATA, (LONG_PTR)nullptr );

  if( ::IsWindow( m_hWnd ) ) {
    ::DestroyWindow( m_hWnd );
  }

  //m_bActive = false;
  //m_bClosed = true;
  m_hDC  = 0;  ///< Dont need to release the HDC due to the CS_OWNDC class style
  m_hWnd = 0;
}

void
Win32Window::Show()
{
  //Show window.
  ::ShowWindow( m_hWnd, SW_NORMAL );
  ::UpdateWindow( m_hWnd );   ///< Not sure that this is necssary??
}

HWND 
Win32Window::GetWnd() const 
{ return m_hWnd; }

HDC 
Win32Window::GetDC() const 
{ return m_hDC; }

Win32Window* 
Win32Window::GetParent() const 
{ return m_pParent; }