
#include "Particle.h"
#include "ParticlePool.h"


using namespace Nebulae;


ParticlePool::ParticlePool( uint32 capacity )
{
  NE_ASSERT( capacity > 0, "Invalid capacity" )();

	m_capacity = capacity;
	m_pParticles = new Particle[ capacity ];

	// All indices start as unused.
	for ( size_t i = 0; i < capacity; ++i )
	{
		m_pParticles[i].m_index = (int)i;
		m_UnusedIndices.push( i );
	}
}

ParticlePool::~ParticlePool()
{
  if( m_pParticles )
	  delete [] m_pParticles;
}

Particle* ParticlePool::fetch()
{
  // Grab the next unused index
	size_t idx = m_UnusedIndices.front();
	m_UnusedIndices.pop();
  // Return particle at that index.
	return &(m_pParticles[idx]);
}

void ParticlePool::replace( Particle* pParticle )
{
  // Clear the particles state. 
	pParticle->m_position     = Vector4(0,0,0,0);
	pParticle->m_velocity     = Vector4(0,0,0,0);
  pParticle->m_scale        = Vector4(1.0f,1.0f,1.0f,0);
	pParticle->m_life         = 0.0f;

  // Store particle index so we know that we can use it again.
  m_UnusedIndices.push( pParticle->m_index );
}
