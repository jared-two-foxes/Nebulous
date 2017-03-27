#ifndef __NEBULAE_WINDOW_H__
#define __NEBULAE_WINDOW_H__

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
   *  can change the window’s root view; you don’t create a new window.
   *
   */
  class Window_iOS : public Window
  {
  public:
    Window_iOS();
    virtual ~Window_iOS();
    virtual bool Initiate( void* creationData );
    virtual void Destroy();
    virtual void Show();

#ifdef __OBJC__
    UIWindow* GetHandle() const { return m_pWindow; }
  protected:
    UIWindow* m_pWindow;
#endif
	  
  };
	
}

#endif // __NEBULAE_EAGL2WINDOW_H__