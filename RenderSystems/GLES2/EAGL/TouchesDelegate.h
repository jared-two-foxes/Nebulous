//
//  TouchesDelegate.h
//  Nebulae
//
//  Created by jessica mccormack on 6/02/13.
//  Copyright 2013 auckland university. All rights reserved.
//
#ifndef __NEBULAE_TOUCHESDELEGATE_H__
#define __NEBULAE_TOUCHESDELEGATE_H__

#ifdef __OBJC__

#include <Nebulae/Common/Common.h>
#import  <UIKit/UIKit.h>

namespace Nebulae {

#define MAX_TOUCHES     5

class Window;

class TouchesDelegate
{
private:
  Window*  m_window;
  float    m_scaleFactor;
  int      m_touchCount; 
  UITouch* m_touchPoints[MAX_TOUCHES];
  CGPoint  m_lastSingleTouch;

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
  
}

#endif

#endif // __NEBULAE_TOUCHESDELEGATE_H__
