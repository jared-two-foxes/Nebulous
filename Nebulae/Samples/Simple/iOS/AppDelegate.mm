#import "AppDelegate.h"

#include <Nebulae/Beta/StateStack/StateStack.h>
//#include "SimpleState.h"

@implementation AppDelegate

@dynamic mLastFrameTime;

- (NSTimeInterval)mLastFrameTime
{
    return mLastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if ( frameInterval >= 1 )
    {
        mLastFrameTime = frameInterval;
    }
}

- (void)go 
{
    //Nebulae::StateStack* stack = new Nebulae::StateStack();
    //stack->PushState( new Sample::SimpleState( "Sample" ) );
    //stack->Run();
    
    //app = stack;
    
    // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
    // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
    // not be called in system versions earlier than 3.1.
    mDate = [[NSDate alloc] init];
    mLastFrameTime = -[mDate timeIntervalSinceNow];
    
    mDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
    [mDisplayLink setFrameInterval:mLastFrameTime];
    [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    mLastFrameTime = 1;
    mDisplayLink = nil;

    [self go];

    return YES;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [mDate release];
    mDate = nil;
    
    [mDisplayLink invalidate];
    mDisplayLink = nil;
}

- (void)applicationWillResignActive:(UIApplication *)application
{

}

- (void)renderOneFrame:(id)sender
{
    // NSTimeInterval is a simple typedef for double
    NSTimeInterval currentFrameTime = -[mDate timeIntervalSinceNow];
    NSTimeInterval differenceInSeconds = currentFrameTime - mLastFrameTime;
    mLastFrameTime = currentFrameTime;

    //app->ProcessFrame( differenceInSeconds );
}

@end