#include <Nebulae/Common/Common.h>

using namespace Nebulae;

///////////////////////////////////////
// ModKeys
///////////////////////////////////////
const ModKey Nebulae::MOD_KEY_NONE    (0x0000);
const ModKey Nebulae::MOD_KEY_LSHIFT  (0x0001);
const ModKey Nebulae::MOD_KEY_RSHIFT  (0x0002);
const ModKey Nebulae::MOD_KEY_LCTRL   (0x0040);
const ModKey Nebulae::MOD_KEY_RCTRL   (0x0080);
const ModKey Nebulae::MOD_KEY_LALT    (0x0100);
const ModKey Nebulae::MOD_KEY_RALT    (0x0200);
const ModKey Nebulae::MOD_KEY_LMETA   (0x0400);
const ModKey Nebulae::MOD_KEY_RMETA   (0x0800);
const ModKey Nebulae::MOD_KEY_NUM     (0x1000);
const ModKey Nebulae::MOD_KEY_CAPS    (0x2000);
const ModKey Nebulae::MOD_KEY_MODE    (0x4000);

namespace {
  bool RegisterModKeys()
  {
    FlagSpec<ModKey>& spec = FlagSpec<ModKey>::instance();
    spec.insert(Nebulae::MOD_KEY_NONE,   "MOD_KEY_NONE",   true);
    spec.insert(Nebulae::MOD_KEY_LSHIFT, "MOD_KEY_LSHIFT", true);
    spec.insert(Nebulae::MOD_KEY_RSHIFT, "MOD_KEY_RSHIFT", true);
    spec.insert(Nebulae::MOD_KEY_LCTRL, "MOD_KEY_LCTRL", true);
    spec.insert(Nebulae::MOD_KEY_RCTRL, "MOD_KEY_RCTRL", true);
    spec.insert(Nebulae::MOD_KEY_LALT, "MOD_KEY_LALT", true);
    spec.insert(Nebulae::MOD_KEY_RALT, "MOD_KEY_RALT", true);
    spec.insert(Nebulae::MOD_KEY_LMETA, "MOD_KEY_LMETA", true);
    spec.insert(Nebulae::MOD_KEY_RMETA, "MOD_KEY_RMETA", true);
    spec.insert(Nebulae::MOD_KEY_NUM, "MOD_KEY_NUM", true);
    spec.insert(Nebulae::MOD_KEY_CAPS, "MOD_KEY_CAPS", true);
    spec.insert(Nebulae::MOD_KEY_MODE, "MOD_KEY_MODE", true);
    return true;
  }
  bool dummy = RegisterModKeys();
}

const Flags<ModKey> Nebulae::MOD_KEY_CTRL  ((Nebulae::MOD_KEY_LCTRL | Nebulae::MOD_KEY_RCTRL));
const Flags<ModKey> Nebulae::MOD_KEY_SHIFT((Nebulae::MOD_KEY_LSHIFT | Nebulae::MOD_KEY_RSHIFT));
const Flags<ModKey> Nebulae::MOD_KEY_ALT((Nebulae::MOD_KEY_LALT | Nebulae::MOD_KEY_RALT));
const Flags<ModKey> Nebulae::MOD_KEY_META((Nebulae::MOD_KEY_LMETA | Nebulae::MOD_KEY_RMETA));

NE_FLAGSPEC_IMPL(ModKey);


//constructor
Window::Window()
: m_caption( "" ),
  m_position( Point(0,0) ),
  m_size( Point(-1,-1) )
{ }

//destructor
Window::~Window()
{ }

int32 
Window::GetWidth() const
{ return m_size.x; }

int32
Window::GetHeight() const
{ return m_size.y; }

const Point 
Window::GetSize() const
{ return m_size; }

const Point 
Window::GetPosition() const
{ return m_position; }

const char* 
Window::GetCaption() const
{ return m_caption.c_str(); }

void 
Window::MoveAndResize( int x, int y, int w, int h )
{
	m_position.x  = x;
	m_position.y  = y;
	m_size.x = w;
	m_size.y = h;
}

void 
Window::SetCaption( const char* szCaption )
{
	m_caption = szCaption;
}

bool 
Window::IsPointInWindow( const Point& point ) const
{
  Point pos( point );
  pos -= m_position;
  
  return (pos.x < m_size.x && 
          pos.y < m_size.y);
}

void 
Window::AddInputListener( InputListener* pInputListener )
{ m_listeners.push_back( pInputListener ); }

void 
Window::RemoveInputListener( InputListener* pInputListener )
{ m_listeners.remove( pInputListener ); }

bool
Window::HasInputListener( InputListener* inputListener )
{
	for( std::list<InputListener*>::iterator it = m_listeners.begin(), end = m_listeners.end(); 
       it != end; ++it ) 
  {
		if( (*it) == inputListener ) 
    {
      return true;
    }
	}

  return false;
}

void 
Window::ButtonPressed( int32 x, int32 y, MouseButton button, Flags<ModKey> modKeys )
{
	std::list<InputListener*>::iterator end_it = m_listeners.end();
	for( std::list<InputListener*>::iterator it = m_listeners.begin(); it != end_it; ++it ) 
  {
    (*it)->MousePressed(button, x, y, modKeys);
	}
}

void 
Window::ButtonReleased( int32 x, int32 y, MouseButton button, Flags<ModKey> modKeys )
{
	std::list<InputListener*>::iterator end_it = m_listeners.end();
	for( std::list<InputListener*>::iterator it = m_listeners.begin(); it != end_it; ++it ) 
  {
    (*it)->MouseReleased( button, x, y, modKeys);
	}
}

void 
Window::ButtonDragged( int32 x, int32 y, MouseButton button, Flags<ModKey> modKeys )
{
  std::list<InputListener*>::iterator end_it = m_listeners.end();
	for( std::list<InputListener*>::iterator it = m_listeners.begin(); it != end_it; ++it ) 
  {
		(*it)->MouseMove( button, x, y, modKeys );
	}
}

void 
Window::KeyPressed( KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys )
{
	std::list<InputListener*>::iterator end_it = m_listeners.end();
	for( std::list<InputListener*>::iterator it = m_listeners.begin(); it != end_it; ++it )
  {
    (*it)->KeyPressed( keyCode, key_code_point, modKeys );
	}
}

void 
Window::KeyReleased( KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys )
{
	std::list<InputListener*>::iterator end_it = m_listeners.end();
	for( std::list<InputListener*>::iterator it = m_listeners.begin(); it != end_it; ++it ) 
  {
		(*it)->KeyReleased( keyCode, key_code_point, modKeys );
	}
}