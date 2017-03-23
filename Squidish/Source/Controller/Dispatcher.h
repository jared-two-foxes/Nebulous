#ifndef CONTROLLER_DISPATCHER_H__
#define CONTROLLER_DISPATCHER_H__

#include <Nebulae/Nebulae.h>

namespace Game {

struct Event 
///
/// An event is a simple structure that is used to store information for later proccessing.
///
/// @note
///   Currently the information is stored in a series of strings in order to make the storage
///   of events as generic as possible as we do not want to assume anything about the type of
///   Event that we are storing.
///
{
  std::string type;
  std::string context;
};

class Dispatcher
///
/// A dispatcher simply catches events for processing at a later time.
///
{
public:
  struct EventResolver 
  {
    /** Accepts an event argument and attempts to resolve it.  If the function deems that the 
        event has been successfully resolved it should return true so that the event is removed
        from the queue. */
    virtual bool ResolveEvent( Event& event ) = 0;
  };

private:
  std::stack<Event > m_events; ///< List of events.

  public:
    Dispatcher();

    /** Adds an event to the event queue. */
    void AddEvent( Event event );

    /** Using a passed EventResolver each Event in the list will be processed and removed if the
        EventResolver was successful. */
    void DispatchQueue( EventResolver& resolver );

};

} // Game

#endif // CONTROLLER_DISPATCHER_H__