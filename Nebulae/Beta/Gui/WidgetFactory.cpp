
#include "WidgetFactory.h"

#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Gui/Control/EditControl.h>
#include <Nebulae/Beta/Gui/Control/ListBox.h>
#include <Nebulae/Beta/Gui/Control/Slider.h>
#include <Nebulae/Beta/Gui/Control/StaticGraphic.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>


using namespace Nebulae;


WidgetFactory::WidgetFactory()
{
}


WidgetFactory::~WidgetFactory()
{
}


ButtonControl* 
WidgetFactory::CreateButton( int x, int y, int width, int height, const std::string& str, const std::shared_ptr<Font>& font,
                             Colour colour, Colour text_colour, uint32 flags ) const
{
  return new ButtonControl( *this, x, y, width, height, str, font, colour, text_colour, flags );
}


StaticGraphic* 
WidgetFactory::CreateStaticGraphic( int x, int y, int width, int height, SubTexture* texture, uint32 flags ) const
{
  return new StaticGraphic( *this, x, y, width, height, texture, flags );
}

EditControl*    
WidgetFactory::CreateEditControl( int32 x, int32 y, int32 w, const std::string& str, const std::shared_ptr<Font >& font,
                                  Colour colour, Colour text_colour, Colour interior, uint32 flags ) const
{
  return new EditControl( *this, x, y, w, str, font, colour, text_colour, interior, flags );
}

ListBox*           
WidgetFactory::CreateListBox( int x, int y, int w, int h, Colour color, Colour interior, uint32 flags ) const
{
  return new ListBox( *this, x, y, w, h, color, interior, flags );
}


Slider* 
WidgetFactory::CreateSlider( int x, int y, int w, int h, Real min, Real max/*, Orientation orientation,
                             SliderLineStyle style, Clr color*/, int tabWidth, int lineWidth,
                             uint32 flags ) const
{ 
  return new Slider( *this, x, y, w, h, min, max/*, orientation, style, color*/, tabWidth, lineWidth, flags ); 
}


TextControl*
WidgetFactory::CreateTextControl( int x, int y, int w, int h, const std::string& str,
                                  const std::shared_ptr<Font>& font, Colour color,
                                  Flags<TextFormat> format, uint32 flags ) const
{
  return new TextControl( *this, x, y, w, h, str, font, color, format, flags ); 
}


TextControl*       
WidgetFactory::CreateTextControl( int x, int y, const std::string& str, const std::shared_ptr<Font>& font,
                                  Colour color, Flags<TextFormat> format, uint32 flags ) const
{
  return new TextControl( *this, x, y, str, font, color, format, flags );
}


ButtonControl* 
WidgetFactory::CreateSliderTabButton( int x, int y, int w, int h, const std::string& str,
                                      const std::shared_ptr<Font>& font, Colour colour, Colour text_colour,
                                      uint32 flags ) const
{ 
  return CreateButton( x, y, w, h, str, font, colour, text_colour, flags ); 
}