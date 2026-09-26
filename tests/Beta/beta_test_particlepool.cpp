#include <Nebulae/Beta/Particle/Particle.h>
#include <Nebulae/Beta/Particle/ParticleEmitter.h>
#include <Nebulae/Beta/Particle/ParticlePool.h>

// The unloaded group has no rendering dependencies; configure its spawn defaults directly.
#define private public
#include <Nebulae/Beta/Particle/ParticleGroup.h>
#undef private

#include "gtest/gtest.h"

using namespace Nebulae;

TEST( ParticlePool, FetchReturnsNullWhenCapacityIsExhausted )
{
  ParticlePool pool( 2 );

  Particle* first = pool.fetch();
  Particle* second = pool.fetch();

  ASSERT_NE( nullptr, first );
  ASSERT_NE( nullptr, second );
  EXPECT_EQ( nullptr, pool.fetch() );
  EXPECT_NE( first, second );
}

TEST( ParticlePool, ReplaceMakesTheParticleAvailableAgain )
{
  ParticlePool pool( 1 );
  Particle* particle = pool.fetch();
  ASSERT_NE( nullptr, particle );
  ASSERT_EQ( nullptr, pool.fetch() );

  pool.replace( particle );

  EXPECT_EQ( particle, pool.fetch() );
  EXPECT_EQ( nullptr, pool.fetch() );
}

TEST( ParticlePool, FetchPreservesIndexOrder )
{
  ParticlePool pool( 3 );

  Particle* first = pool.fetch();
  Particle* second = pool.fetch();
  Particle* third = pool.fetch();

  ASSERT_NE( nullptr, first );
  ASSERT_NE( nullptr, second );
  ASSERT_NE( nullptr, third );
  EXPECT_EQ( 0u, first->m_index );
  EXPECT_EQ( 1u, second->m_index );
  EXPECT_EQ( 2u, third->m_index );
}

TEST( ParticlePool, EmitterStopsSpawningWhenGroupReachesCapacity )
{
  ParticleGroup group( nullptr, nullptr, "test", 1 );
  group.m_template_life.SetConstant( 1.0f );
  group.m_template_scale.SetConstant( 1.0f );

  ParticleEmitter emitter( "test" );
  Distribution<Vector4> force;
  force.SetConstant( Vector4( 0, 0, 0 ) );
  emitter.SetEmissiveForce( force );
  emitter.AddParticlesToReservoir( &group, 3, 0.0f );
  emitter.Start();

  emitter.Update( 0 );

  EXPECT_EQ( 1u, group.GetParticleCount() );
  EXPECT_TRUE( emitter.IsEmpty() );
  EXPECT_FALSE( emitter.IsActive() );
}
