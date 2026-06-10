#include "Win32Window.h"

#include <utility>

using namespace Nebulae;


Win32Window::Win32Window( std::wstring strClassName, Win32Window* pParent )
  : m_strClassName( std::move( strClassName ) ), m_pParent( pParent )
{
}

Win32Window::~Win32Window()
{
  if ( IsWindow( m_hWnd ) != 0 )
  {
    Destroy();
  }
}

bool Win32Window::Initiate( void* creationData )
{
  HINSTANCE hInstance = ::GetModuleHandle( nullptr );
  DWORD dwStyle = ( WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
  DWORD dwExStyle = NULL;
  HWND hParent = ( m_pParent != nullptr ) ? m_pParent->GetWnd() : nullptr;

  // Setup the window styles.
  if ( m_pParent != nullptr )
  {
    dwStyle |= WS_CHILDWINDOW;
  }
  else
  {
    dwStyle |= WS_OVERLAPPEDWINDOW;
  }

  const Point POSITION = GetPosition();
  const Point SIZE = GetSize();

  // Adjust window dimensions so that client area equals the passed in width & height values.
  RECT rc;
  ::SetRect( &rc, POSITION.x, POSITION.y, SIZE.x, SIZE.y );
  ::AdjustWindowRectEx( &rc, dwStyle, FALSE, dwExStyle );

  int x = m_pParent == nullptr ? CW_USEDEFAULT : POSITION.x;
  int y = m_pParent == nullptr ? CW_USEDEFAULT : POSITION.y;

  // create OS window
  m_hWnd = ::CreateWindowEx( dwExStyle, m_strClassName.c_str(), GetCaption(), dwStyle, x, y, rc.right - rc.left,
                             rc.bottom - rc.top, hParent, nullptr, hInstance, creationData );

  // Grab the HDC for the created window.
  // Not sure if this is valid on control windows.
  m_hDC = ::GetDC( m_hWnd );

  // return success.
  return true;
}

void Win32Window::Destroy()
{
  if ( m_hWnd == nullptr )
  {
    return;
  }

  SetWindowLongPtr( m_hWnd, GWLP_USERDATA, ( LONG_PTR ) nullptr );

  if ( ::IsWindow( m_hWnd ) != 0 )
  {
    ::DestroyWindow( m_hWnd );
  }

  // m_bActive = false;
  // m_bClosed = true;
  m_hDC = nullptr; ///< Dont need to release the HDC due to the CS_OWNDC class style
  m_hWnd = nullptr;
}

void Win32Window::Show()
{
  // Show window.
  ::ShowWindow( m_hWnd, SW_NORMAL );
  ::UpdateWindow( m_hWnd ); ///< Not sure that this is necssary??
}

HWND Win32Window::GetWnd() const { return m_hWnd; }

HDC Win32Window::GetDC() const { return m_hDC; }

Win32Window* Win32Window::GetParent() const { return m_pParent; }