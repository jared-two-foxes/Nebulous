//
//  EAGLViewController.mm
//  simpleExampleApp
//
//  Created by jessica mccormack on 16/11/13.
//  Copyright (c) 2013 jessica mccormack. All rights reserved.
//

#import "EAGLViewController.h"
#import "EAGL2View.h"

#import "TouchesDelegate.h"

@interface EAGLViewController () {

}

@end

@implementation EAGLViewController

- (void)dealloc
{
  // [self tearDownGL];

  [EAGLContext setCurrentContext:self.context];
  if( [EAGLContext currentContext] == self.context ) {
    [EAGLContext setCurrentContext:nil];
  }

  [super dealloc];
}

- (void)didReceiveMemoryWarning
{
  // Releases the view if it doesn't have a superview.
  [super didReceiveMemoryWarning];

  if( [self isViewLoaded] && ([[self view] window] == nil) ) {
    self.view = nil;

    if( [EAGLContext currentContext] == self.context ) {
      [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
  }

  // Dispose of any resources that can be recreated.
}

- (void)loadView
{
  [super loadView];

  // Create the view
  EAGL2View* view = [[EAGL2View alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  assert( view != nil && "Unable to create view." );
  
  view.opaque             = TRUE;
  view.contentScaleFactor = [UIScreen mainScreen].scale;
  
  assert( [view.layer isKindOfClass:[CAEAGLLayer class]] && "GLES2RenderSystem: View's Core Animation layer is not a CAEAGLLayer. This is a requirement for using OpenGL ES for drawing." );
    
  CAEAGLLayer *eaglLayer = (CAEAGLLayer *)view.layer;
  assert( eaglLayer != nil && "GLES2RenderSystem: Failed to retrieve a pointer to the view's Core Animation layer" );
  eaglLayer.opaque = TRUE;
  eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE], 
                  kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, 
                  kEAGLDrawablePropertyColorFormat, nil];

  self.view = view;
}

- (void)viewDidLoad
{
  [super viewDidLoad];

  // touches setup
  self.view.userInteractionEnabled = YES;
  self.view.multipleTouchEnabled   = YES;
}

- (void)viewDidUnload
{
  [super viewDidUnload];

  self.touchesDelegate = nil;
}

- (void) setTouchesDelegate: (Nebulae::TouchesDelegate*) delegate
{
  m_touchesDelegate = delegate;
}

-(void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
  m_touchesDelegate->TouchesBegan( touches, event );
}

-(void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
  m_touchesDelegate->TouchesMoved( touches, event );
}

-(void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
  m_touchesDelegate->TouchesEnded( touches, event );
}

-(void) touchesCancelled: (NSSet*) touches withEvent: (UIEvent*) event
{
  m_touchesDelegate->TouchesCancelled( touches, event );
}


@end
