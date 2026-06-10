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
  HWND m_hWnd{ nullptr };
  HDC m_hDC{ nullptr };
  std::wstring m_strClassName;
  Win32Window* m_pParent;

public:
  Win32Window( std::wstring strClassName, Win32Window* pParent );
  ~Win32Window() override;
  bool Initiate( void* creationData ) override;
  void Destroy() override;
  void Show() override;

  HWND GetWnd() const;
  HDC GetDC() const;
  Win32Window* GetParent() const;

}; // Win32Window

} // namespace Nebulae

#endif // __WIN32_WINDOW_H__