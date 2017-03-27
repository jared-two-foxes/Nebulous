#import "AppDelegate.h"

//#import "Nebulae/RootViewController.h"
//#import "Nebulae/RootView.h"

@implementation AppDelegate

@dynamic m_LastFrameTime;
//@synthesize window;
//@synthesize view;

- (NSTimeInterval)m_LastFrameTime
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
    if ( frameInterval >= 1 )
    {
        m_LastFrameTime = frameInterval;
    }
}

- (void)go 
{
	//UIWindow* tmpwindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	
	//UIView* tmpview = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	//tmpview.backgroundColor = [UIColor whiteColor];
	//[tmpwindow addSubview:tmpview];
	//[tmpview release];
	//[tmpwindow makeKeyAndVisible];
	
	m_pWindow = new Nebulae::Window_iOS();
	m_pWindow->Initiate();
	
    m_pRenderSystem = new Nebulae::GLES2RenderSystem( m_pWindow );
    m_pRenderSystem->Initiate();

    // Set clear color to red so that we can tell if its working. (Default Window_iOS background color is black)
    m_pRenderSystem->SetClearColour( 1.0, 0.0, 0.0, 1.0 );

    // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
    // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
    // not be called in system versions earlier than 3.1.
    m_pDate = [[NSDate alloc] init];
    m_LastFrameTime = -[m_pDate timeIntervalSinceNow];
    
    m_pDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
    [m_pDisplayLink setFrameInterval:m_LastFrameTime];
    [m_pDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

	m_bFirstInit = true; 
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    m_LastFrameTime = 1;
    m_pDisplayLink = nil;

    [self go];

    return YES;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	if (m_pWindow)
		delete m_pWindow;
	
	//[self.window release];
	//[self.view release];
	
    [m_pDate release];
    m_pDate = nil;
    
    [m_pDisplayLink invalidate];
    m_pDisplayLink = nil;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	
}

- (void)renderOneFrame:(id)sender
{
    // NSTimeInterval is a simple typedef for double
    NSTimeInterval currentFrameTime = -[m_pDate timeIntervalSinceNow];
    //NSTimeInterval differenceInSeconds = currentFrameTime - m_LastFrameTime;
    m_LastFrameTime = currentFrameTime;

	if (m_bFirstInit)
	{
		m_pRenderSystem->CreateFramebuffer();
		m_bFirstInit = false;
	}
	
    m_pRenderSystem->Clear();
    m_pRenderSystem->SwapBuffers();
}

@end