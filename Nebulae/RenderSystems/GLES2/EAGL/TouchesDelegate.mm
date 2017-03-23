
#include "TouchesDelegate.h"

#include <Nebulae/Common/Window/Window.h>

#define TOUCH_THRESHOLD 20

using namespace Nebulae;

TouchesDelegate::TouchesDelegate( Window* window, float scalefactor )
  : m_window( window ),
    m_scaleFactor( scalefactor ),
    m_touchCount( 0 )
    //m_lastSingleTouch()
{
}


void
TouchesDelegate::Clear()
{
  for( int i = 0; i < MAX_TOUCHES; ++i ) {
    if( m_touchPoints[i] ) {
      [m_touchPoints[i] release];
      m_touchPoints[i] = NULL;
    }
  }
  m_touchCount  = 0;
}


void 
TouchesDelegate::ProcessTouchEnd( UITouch* touch )
{
  CGPoint currentPoint  = [touch locationInView:[touch view]];
  CGPoint previousPoint = [touch previousLocationInView:[touch view]];
  float   diffx         = currentPoint.x - m_lastSingleTouch.x;
  float   diffy         = currentPoint.y - m_lastSingleTouch.y;
  
  if( fabs(diffx) + fabs(diffy) <= TOUCH_THRESHOLD ) { // Manhatten distance for speed
    currentPoint = m_lastSingleTouch;
  }

  CGPoint scenePoint;
  scenePoint.x = currentPoint.x * m_scaleFactor;
  scenePoint.y = currentPoint.y * m_scaleFactor;
  
  if( !CGPointEqualToPoint( currentPoint, previousPoint ) ) {
    m_window->SetCursorPosition( scenePoint.x, scenePoint.y );
  }
  
  m_window->ButtonReleased( Button1 );
}


void
TouchesDelegate::TouchesBegan( NSSet* touches, UIEvent* event )
{
  //WipeLostTouches();
  
  if( m_touchCount == 1 && [touches count] > 0 ) {
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      if( m_touchPoints[i] ) {
        ProcessTouchEnd( m_touchPoints[i] );
      }
    }
  }
  
  NSEnumerator* itr = [touches objectEnumerator];
  while( UITouch* touch = [itr nextObject] ) {
    bool foundFreeSlot = false;
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      if( !m_touchPoints[i] ) {
        m_touchPoints[i] = [touch retain]; 
        ++m_touchCount;
        foundFreeSlot = true;
        break;
      }
    }
  }
  
  if( m_touchCount == 1 && [touches count] == 1 ) {
    UITouch* touch = [touches anyObject];
    CGPoint  point = [touch locationInView: [touch view]];
    m_lastSingleTouch = point;
    m_window->SetCursorPosition( point.x * m_scaleFactor, point.y * m_scaleFactor );
    m_window->ButtonPressed( Button1 );
  }
}


void
TouchesDelegate::TouchesMoved( NSSet* touches, UIEvent* event )
{
  //WipeLostTouches();
  
  int foundTouches = 0;
  NSEnumerator* itr = [touches objectEnumerator];
  while( UITouch* touch = [itr nextObject] ) {
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      if( m_touchPoints[i] == touch ) {
        ++foundTouches;
        break;
      }
    }
  }

  if( foundTouches != [touches count] ) {
    Clear();
    return;
  }

  
  if( m_touchCount == 1 ) { // pan
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      if( m_touchPoints[i] ) {
        UITouch* touch = m_touchPoints[i];
        CGPoint  point = [touch locationInView: [touch view]];

        m_window->SetCursorPosition( point.x * m_scaleFactor, point.y * m_scaleFactor );
        break;
      }
    }
  } else if( m_touchCount == 2 ) { // zoom
    UITouch* touch1 = NULL;
    UITouch* touch2 = NULL;
    
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      if( m_touchPoints[i] ) {
        if( !touch1 ) {
          touch1 = m_touchPoints[i];
        } else if( !touch2 ) {
          touch2 = m_touchPoints[i];
          break;
        }
      }
    }
    
    if( !touch1 || !touch2 ) {
      Clear();
      return;
    }
    
    CGPoint point1     = [touch1 locationInView: [touch1 view]];
    CGPoint prevPoint1 = [touch1 previousLocationInView: [touch1 view]];
    CGPoint point2     = [touch2 locationInView: [touch2 view]];
    CGPoint prevPoint2 = [touch2 previousLocationInView: [touch2 view]];
    
    float diffx = point1.x - point2.x;
    float diffy = point1.y - point2.y;
    float dist  = sqrt(diffx*diffx + diffy*diffy) * m_scaleFactor;
    
    float olddiffx = prevPoint1.x - prevPoint2.x;
    float olddiffy = prevPoint1.y - prevPoint2.y;
    float olddist  = sqrt(olddiffx*olddiffx + olddiffy*olddiffy) * m_scaleFactor;
    
    CGPoint midpoint;
    midpoint.x = (point1.x + point2.x) / 2;
    midpoint.y = (point1.y + point2.y) / 2;
    
    //m_window->InjectPinch(midpoint.x * m_scaleFactor, midpoint.y * m_scaleFactor, dist, olddist);
  }
}


void
TouchesDelegate::TouchesEnded( NSSet* touches, UIEvent* event )
{
  //WipeLostTouches();
  
  int foundCount = 0;
  NSEnumerator* itr = [touches objectEnumerator];
  while( UITouch* touch = [itr nextObject] ) {
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      if( m_touchPoints[i] == touch ) {
        [m_touchPoints[i] release];
        m_touchPoints[i] = NULL;
        --m_touchCount;
        ++foundCount;
      }
    }
  }
  
  if( foundCount != [touches count] || m_touchCount < 0 ) {
    Clear();
    return;
  } else if( m_touchCount == 0 ) {
    if( [touches count] == 1 ) {
      ProcessTouchEnd( [touches anyObject] );
    } else {
      //m_window->InjectPinchCancel();
    }
  } else if( m_touchCount == 1 ) { //still one touch ? 
    for( int i = 0; i < MAX_TOUCHES; ++i ) {
      UITouch* touch = m_touchPoints[i];
      if( touch )
      {
        CGPoint point = [touch locationInView: [touch view]];
        m_window->SetCursorPosition( point.x * m_scaleFactor, point.y * m_scaleFactor );
        m_window->ButtonPressed( Button1 );
        break;
      }
    }
  }
}


void
TouchesDelegate::TouchesCancelled( NSSet* touches, UIEvent* event )
{
  TouchesEnded( touches, event );
}

