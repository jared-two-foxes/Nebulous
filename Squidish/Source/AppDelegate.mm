//
// AppDelegate.mm
// testingPremakeApp
//
// Created by jessica mccormack on 16/11/13.
// Copyright (c) 2013 jessica mccormack. All rights reserved.
//

#import "AppDelegate.h"

#include "ExampleApplication.h"

#include <Nebulae/Common/Window/iOS/Window_iOS.h>
#include <Nebulae/Beta/StateStack/StateStack.h>

@interface AppDelegate () {
  Game::ExampleApplication* _application;
  Nebulae::Window_iOS*      _window;
}
@end

@implementation AppDelegate

@dynamic    LastFrameTime;
@dynamic    window;             //@note [jared.watt] dynamic rather than synthesised as we want to provide the accessor functions manually.

- (UIWindow *)window
{
  assert( _window );
  return _window->GetHandle();
}

- (void)setwindow:(UIWindow *)handle
{
  // @note [jared.watt 12.04.2014] Blank implementation of the setwindow function as we dont
  // actually want to allow setting the window as it will mess with our Nebulae engine
  // window creation.
  int breakpoint = 12;
}

- (NSTimeInterval)LastFrameTime
{
  return m_LastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
  // Frame interval defines how many display frames must pass between each time the
  // display link fires. The display link will only fire 30 times a second when the
  // frame internal is two on a display that refreshes 60 times a second. The default
  // frame interval setting of one will fire 60 times a second when the display refreshes
  // at 60 times a second. A frame interval setting of less than one results in undefined
  // behavior.
  if( frameInterval >= 1 )
  {
    m_LastFrameTime = frameInterval;
  }
}

- (void)renderOneFrame:(id)sender
{
  // NSTimeInterval is a simple typedef for double
  NSTimeInterval currentFrameTime    = -[m_pDate timeIntervalSinceNow];
  NSTimeInterval differenceInSeconds = (currentFrameTime - m_LastFrameTime) * 1000000.0f;
  m_LastFrameTime                    = currentFrameTime;
  
  _application->ProcessApplicationLoop( uint64(differenceInSeconds) );
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will
  // result in a warning, but can be dismissed if the system version runtime check for
  // CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
  // not be called in system versions earlier than 3.1.
  m_pDate         = [[NSDate alloc] init];
  m_LastFrameTime = -[m_pDate timeIntervalSinceNow];
  m_pDisplayLink  = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
  
  [m_pDisplayLink setFrameInterval:m_LastFrameTime];
  [m_pDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
 
  self.motionManager = [[CMMotionManager alloc] init];
  self.motionManager.accelerometerUpdateInterval = 1.0 / 60.0;
  
  if( [self.motionManager isAccelerometerAvailable] )
  {
    NSOperationQueue* operationQueue = [[NSOperationQueue alloc] init];
  
    [self.motionManager startAccelerometerUpdatesToQueue:operationQueue
      withHandler:^(CMAccelerometerData *data, NSError *error) {
        dispatch_async(dispatch_get_main_queue(), ^{
          float dx = data.acceleration.x;
          float dy = data.acceleration.y;
          float dz = data.acceleration.z;
          
          if( dx < -0.25f ) {
            _window->KeyPressed( Nebulae::VKC_LEFT );
          } else {
            _window->KeyReleased( Nebulae::VKC_LEFT );
          }
          
          if( dx > 0.25f ) {
            _window->KeyPressed( Nebulae::VKC_RIGHT );
          } else {
            _window->KeyReleased( Nebulae::VKC_RIGHT );
          }
        });
      }
    ];
  }
  
  _window = new Nebulae::Window_iOS();
  _window->Initiate();

  _application = new Game::ExampleApplication();
  _application->SetMainWindow( _window );
  _application->Init();
  
  _window->Show();
  
  return YES;
}
                            
- (void)applicationWillResignActive:(UIApplication *)application
{
  // Sent when the application is about to move from active to inactive state. This can
  // occur for certain types of temporary interruptions (such as an incoming phone call
  // or SMS message) or when the user quits the application and it begins the transition
  // to the background state.
  // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES
  // frame rates. Games should use this method to pause the game.
  //[glView stopRenderLoop];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
  // Use this method to release shared resources, save user data, invalidate timers, and
  // store enough application state information to restore your application to its current
  // state in case it is terminated later.
  // If your application supports background execution, this method is called instead of
  // applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
  // Called as part of the transition from the background to the inactive state; here you
  // can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
  // Restart any tasks that were paused (or not yet started) while the application was
  // inactive. If the application was previously in the background, optionally refresh the
  // user interface.
  //[glView startRenderLoop];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  // Called when the application is about to terminate. Save data if appropriate. See also
  // applicationDidEnterBackground:.
  //[glView stopRenderLoop];
}

@end
