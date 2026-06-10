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
  ClassRegisterationUtility* m_pRegisterationUtility{ nullptr };
  std::vector<WindowPtr> m_windows;

public:
  Win32Platform();
  ~Win32Platform() override;

  bool Initiate() override;
  void Destroy() override;
  bool MessagePump() override;
  void SetWorkingDirectory( const char* directory ) override;

  WindowPtr CreateApplicationWindow( int x, int y, int w, int h, WindowPtr pParent = nullptr ) override;
  void DestroyWindow( WindowPtr window ) override;
  WindowPtr FindWindowFromHandle( HWND handle );

}; // Win32Platform

} // namespace Nebulae

#endif // __NEBULAE_WIN32_PLATFORM_H__