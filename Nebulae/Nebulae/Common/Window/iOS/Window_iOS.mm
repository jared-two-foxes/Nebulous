#include "Window_iOS.h"

#import <QuartzCore/QuartzCore.h>

using namespace Nebulae;

Window_iOS::Window_iOS()
  : m_pWindow(nil)
{

}

Window_iOS::~Window_iOS()
{
    Destroy();
}

void Window_iOS::Destroy(void)
{
    [m_pWindow release];
    m_pWindow = nil;
}

//
// Unsure about how we are handling the view associated with the window.  I think I'd prefer to make the UIView decoupled to the window class
// Thus allowing us to reuse the window class and be able to say pass in any old view.
//
bool Window_iOS::Initiate( void* creationData )
{
  // This method is called from within create() and after parameters have been parsed.
  // If the window, view or view controller objects are nil at this point, it is safe
  // to assume that external handles are either not being used or are invalid and
  // we can create our own.
  
  // Attempt to create the platform window.
  m_pWindow = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] retain];
  assert( m_pWindow != nil && "Window_iOS: Failed to create native window" );
  if( m_pWindow ) {
    m_pWindow.backgroundColor = [UIColor whiteColor];
    
    CGRect  rect   = [[UIScreen mainScreen] bounds];
    CGFloat scale  = [[UIScreen mainScreen] scale];
    float   width  = rect.size.width * scale;
    float   height = rect.size.height * scale;
    MoveAndResize( 0, 0, width, height );
    
    return true;
  }
  
  return false;
}
	
void Window_iOS::Show()
{
  [m_pWindow makeKeyAndVisible];
}
