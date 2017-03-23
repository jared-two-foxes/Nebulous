//
//  EAGLViewController.h
//  Nebulae
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//
#ifndef __NEBULAE_EAGL2VIEWCONTROLLER_H__
#define __NEBULAE_EAGL2VIEWCONTROLLER_H__

#ifdef __OBJC__

#import <UIKit/UIKit.h>

@class EAGLContext;

namespace Nebulae {
  class TouchesDelegate;
}

@interface EAGLViewController : UIViewController <UIAccelerometerDelegate>
{
  Nebulae::TouchesDelegate* m_touchesDelegate;
}

- (void)setTouchesDelegate: (Nebulae::TouchesDelegate*) delegate;

@property (retain, nonatomic) EAGLContext     *context;

@end

#endif // __OBJC__

#endif // __NEBULAE_EAGL2VIEWCONTROLLER_H__