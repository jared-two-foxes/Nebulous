#include "State.h"

namespace Nebulae
{

  void
  State::Enter( StateStack* caller )
  {

  }

  void
  State::Pause()
  {

  }

  void
  State::Resume()
  {

  }

  void
  State::Exit( StateStack* caller )
  {

  }

  bool 
  State::IsState( const char* szStateName ) const 
  {
    return (strcmp(m_strName.c_str(), szStateName) == 0);
  }
  
}