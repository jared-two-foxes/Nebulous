
#include "WidgetEvent.h"


using namespace Nebulae;


WidgetEvent::WidgetEvent( EventType type, const Point& pt, Flags<ModKey> modKeys ) :
  m_type(type),
  m_point(pt),
  m_key(VKC_UNKNOWN),
  m_keyCodePoint(0),
  m_modKeys(modKeys)
{}

WidgetEvent::WidgetEvent( EventType type, const Point& pt, const Point& move, Flags<ModKey> modKeys ) :
  m_type(type),
  m_point(pt),
  m_key(VKC_UNKNOWN),
  m_keyCodePoint(0),
  m_modKeys(modKeys),
  m_dragMove(move)
{}

WidgetEvent::WidgetEvent( EventType type, KeyCode key, uint32 codePoint, Flags<ModKey> modKeys ) :
  m_type(type),
  m_key(key),
  m_keyCodePoint( codePoint ),
  m_modKeys( modKeys )
{}

WidgetEvent::WidgetEvent( EventType type, const Point& pt, const std::map<Widget*, Point>& dragDropWidgets, Flags<ModKey> modKeys ) :
  m_type(type),
  m_point(pt),
  m_key(VKC_UNKNOWN),
  m_keyCodePoint(0),
  m_modKeys(modKeys),
  m_dragDropWidgets(dragDropWidgets)
{}

WidgetEvent::WidgetEvent( EventType type ) :
  m_type(type),
  m_key(VKC_UNKNOWN),
  m_keyCodePoint(0),
  m_modKeys()
{}

WidgetEvent::EventType 
WidgetEvent::GetType() const
{
  return m_type;  
}

const Nebulae::Point& //@note symbol is ambiguous on iOS without qualification
WidgetEvent::GetPoint() const
{
  return m_point; 
}

KeyCode 
WidgetEvent::GetKey() const
{
  return m_key; 
}

uint32
WidgetEvent::GetKeyCodePoint() const
{
  return m_keyCodePoint; 
}

Flags<ModKey> 
WidgetEvent::GetModKeys() const
{
  return m_modKeys; 
}

const Nebulae::Point& //@note symbol is ambiguous on iOS without qualification
WidgetEvent::GetDragMove() const
{ 
  return m_dragMove; 
}


const std::map<Widget*, Nebulae::Point>& //@note symbol is ambiguous on iOS without qualification
WidgetEvent::GetDragDropWidgets() const
{ 
  return m_dragDropWidgets; 
}
