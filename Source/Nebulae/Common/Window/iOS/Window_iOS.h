#ifndef NEBULAE_COMMON_WINDOW_IOS_WINDOW_IOS_H_
#define NEBULAE_COMMON_WINDOW_IOS_WINDOW_IOS_H_

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/Window.h>

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#endif

namespace Nebulae
{

/** Window_iOS.
 *
 *  This is basically a wrapper for the iOS UIWindow objc class. The UIWindow objc class provides an
 *  area for displaying its views and to distribute events to the views. Unless an app can display content
 *  on an external device screen, an app has only one window. To change the content your app displays, you
 *  can change the window�s root view; you don�t create a new window.
 *
 */
class Window_iOS : public Window
{
public:
  Window_iOS();
  ~Window_iOS() override;
  bool Initiate( void* creationData ) override;
  void Destroy() override;
  void Show() override;

#ifdef __OBJC__
  UIWindow* GetHandle() const { return m_pWindow; }

protected:
  UIWindow* m_pWindow;
#endif
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_WINDOW_IOS_WINDOW_IOS_H_