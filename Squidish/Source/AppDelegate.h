//
//  AppDelegate.h
//  simpleExampleApp
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
  // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
  // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
  // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
  // isn't available.
  CADisplayLink* m_pDisplayLink;
  NSDate*        m_pDate;
  NSTimeInterval m_LastFrameTime;
}

- (void)renderOneFrame:(id)sender;


@property (nonatomic, retain) UIWindow        *window;
@property (nonatomic, retain) CMMotionManager *motionManager;

@property (nonatomic) NSTimeInterval LastFrameTime;

@end
