

#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleGroup.h"
#include "ParticlePool.h"
#include "ParticleSystem.h"

using namespace Nebulae;

Real RandFloat( const Real min, const Real max )
{
  return min + ((float(rand()) / RAND_MAX) * (max - min));
}


ParticleEmitter::ParticleEmitter( const std::string& name )
: m_name( name ),
  m_active( false ),
  m_position( 0,0,0 ),
  m_rotation( 0,0,0,1 )
{}

ParticleEmitter::~ParticleEmitter()
{}

void ParticleEmitter::Clear()
{}

void 
ParticleEmitter::Start()
{
  NE_ASSERT( !IsEmpty(), "Attempting to start an Emitter that has no particles left in the reservoir." )();
  
  m_active = true;
}

void
ParticleEmitter::Pause()
{
  m_active = false;
}

void
ParticleEmitter::Update( const uint64 elapsed )
{
  // Early out if we are not currently active.
  if( !m_active ) return;
    
  float deltaSeconds = float(elapsed) / 1000000; //< convert microseconds to seconds.

//
// Spawn new particles.
//
  bool reservoirEmpty = true;
  std::vector<ReservoirElement>::iterator end_it = m_reservoir.end();
  for( std::vector<ReservoirElement>::iterator it = m_reservoir.begin(); it != end_it; ++it )
  {
    ParticleGroup* group = (*it).m_group;

  //
  // Determine how many particles are to be born.
  //
    uint32 count = 0;
    if( (*it).m_flow <= 0.0f ) 
    {
      count = std::max<uint32 >( 0, (*it).m_tank );
      (*it).m_tank = 0;
    } 
    else if( (*it).m_tank > 0 ) 
    {
      (*it).m_fraction += (*it).m_flow * deltaSeconds;

      count = std::min<uint32 >( (*it).m_tank, static_cast<uint32 >((*it).m_fraction) );
      (*it).m_tank     -= count;
      (*it).m_fraction -= count;
    } 
    else 
    {
      count = 0;
    }

    if( (*it).m_tank > 0 ) reservoirEmpty = false;

  //
  // Create particles.
  //
    for( size_t i = 0; i < count; ++i ) 
    {
      Particle* particle = group->SpawnParticle();
      
      NE_ASSERT( particle != NULL, "Unable to spawn new particle." )();

      if( particle ) 
      {
        // Store the original position.
        particle->m_position  = m_position; //< Assumes the emitters location is the emitter position.
        particle->m_velocity  = m_emissionForce.Value();
      }
    }
  }

  // Check if the reservoir is empty, if so we can stop updating the emitter.
  if( reservoirEmpty )
  {
    m_active = false;
  }
}

void 
ParticleEmitter::SetPosition( const Vector4& position )
{ m_position = position; }

void 
ParticleEmitter::SetRotation( const Quaternion& rotation )
{ m_rotation = rotation; }

void
ParticleEmitter::SetEmissiveForce( const Distribution<Vector4 >& force )
{ m_emissionForce = force; }

const Vector4&
ParticleEmitter::GetPosition() const
{ return m_position; }

const Quaternion&
ParticleEmitter::GetRotation() const
{ return m_rotation; }

bool
ParticleEmitter::IsEmpty() const
{
  bool empty = true;
  for( std::vector<ReservoirElement >::const_iterator it = m_reservoir.begin(); it != m_reservoir.end(); ++it )
  {
    if( (*it).m_tank > 0 ) 
    {
      empty = false;
      break;
    }
  }

  return empty;
}

bool
ParticleEmitter::IsActive() const
{
  return m_active;
}

void 
ParticleEmitter::AddParticlesToReservoir( ParticleGroup* group, uint32 count, Real flow )
{
  ReservoirElement elem;
  elem.m_group        = group;
  elem.m_tank         = count;
  elem.m_flow         = flow;
  elem.m_fraction     = 0.0f;

  m_reservoir.push_back( elem );
}

