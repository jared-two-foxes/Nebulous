#ifndef __PARTICLEPOOL_H__
#define __PARTICLEPOOL_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  
struct Particle;

///
/// An object representing a pool of Particle objects. 
///
class ParticlePool
{  
private:
  int                  m_capacity;
  Particle*            m_pParticles;
  std::queue< size_t > m_UnusedIndices;

  public:
    ParticlePool( uint32 capacity );
    ~ParticlePool();

    Particle* fetch();
    void replace( Particle* pParticle );

}; //ParticlePool

}

#endif // __PARTICLEPOOL_H__