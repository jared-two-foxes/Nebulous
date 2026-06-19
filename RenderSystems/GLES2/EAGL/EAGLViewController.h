//
//  EAGLViewController.h
//  Nebulae
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//
#ifndef NEBULAE_RENDERSYSTEMS_GLES2_EAGL_EAGLVIEWCONTROLLER_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_EAGL_EAGLVIEWCONTROLLER_H_

#ifdef __OBJC__

#import <UIKit/UIKit.h>

@class EAGLContext;

namespace Nebulae
{
class TouchesDelegate;
}

@interface EAGLViewController : UIViewController <UIAccelerometerDelegate>
{
  Nebulae::TouchesDelegate* m_touchesDelegate;
}

- (void)setTouchesDelegate:(Nebulae::TouchesDelegate*)delegate;

@property( retain, nonatomic ) EAGLContext* context;

@end

#endif // __OBJC__

#endif // NEBULAE_RENDERSYSTEMS_GLES2_EAGL_EAGLVIEWCONTROLLER_H_