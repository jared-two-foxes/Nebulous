
#import "EAGL2View.h"

#include <Nebulae/Common/Common.h>
#import  <QuartzCore/QuartzCore.h>

using namespace Nebulae;

@implementation EAGL2View

+ (Class)layerClass
{
  return [CAEAGLLayer class];
}


//-(void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
//{
//  int breakpoint = 12;
//}
//-(void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
//{
//  int breakpoint = 12;
//}
//-(void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
//{
//  int breakpoint = 12;
//}
//-(void) touchesCancelled: (NSSet*) touches withEvent: (UIEvent*) event
//{
//  int breakpoint = 12;
//}

@end
