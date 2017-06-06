#include "ButtonControl.h"

#include <Nebulae/Common/Window/Window.h>

#include <Nebulae/Beta/Gui/WidgetEvent.h>

using namespace Nebulae;


ButtonControl::ButtonControl( const WidgetFactory& factory, int x, int y, int w, int h, const std::string& str, 
    const std::shared_ptr<Font>& font, Colour color, Colour text_color,
    uint32 flags )
///
/// Constructor for a simple button control which fires a callback when a click is detected.
///
: TextControl( factory, x, y, w, h, str, font, text_color, FORMAT_NONE, flags ),
  m_buttonState( BN_UNPRESSED ),
  m_unpressedGraphic( NULL ),
  m_pressedGraphic( NULL )
{
  m_colour = color;
}

ButtonControl::~ButtonControl()
///
/// Destructor.
///
{}

ButtonControl::ButtonState
ButtonControl::GetState() const
{
  return m_buttonState;
}

const SubTexture* 
ButtonControl::GetUnpressedGraphic() const
{
  return m_unpressedGraphic;
}

const SubTexture* 
ButtonControl::GetPressedGraphic() const
{
  return m_pressedGraphic;
}

WidgetType 
ButtonControl::GetWidgetType() const 
{ 
  return BUTTON; 
}

void
ButtonControl::SetState( ButtonState state )
{
  m_buttonState = state;
}

void
ButtonControl::SetUnpressedGraphic( const SubTexture& texture )
{
  m_unpressedGraphic = &texture;
}

void
ButtonControl::SetPressedGraphic( const SubTexture& texture )
{
  m_pressedGraphic = &texture;
}

void 
ButtonControl::TouchDown( const Point& pt, Flags<ModKey> mod_keys )
///
/// A touch down was detected within the bounds of the Control.
///
/// @param pt
///   The position of the touch.
///
/// @return
///   Nothing.
///
{
  if( !IsDisabled() )
  {
    ButtonState prev_state = m_buttonState;
    SetState( BN_PRESSED );
    if( prev_state == BN_PRESSED && RepeatButtonDown() ) 
    {
      ClickedSignal();
    }
  }
}

void 
ButtonControl::TouchDrag( const Point& pt, Flags<ModKey> mod_keys )
///
/// A touch was moved while contained within the bounds of the Control.
///
/// @param pt
///   The position that the touch is. 
///
/// @return
///   Nothing.
///
{
  if( !IsDisabled() ) 
  {
    SetState( BN_PRESSED );
  }
}

void
ButtonControl::TouchUp( const Point& pt, Flags<ModKey> mod_keys )
///
/// A touch was released over this control.  If the touch began over the
/// control then trigger the onClicked callback.
///
/// @param pt
///   The position that the touch was released.
///
/// @return
///   Nothing.
///
{
  if( !IsDisabled() ) 
  {
    SetState( BN_UNPRESSED );
  }
}

void
ButtonControl::Clicked( const Point& pt, Flags<ModKey> mod_keys )
///
/// A touch was released over this control.  If the touch began over the
/// control then trigger the onClicked callback.
///
/// @param pt
///   The position that the touch was released.
///
/// @return
///   Nothing.
///
{
  if( !IsDisabled() ) 
  {
    SetState( BN_UNPRESSED );
    ClickedSignal();
  }
}