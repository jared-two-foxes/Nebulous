
#include "SampleState.h"

#include <Samples/Entity/Core/Entity.h>
#include <Samples/Entity/Core/EntityManager.h>
#include <Samples/Entity/Entities/Boid.h>
#include <Samples/Entity/Systems/IntelligenceSystem.h>
#include <Samples/Entity/Systems/MotionSystem.h>

#include <Samples/Entity/Systems/EntityRenderSystem.h>

#include <Nebulae/Beta/StateStack/StateStack.h>
#include <Nebulae/Beta/Debug/DebugUtil.h>

using namespace Nebulae;

namespace Sample
{
#define RAND_FLOAT( min, max ) (min + ((float(rand()) / RAND_MAX) * (max - min)))

SampleState::SampleState() 
///
/// Default constructor.  Does nothing interesting.
///
  : State( "Sample" )
  , m_entityManager( NULL )
  , m_intelligenceSystem( NULL )
  , m_motionSystem( NULL )
  , m_entityRenderer( NULL )
{

}

SampleState::~SampleState()
///
/// Destructor.  Destroys internal resources.
///
{
  if( m_entityManager ) {
    delete m_entityManager;
  }
  m_entityManager = NULL;

  if( m_intelligenceSystem ) {
    delete m_intelligenceSystem;
  }
  m_intelligenceSystem = NULL;

  if( m_motionSystem ) {
    delete m_motionSystem;
  }
  m_motionSystem = NULL;

  if( m_entityRenderer ) {
    delete m_entityRenderer;
  }
  m_entityRenderer = NULL;
}

void 
SampleState::Enter( Nebulae::StateStack* caller )
///
/// Prepares all internal structures that are required for the states execution.  This includes
/// creating the EntityManager & WorldRenderer.  Also initiated are all the boid objects that are
/// going to be present in the flocking simulation.
///
/// @return
///   Nothing.
///
/// @todo [jared.watt 29-07-2013] Bound the random position into some form of area frustrum.
/// @todo [jared.watt 13-08-2013] set a random heading for the boid.
///
{
  static const int entityCount = 32;

  m_entityManager = new EntityManager();
  m_entityManager->RegisterAllocator( BOID, new BoidAllocator() );

  m_intelligenceSystem = new IntelligenceSystem();
  m_motionSystem       = new MotionSystem();

  m_entityRenderer = new EntityRenderSystem( caller->GetRenderSystem() );
  m_entityRenderer->Init( entityCount );

#ifdef _DEBUG
  m_debugRenderer = new Nebulae::DebugUtil( caller->GetRenderSystem() );
  m_debugRenderer->Init();
#endif

  // Spawn some entities to use as Boids.
  for( int i = 0; i < entityCount; ++i ) 
  {
    Boid*               boid      = m_entityManager->SpawnEntity<Boid>( BOID );
    PlacementComponent& placement = boid->GetPlacement();
    MotionComponent&    motion    = boid->GetMotion();

    //set a random position for the boid
    placement.position = Vector4( RAND_FLOAT(-200.0f,200.0f), RAND_FLOAT(-200.0f,200.0f), RAND_FLOAT(-200.0f,200.0f), 1.0f );
    placement.rotation = Quaternion( 0,0,0,1 );

    motion.velocity = Vector4( RAND_FLOAT(-10.0f,10.0f), RAND_FLOAT(-10.0f,10.0f), RAND_FLOAT(-10.0f,10.0f), 0.0f ); 
  }
}

void 
SampleState::Exit( Nebulae::StateStack* caller )
///
/// Does nothing interesting.
///
/// @return
///   Nothing.
///
{
}

void 
SampleState::Update( float elapsed, StateStack* pCaller )
///
/// @todo [jared.watt 29-07-2013] Needs documentation.
///
{
  std::vector<Entity*> entities;
  m_entityManager->GetEntities( &entities );

  if( m_intelligenceSystem ) {
    m_intelligenceSystem->Process( elapsed, entities );
  }
  if( m_motionSystem ) {
    m_motionSystem->Process( elapsed, entities );
  }
} 

void 
SampleState::Render() const
///
/// Renders the curent scene.
///
/// @return
///   Nothing.
/// 
{
  std::vector<Entity*> entities;
  m_entityManager->GetEntities( &entities );

  if( m_entityRenderer )
  {
    m_entityRenderer->Render( entities );
  }

#ifdef _DEBUG
  m_debugRenderer->Reset();
  m_motionSystem->DebugDraw( *m_debugRenderer );
  m_debugRenderer->Draw( m_entityRenderer->GetCamera() );
#endif
}

}