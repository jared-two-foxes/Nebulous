
#ifndef __NEBULAE_GUISAMPLE_iOS_H__
#define __NEBULAE_GUISAMPLE_iOS_H__

// #include "OgrePlatform.h"

// #if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
// #error This header is for use with iOS only
// #endif

#ifdef __OBJC__

#import <UIKit/UIKit.h> 
#import <QuartzCore/QuartzCore.h>

#include <Nebulae/Beta/Application/Application.h>


@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    Nebulae::Application app;

    // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
    // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
    // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
    // isn't available.
    CADisplayLink* m_pDisplayLink;   
    NSDate* m_pDate;
    NSTimeInterval m_LastFrameTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (nonatomic) NSTimeInterval mLastFrameTime;

@end

#endif // __OBJC__

#endif //__NEBULAE_GUISAMPLE_iOS_H__
