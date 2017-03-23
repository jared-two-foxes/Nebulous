
#include "ScreenController.h"

using namespace Game;

Screen::Screen( const std::string& name, ScreenController *owner )
///
/// Constructor.  Does nothing interesting.
///
  : m_name(name)
  , m_owner(owner)
{
  NE_ASSERT( m_owner != NULL, "Invalid pointer" )();
  NE_ASSERT( m_name.length() > 0, "Invalid argument" )( m_name.length() );
}


Screen::~Screen()
///
/// Destructor.  Does nothing interesting.
///
{}


void
Screen::Init()
///
/// Called when the screen is added to the ScreenController and can be used to initiate
/// any objects and states that should persist across the life of the screen.
///
/// @return
///   Nothing.
///
{}


void
Screen::Enter( const std::vector<std::string>& args )
///
/// Pre execution callback; gets invoked when the screen has been transitioned to.
/// Currently does nothing interesting.
///
/// @return
///   Nothing.
///
{}


void 
Screen::Update( float dt_s ) 
///
/// Main execution loop.  Currently does nothing interesting.
///
/// @return
///   Nothing.
///
{

}


void Screen::Exit()
///
/// Post execution callback; gets invoked when the screen has been transitioned from.
/// Currently does nothing interesting.
///
/// @return
///   Nothing.
///
{
  
}


bool 
Screen::OnKeyPressed( Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys ) 
///
/// Key press hook.
///
/// @return
///   Nothing.
///
{ return false; }


bool 
Screen::OnKeyReleased(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys)
///
/// Key release hook.
///
/// @return
///   Nothing.
///
{ return false; }


bool 
Screen::OnTouchDown(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys)
///
/// Touch down hook.
///
/// @return
///   Nothing.
///
{ return false; }


bool 
Screen::OnTouchUp(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys)
///
/// Touch up hook.
///
/// @return
///   Nothing.
///
{ return false; }


bool 
Screen::OnTouchDrag(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys)
///
/// Touch drag hook.
///
/// @return
///   Nothing.
///
{ return false; }


bool
Screen::Goto( const std::string& action, const std::vector<std::string> &args )
///
/// Attempts to transition from this screen using a specified action name.
///
/// @param action
///   The name of the action to invoke.
///
/// @param args
///   The arguments to pass to the new state.
///
/// @return
///   Whether transition was successful.
///
{
  return m_owner->GotoScreen( action, args );
}


ScreenController::ScreenController()
///
/// Default constructor.
///
  : m_current_screen( 0 )
{}


void 
ScreenController::Clear() 
///
/// Clears all of the current screens and edges returning to an uninitialised state.
///
/// @return
///   Nothing
///   
{
  for( std::vector<Screen* >::iterator it = m_screens.begin(), end = m_screens.end(); it != end; ++it ) {
    delete (*it);
  }
  m_screens.clear();

  m_graph.clear();
}


bool
ScreenController::HasEdge( const std::string& action ) const
///
/// Transitions from the current screen to a new screen based upon the specified action.
///
/// @param action
///   edge identifier.
///
/// @return
///   true  - if edge exists.
///   false - if edge does not exist.
///
{
  const Edge* e = m_graph.FindEdge( m_current_screen, action );
  return ( e != NULL );
}


bool 
ScreenController::GotoScreen( const std::string& action, const std::vector<std::string>& args )
///
/// Transitions from the current screen to a new screen based upon the specified action. 
///
/// @param action
///   edge identifier.
///
/// @param args
///   argument list to pass to the new state.
///
/// @return
///   Whether transition was successful.
///
{
  const Edge* e = m_graph.FindEdge( m_current_screen, action );
  if( e == NULL ) {
    return false;
  }

  Screen* cur_screen = m_screens[m_current_screen];
  m_current_screen = e->b;
  cur_screen->Exit();
  ScreenHandle sh = CurrentScreen();

  std::vector<std::string> composite_args( e->args );
  for( uint32 i = 0; i < args.size(); i++ ) {
    composite_args.push_back( args[i] );
  }
  sh.Enter( composite_args );

  return sh;
}


