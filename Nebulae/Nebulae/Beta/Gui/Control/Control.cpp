#include "Control.h"

#include <Nebulae/Beta/Gui/WidgetEvent.h>


using namespace Nebulae;


Control::Control( const WidgetFactory& factory, int x, int y, int w, int h, uint32 flags )
  : Widget( factory, x, y, w, h, flags )
  , m_disabled( false )
  , m_colour( CLR_WHITE )
{
}


Control::~Control()
{

}

void 
Control::DropsAcceptable( DropsAcceptableIter first,
                          DropsAcceptableIter last,
                          const Point& pt ) const
{
  if( GetParent() ) {
    GetParent()->DropsAcceptable( first, last, pt );
  }
}


const Colour& 
Control::GetColour() const
{
  return m_colour; 
}


bool 
Control::IsDisabled() const
{
  return m_disabled;
}


void
Control::AcceptDrops( const std::vector<Widget*>& widgets, const Point& pt )
{
  if( GetParent() ) {
    GetParent()->AcceptDrops( widgets, pt );
  }
}

void 
Control::SetColour( const Colour& c )
{ 
  m_colour = c; 
}


void 
Control::SetDisabled( bool disabled )
{
  m_disabled = disabled;
}


// void 
// Control::MouseWheel( const Point& pt, int move/*, Flags<ModKey> mod_keys*/ )
// { 
//   ForwardEventToParent(); 
// }


void
Control::KeyPress( KeyCode key, uint32 keyCodePoint, Flags<ModKey> mod_keys )
{ 
  ForwardEventToParent(); 
}

void 
Control::KeyRelease( KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys )
{ 
  ForwardEventToParent(); 
}