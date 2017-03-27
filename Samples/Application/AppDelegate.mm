//
// AppDelegate.mm
// testingPremakeApp
//
// Created by jessica mccormack on 16/11/13.
// Copyright (c) 2013 jessica mccormack. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"

#include "ExampleScreen.h"

#include <Nebulae/Common/Window/iOS/Window_iOS.h>
#include <Nebulae/Beta/StateStack/StateStack.h>

@interface AppDelegate () {
  Nebulae::StateStack* _application;
  Nebulae::Window_iOS*  _window;
  //ViewController*       _viewController;
}
@end

@implementation AppDelegate

@dynamic LastFrameTime;
@dynamic window;             //@note [jared.watt] dynamic rather than synthesised as we want to provide the accessor functions manually.
@synthesize rootViewController;

- (UIWindow *)window
{
  // @note [jared.watt] The .storyboard loading procedure seems to expect that the window will exist when
  // the accessor is called.  I am not sure if this is a common expectation in objc but if the window is
  // not yet created create it now.
  if( !_window ) {
    _window = new Nebulae::Window_iOS();
    _window->Initiate();
  }
    
  return _window->GetHandle();
}

- (void)setwindow:(UIWindow *)handle
{
  // @note [jared.watt 12.04.2014] Blank implementation of the setwindow function as we dont
  // actually want to allow setting the window as it will mess with our Nebulae engine
  // window creation.
}

/*- (ViewController *)rootViewController
{
  return _viewController;
}

- (void)setrootViewController:(ViewController *)viewController
{
  // @note [jared.watt 12.04.2014] Blank implementation of the setwindow function as we dont
  // actually want to allow setting the window as it will mess with our Nebulae engine
  // window creation.
  
  _viewController = viewController;
}*/

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

/*- (void)go
{
  // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will
  // result in a warning, but can be dismissed if the system version runtime check for 
  // CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
  // not be called in system versions earlier than 3.1.
  m_pDate = [[NSDate alloc] init];
  m_LastFrameTime = -[m_pDate timeIntervalSinceNow];

  m_pDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
  [m_pDisplayLink setFrameInterval:m_LastFrameTime];
  [m_pDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}*/

- (void)renderOneFrame:(id)sender
{
  // NSTimeInterval is a simple typedef for double
  NSTimeInterval currentFrameTime    = -[m_pDate timeIntervalSinceNow];
  NSTimeInterval differenceInSeconds = currentFrameTime - m_LastFrameTime;
  m_LastFrameTime                    = currentFrameTime;
  
  _application->ProcessFrame( differenceInSeconds );
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  // Override point for customization after application launch.
  
  self.rootViewController = [[ViewController alloc] init];
  //[self.window addSubview:rootViewController.view];
  [self.window setRootViewController:self.rootViewController];
  
  _application = new Nebulae::StateStack();
  _application->SetMainWindow( _window );
  _application->Initiate( NULL );
  _application->PushState( new Nebulae::ExampleScreen("AudioTest", _application->GetRenderSystem()) );
  
  _window->Show();
  
  // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will
  // result in a warning, but can be dismissed if the system version runtime check for
  // CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
  // not be called in system versions earlier than 3.1.
  m_pDate         = [[NSDate alloc] init];
  m_LastFrameTime = -[m_pDate timeIntervalSinceNow];
  m_pDisplayLink  = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
  
  [m_pDisplayLink setFrameInterval:m_LastFrameTime];
  [m_pDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
  
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
