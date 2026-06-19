#ifndef NEBULAE_COMMON_PLATFORM_CLOCK_H_
#define NEBULAE_COMMON_PLATFORM_CLOCK_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

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

} // namespace Nebulae

#endif // NEBULAE_COMMON_PLATFORM_CLOCK_H_