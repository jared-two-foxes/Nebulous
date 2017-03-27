
#ifndef __NEBULAE_GUISAMPLE_iOS_H__
#define __NEBULAE_GUISAMPLE_iOS_H__

// #include "OgrePlatform.h"

// #if NEBULAE_PLATFORM != NEBULAE_PLATFORM_APPLE_IOS
// #error This header is for use with iOS only
// #endif

#ifdef __OBJC__

#import <UIKit/UIKit.h> 
#import <QuartzCore/QuartzCore.h>

#include <Nebulae/Common/Window/iOS/Window_iOS.h>
#include <Nebulae/RenderModules/GLES2/EAGL/GLES2RenderSystem.h>

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
    // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
    // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
    // isn't available.
    CADisplayLink* m_pDisplayLink;   
    NSDate* m_pDate;
    NSTimeInterval m_LastFrameTime;
	
    Nebulae::Window_iOS* m_pWindow;
    Nebulae::GLES2RenderSystem* m_pRenderSystem;
	
	bool m_bFirstInit;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (nonatomic) NSTimeInterval m_LastFrameTime;

//@property (nonatomic, retain) IBOutlet UIWindow *window;
//@property (nonatomic, retain) IBOutlet UIView *view;

@end

#endif // __OBJC__

#endif //__NEBULAE_GUISAMPLE_iOS_H__
