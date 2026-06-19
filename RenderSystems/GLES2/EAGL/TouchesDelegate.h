//
//  TouchesDelegate.h
//  Nebulae
//
//  Created by jessica mccormack on 6/02/13.
//  Copyright 2013 auckland university. All rights reserved.
//
#ifndef NEBULAE_RENDERSYSTEMS_GLES2_EAGL_TOUCHESDELEGATE_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_EAGL_TOUCHESDELEGATE_H_

#ifdef __OBJC__

#include <Nebulae/Common/Common.h>
#import <UIKit/UIKit.h>

namespace Nebulae
{

#define MAX_TOUCHES 5

class Window;

class TouchesDelegate
{
private:
  Window* m_window;
  float m_scaleFactor;
  int m_touchCount;
  UITouch* m_touchPoints[MAX_TOUCHES];
  CGPoint m_lastSingleTouch;

public:
  TouchesDelegate( Window* window, float scalefactor );

  void Clear();
  void TouchesBegan( NSSet* touches, UIEvent* event );
  void TouchesMoved( NSSet* touches, UIEvent* event );
  void TouchesEnded( NSSet* touches, UIEvent* event );
  void TouchesCancelled( NSSet* touches, UIEvent* event );

private:
  void ProcessTouchEnd( UITouch* touch );
};

} // namespace Nebulae

#endif

#endif // NEBULAE_RENDERSYSTEMS_GLES2_EAGL_TOUCHESDELEGATE_H_
