#ifndef __NEBULAE_STATESTACK_H__
#define __NEBULAE_STATESTACK_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Application/Application.h>

namespace Nebulae
{

	class State;
	
	class StateStack : public Application
  ///
  /// An implementation of a modified FiniteStateMachine pattern in which it is possible to
  /// push and pop states onto a stack.  Useful for when we wish to be able to enter a new 
  /// state while preserving the status of the current state and return to it once we are 
  /// finished with the new state.
  ///
	{
  private:
    std::list< State* > m_States; ///< List of state objects currently associated with the Stack.  'Top' State is active.

  	public:
      /** \name Structors */ ///@{
  		StateStack();
  		virtual ~StateStack();
      //@}

      /** \name Accessors */ ///@{
  		State* GetCurrentState();
  		State* FindState( const char* stateName ) const;
      //@}

      /** \name Mutators */ ///@{
  		void Clear();
      void PushState( State* newstate );
      void PopState();
      
      virtual void ProcessFrame( float fTimeElapsed );
      //@}

    private:

      /** \name Mutators */ ///@{
      void Update( float fTimeElapsed );
      void Render() const;
      //@}

	}; //StateStack

} //Nebulae

#endif // __NEBULAE_APPLICATION_H__