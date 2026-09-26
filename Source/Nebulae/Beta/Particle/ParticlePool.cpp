
#include "ParticlePool.h"
#include "Particle.h"


using namespace Nebulae;


ParticlePool::ParticlePool( uint32 capacity )
{
  NE_ASSERT( capacity > 0, "Invalid capacity" );

  m_capacity = capacity;
  m_pParticles = new Particle[capacity];
  m_freeIndices.reserve( capacity );

  // All indices start as unused. Fill in reverse so index zero is fetched first.
  for ( size_t i = capacity; i > 0; --i )
  {
    const size_t index = i - 1;
    m_pParticles[index].m_index = static_cast<uint32>( index );
    m_freeIndices.push_back( index );
  }
}

ParticlePool::~ParticlePool()
{
  if ( m_pParticles )
  {
    delete[] m_pParticles;
  }
}

Particle* ParticlePool::fetch()
{
  // Grab the next unused index
  if ( m_freeIndices.empty() )
  {
    return nullptr;
  }

  size_t idx = m_freeIndices.back();
  m_freeIndices.pop_back();
  // Return particle at that index.
  return &( m_pParticles[idx] );
}

void ParticlePool::replace( Particle* pParticle )
{
  // Clear the particles state.
  pParticle->m_position = Vector4( 0, 0, 0, 0 );
  pParticle->m_velocity = Vector4( 0, 0, 0, 0 );
  pParticle->m_scale = Vector4( 1.0f, 1.0f, 1.0f, 0 );
  pParticle->m_life = 0.0f;

  // Store particle index so we know that we can use it again.
  m_freeIndices.push_back( pParticle->m_index );
}
