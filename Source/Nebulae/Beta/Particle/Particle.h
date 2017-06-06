#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

struct Particle
{
public:
  uint32    m_index;
  Vector4   m_position;
  Vector4   m_velocity;
  Vector4   m_scale;
  float     m_age;
  float     m_life;

}; 

} //Nebulae

#endif // __PARTICLE_H__