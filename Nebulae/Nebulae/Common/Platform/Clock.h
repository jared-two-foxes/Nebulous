#ifndef NEBULAE_BETA_CLOCK_H__
#define NEBULAE_BETA_CLOCK_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {
  
class Clock 
///
/// The clock object is responsible for returning the various times that the applicaiton
/// will rely on for accurate simulation of the game state and simulation.
///
{
private:
  struct ClockImpl;

private:
  ClockImpl* m_impl;

  public:
    Clock();
    virtual ~Clock();

    virtual void Init();

    virtual void MarkThisTick();
      
    virtual uint64 GetAccurateTime() const;
    virtual uint64 GetElapsedTime() const;
    virtual uint64 GetUpTime() const;

};

}

#endif // NEBULAE_BETA_CLOCK_H__