#ifndef NEBULAE_BETA_PARTICLE_PARTICLEPOOL_H_
#define NEBULAE_BETA_PARTICLE_PARTICLEPOOL_H_

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
  int m_capacity;
  Particle* m_pParticles;
  std::vector<size_t> m_freeIndices;

public:
  ParticlePool( uint32 capacity );
  ~ParticlePool();

  Particle* fetch();
  void replace( Particle* pParticle );

}; // ParticlePool

} // namespace Nebulae

#endif // NEBULAE_BETA_PARTICLE_PARTICLEPOOL_H_
