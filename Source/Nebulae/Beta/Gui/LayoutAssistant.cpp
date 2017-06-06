
#include "LayoutAssistant.h"

#include "Widget.h"

using namespace Nebulae;

Rect GetBounds( Widget* widget )
{
  return Rect( widget->GetLowerRight(), widget->GetUpperLeft() );
}

LayoutAssistant::LayoutAssistant( int w, int h )
: m_defaultCreationWidth( w ),
  m_defaultCreationHeight( h ),
  m_resolution( Point(640,960) )
{}

void
LayoutAssistant::SetResolution( const Point& resolution )
{  
  m_resolution = resolution;
}

bool
LayoutAssistant::Process( Widget* widget )
{
  NE_ASSERT(widget,"")();
  if( widget == NULL ) return false;
  
  float deviceScale  = 1.0f;
  if( m_defaultCreationHeight > m_defaultCreationWidth )
  {
    deviceScale = (Real)m_resolution.x / m_defaultCreationWidth;
  }
  else
  {
    deviceScale = (Real)m_resolution.y / m_defaultCreationHeight;
  }

  Scale( widget, deviceScale, deviceScale );

  Rect old_bounds;
  old_bounds.ul.x = 0.f;
  old_bounds.ul.y = 0.f;
  old_bounds.lr.x = m_defaultCreationWidth;
  old_bounds.lr.y = m_defaultCreationHeight;

  Rect new_bounds;
  new_bounds.ul.x = 0.f;
  new_bounds.ul.y = 0.f;           
  new_bounds.lr.x = m_resolution.x;
  new_bounds.lr.y = m_resolution.y;

  Adjust( widget, old_bounds, new_bounds );

  return true;
}

Real 
LayoutAssistant::GetAspectRatio() const
{
  return (((Real)m_resolution.x) / m_resolution.y);
}

void
LayoutAssistant::Adjust( Widget* widget, const Rect& old_bounds, const Rect& new_bounds )
{
  NE_ASSERT(widget,"")();

  Rect                my_old_bounds = GetBounds( widget );
  Real x_pos, y_pos;

//
// Positioning
//
  Point current_position = widget->GetRelativeUpperLeft();
  Real  old_width        = old_bounds.GetWidth();
  Real  old_height       = old_bounds.GetHeight();
  Real  x_ratio          = ( ( current_position.x - old_bounds.ul.x ) / ( old_width  ) );
  Real  y_ratio          = ( ( current_position.y - old_bounds.ul.y ) / ( old_height ) );
  Real  new_width        = new_bounds.GetWidth();
  Real  new_height       = new_bounds.GetHeight();
  x_pos   = ( x_ratio * new_width )  + new_bounds.ul.x;
  y_pos   = ( y_ratio * new_height ) + new_bounds.ul.y;
    
// 
// Scaling
//
  Real scale = 1.0f; //getLocalVariable("Scale_for_larger_aspect_ratio");
  
  static const double ratio_4_3  = 4.0 / 3.0;
  static const double ratio_16_9 = 16.0 / 9.0;
  static const double run_value  = ratio_16_9 - ratio_4_3;

  double rise_value = scale - 1.0f;
  double gradient   = ( rise_value ) / ( run_value );
  Real   new_scale  = ( gradient * ( GetAspectRatio() - ratio_4_3 ) ) + 1.0f;
  Real   width      = widget->GetWidth() * new_scale;
  Real   height     = widget->GetHeight() * new_scale;

  Point upperLeft(x_pos, y_pos);
  Point lowerRight( x_pos + width, y_pos + height );
  widget->SizeMove( upperLeft, lowerRight );
}

void 
LayoutAssistant::Scale( Widget* widget, Real x_scale, Real y_scale )
{
  // Get the objects initial dimensions.
  Point position = widget->GetRelativeUpperLeft();
  Real  width    = widget->GetWidth() * x_scale;
  Real  height   = widget->GetHeight() * y_scale; 
  
  widget->SizeMove( position, position + Point(width,height) );

  // Iterate all the child widgets and SizeMove them appropriately.
  std::list<Widget* > children = widget->GetChildren();
  std::for_each( children.begin(), children.end(), 
    [&](Widget* child){ 
      // Get the objects initial dimensions.
      Point relative_position = child->GetRelativeUpperLeft();
      Real  original_width    = child->GetWidth();
      Real  original_height   = child->GetHeight(); 

      // Apply the scale to the position & dimensions.
      Point scaled_position( relative_position.x * x_scale, relative_position.y * y_scale );
      Real width  = x_scale * original_width;
      Real height = y_scale * original_height;

      // Set the scaled width and height to the object.
      child->SizeMove( scaled_position, scaled_position + Point(width,height) );
    } );
}
