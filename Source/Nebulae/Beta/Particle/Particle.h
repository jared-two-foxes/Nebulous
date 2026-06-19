#ifndef NEBULAE_BETA_PARTICLE_PARTICLE_H_
#define NEBULAE_BETA_PARTICLE_PARTICLE_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

struct Particle
{
public:
  uint32 m_index;
  Vector4 m_position;
  Vector4 m_velocity;
  Vector4 m_scale;
  float m_age;
  float m_life;
};

} // namespace Nebulae

#endif // NEBULAE_BETA_PARTICLE_PARTICLE_H_