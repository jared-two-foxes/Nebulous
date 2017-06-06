#ifndef __NEBULAE_STATE_H__
#define __NEBULAE_STATE_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{
  // Forward Decelerations.
  class StateStack;

  /** Defines an interface that encapsulates the behaviour a specific State in a FSM.
   *  It is used by the StateStack class. 
   */
  class State
  {
  private:
    std::string m_strName; ///< Unique identifier.

  public:
    /** \name Structors */ ///@{
    State( const std::string& name ) : m_strName(name) {}
    virtual ~State() {}
    //@}

    /** \name Mutators */ ///@{
    virtual void Enter( StateStack* caller );
    virtual void Exit( StateStack* caller );
    virtual void Pause();
    virtual void Resume();
    virtual void Update( float fDeltaTimeStep, StateStack* pCaller ) = 0;
    virtual void Render() const = 0;
    //@}

    /** \name Accessors */ ///@{
    bool IsState( const char* szStateName ) const;
    //@}

  }; //State

} //Nebulae

#endif // __NEBULAE_STATE_H__