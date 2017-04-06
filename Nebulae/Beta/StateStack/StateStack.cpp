
#include "State.h"
#include "StateStack.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>


using namespace Nebulae;


StateStack::StateStack()
///
/// Default constructor
/// 
{
}


StateStack::~StateStack()
///
/// Destructor
/// 
{
  Clear();
}


void 
StateStack::Clear()
///
/// Clears all internal resources.
///
/// @return 
///   Nothing.
///  
{
  while( !m_States.empty() ) {
    (m_States.back())->Exit( this );
    delete m_States.back();
    m_States.pop_back();
  }
}


void 
StateStack::PushState( State* pNewState )
///
/// Pushes a new state ontop of the stack and invokes the Enter function on it.  
/// If there already exists a state on the stack it calls Pause on that State.
///
/// @return
///   Nothing.
///  
{
  if( !m_States.empty() ) {
    m_States.back()->Pause();
	}

  m_States.push_back( pNewState );
  pNewState->Enter( this );
}


void 
StateStack::PopState()
///
/// If there is a state ontop of the stack will call Exit on that State and removes
/// it from the stack.
/// If there exists a state under it on the stack it will call Resume on that state
///
/// @return
///   Nothing
///
{
  NE_ASSERT( !m_States.empty(), "Tried to pop a state when there was no state ontop of the stack" )();

  m_States.back()->Exit( this );
  delete m_States.back();
  m_States.pop_back();

  if( !m_States.empty() ) {    
    m_States.back()->Resume();
  }
}


void 
StateStack::ProcessSimulation( uint64 iTimeElapsed )
{
  // Basically need to do a conversion between microseconds and seconds as
  // the subsystems are expecting time in seconds.  Should possibly change all
  // subsystems to also expect microseconds.
  float elapsedInSeconds = iTimeElapsed / 1000000.0f;

  Update(elapsedInSeconds);
}


void 
StateStack::ProcessFrame( uint64 iTimeElapsed )
///
/// Called each frame and contains the main application logic for the application.
/// Will update whatever the current application state is and then render the results.
///
/// @param fTimeElapsed  
///   The time since the function was last called.
///
/// @return
///   Nothing.
/// 
{
  Render();
}


void
StateStack::Update( float fTimeElapsed )
///
/// Will call Update on the State ontop of the Stack if one exists.
///
/// @param fTimeElapsed
///   The time since this function was last called.
/// @return
///   Nothing
///
{
  if( !m_States.empty() ) {
    m_States.back()->Update( fTimeElapsed, this );
  }
}


void 
StateStack::Render() const
///
/// Will call Render on the State ontop of the Stack if one exists.
///
/// @return
///   Nothing
///
{
  if( !m_States.empty() ) {
    m_States.back()->Render();
  }
}


State*
StateStack::GetCurrentState()
///
/// Returns the State ontop of the stack.
///
/// @return
///   The state ontop of the stack
/// 
{
  if( m_States.empty() ) {
		return NULL;
	}

  return m_States.back();
}


State* 
StateStack::FindState( const char* szStateName ) const
///
/// Will attempt to return a State of a given name
///
/// @param szStateName
///   The name of the State that we wish to find.
/// @return
///   The State if it was found, otherwise NULL
///  
{
  if( m_States.empty() ) {
    return NULL;
	}

  std::list<State*>::const_iterator end_it = m_States.end();
  for( std::list<State*>::const_iterator it = m_States.begin(); it != end_it; ++it) {
    if( (*it)->IsState( szStateName ) ) {
      return (*it);
    }
  }

  return NULL;
}
