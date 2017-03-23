
#include "Dispatcher.h"


using namespace Nebulae;
using namespace Game;

Dispatcher::Dispatcher()
{}

void 
Dispatcher::AddEvent( Event event )
{
  m_events.push( event );
}

void 
Dispatcher::DispatchQueue( EventResolver& resolver )
{
  // Attempt to process the top event of the stack, if successful then remove the event and attempt to resolve the 
  // next event in the stack otherwise if it fails exit out.
  while( !m_events.empty() ) 
  {
    if( resolver.ResolveEvent(m_events.top()) )
    {
      m_events.pop();
    }
    else
    {
      return;
    }
  }
}