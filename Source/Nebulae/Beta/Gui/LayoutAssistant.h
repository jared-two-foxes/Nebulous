#ifndef NEBULAE_WIDGET_LAYOUTASSISTANT_H__
#define NEBULAE_WIDGET_LAYOUTASSISTANT_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Widget;

class LayoutAssistant
///
/// The LayoutAssistant class is responsible for determining the position & size
/// for a Widget object based upon the default environment size and the actual 
/// current environment size.
///
{
public:
  typedef std::weak_ptr<Widget > WidgetPtr;

private:
  int32 m_defaultCreationWidth;  ///< default widget definition width.
  int32 m_defaultCreationHeight; ///< default widget definition height.
  Point m_resolution;            ///< current environment size.

  public:
    LayoutAssistant( int w = 640, int h = 960 );

    /**
     *  Sets the current environment size.
     *  
     *  @param resolution
     *    The environment size.
     *    
     *  @return
     *    Nothing.
     */
    void SetResolution( const Point& resolution );

    /**
     *  Scales and positions a Widget based upon its current position
     *  and size (assuming a default creation environment) and determines
     *  what its size and position should be for the current user interface
     *  environment size.
     *  
     *  @param width
     *    The widget to process
     *    
     *  @return
     *    Nothing.
     */
    bool Process( Widget* widget );

  private:
    /**
     *  Returns a ratio of the width vs height of the current user interface 
     *  environment.
     *  
     *  @return
     *    The ratio.
     */
    Real GetAspectRatio() const;

    /**
     *  Positions the Widget so that it's new position relative to the new_bounds
     *  is the same as the old position relative to it's old bounds.
     *  
     *  @param widget
     *    Widget to position
     *    
     *  @param old_bounds
     *    The old bounds that the widget existed within.
     *    
     *  @param new_bounds
     *    The new bounds that the widget will exist within.
     *  
     *  @return
     *    Nothing.
     */
    void Adjust( Widget* widget, const Rect &old_bounds, const Rect &new_bounds );

    /**
     *  Scales the Widget & all its child widgets non-uniformly.  The position of 
     *  the Widget is preserved but all the positions child widgets will be scaled 
     *  to preserve relative positions within the new widget bounds.
     *  
     *  @param widget
     *    Widget to scale
     *    
     *  @param x_scale
     *    The scale factor for the x-axis
     *    
     *  @param y_scale
     *    The scale factor for the y-axis
     *  
     *  @return
     *    Nothing.
     */
    void Scale( Widget* widget, Real x_scale, Real y_scale );

};

}
#endif // NEBULAE_WIDGET_LAYOUTASSISTANT_H__