void
ScreenController::AddScreen( Screen* screen )
///
/// Adds a screen "node" to the graph.
///
/// @param screen
///   The screen.
/// 
/// @return.
///   Nothing.
///
{
  NE_ASSERT( screen, "Null pointer for argument screen encountered" )();

  if( screen && std::find(m_screens.begin(), m_screens.end(), screen) == m_screens.end() ) 
  {
    screen->Init();
    m_screens.push_back( screen );
  }
}


void
ScreenController::AddEdge( const std::string& screen_from, const std::string& screen_to, const std::string& condition, const std::string& arg0 )
///
/// Adds an edge between 2 screen nodes including an action name to act as an identifier for the edge.
///
/// @param screen_from
///   Name of the screen to begin the transition from
///
/// @param screen_to
///   Name of the screen to transition to.
///
/// @param condition
///   Identifier for the edge.
///
/// @return
///   Nothing.
///
{
  int a = GetScreenId( screen_from ), b = GetScreenId( screen_to );
  NE_ASSERT( a >= 0, "Failed to find screen" )();
  NE_ASSERT( b >= 0, "Failed to find screen" )();
  std::vector<std::string> args;
  if( arg0.length() > 0 ) {
    args.push_back( arg0 );
  }
  m_graph.push_back( Edge(a, b, condition, args) );
}


void 
ScreenController::AddEdge(const std::string& screen_from, int b, const std::string& condition, const std::string& arg0 )
///
/// Adds an edge between 2 screen nodes including an action name to act as an identifier for the edge.  Typically used to
/// identify an "exit" terminial node.
///
/// @param screen_from
///   Name of the screen to begin the transition from
///
/// @param b
///   Index of the screen to transition to.
///
/// @param condition
///   Identifier for the edge.
///
/// @return
///   Nothing.
///
{
  int a = GetScreenId( screen_from );
  NE_ASSERT( a >= 0, "Failed to find screen" )();
  std::vector<std::string> args;
  if( arg0.length() > 0 ) {
    args.push_back( arg0 );
  }
  m_graph.push_back( Edge(a, b, condition, args) );
}


ScreenHandle
ScreenController::CurrentScreen( const std::string& name )
///
/// Sets the current screen using the name of the screen to set and returns 
/// a ScreenHandle to the set screen.
///
/// @param name
///   The name of the screen to set.
///
/// @return
///   Handle to the new current screen. 
///
{
  return CurrentScreen(GetScreenId(name));
}


ScreenHandle 
ScreenController::CurrentScreen( int set_current )
///
/// Sets the current screen and returns a ScreenHandle to the set screen.
///
/// @param set_current
///   index of the screen to set.
///
/// @return
///   Handle to the new current screen. 
///
{
  if( m_current_screen >= 0 && m_current_screen < (int)m_screens.size() ) 
  {
    return ScreenHandle( m_screens[m_current_screen] );
  }
  return ScreenHandle( NULL );
}


int 
ScreenController::GetScreenCount() const 
///
/// Returns the number of screens currently registered with the controller.
/// 
/// @return
///   the screen count.
///   
{
  return m_screens.size();
}


int 
ScreenController::GetEdgeCount() const
///
/// Returns the number of edges currently registered with the controller.
/// 
/// @return
///   the edge count.
///   
{
  return m_graph.size();
}


int 
ScreenController::GetCurrentScreenId() const
///
/// Returns the index of current screen.
///
/// @return
///   The screen index.
///
{
  return m_current_screen; 
}


int
ScreenController::GetScreenId( const std::string& name ) const
///
/// Searches for and returns the index of a screen of a given name.
///
/// @param name
///   The name of the screen to search.
///
/// @return
///   The screen index.
///
{
  for( uint32 i = 0; i < m_screens.size(); i++ ) {
    if( m_screens[i]->m_name == name ) {
      return i;
    }
  }
  return -1;
}
