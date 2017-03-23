
#include "ParticleViewSystem.h"

#include <Model/Model.h>
#include <Model/Core/Entity.h>
#include <Model/Component/ParticleComponent.h>
#include <Model/Component/PlacementComponent.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Particle/ParticleEmitter.h>
#include <Nebulae/Beta/Particle/ParticleSystem.h>


using namespace Nebulae;
using namespace Game;

ParticleViewSystem::ParticleViewSystem( const FileArchivePtr& fileSystem, const RenderSystemPtr& renderDevice, const ParticleSystemPtr& particleSystem, const ModelPtr& model )
: m_fileSystem( fileSystem ),
  m_renderDevice( renderDevice ),
  m_particleSystem( particleSystem ),
  m_camera( nullptr ),
  m_model( model )
{}

ParticleViewSystem::~ParticleViewSystem()
{}

void 
ParticleViewSystem::Clear()
{}

void
ParticleViewSystem::Init( uint32 count )
{
  if( m_camera != nullptr )
  {
    m_particleSystem->SetCamera( m_camera.get() );
  }
}

void 
ParticleViewSystem::SetCamera( CameraPtr camera )
{
  if( m_camera != camera )
  {
    m_camera = camera;

    if( m_particleSystem != nullptr )
    {
      m_particleSystem->SetCamera( m_camera.get() );
    }
  }
}

void
ParticleViewSystem::Process( const std::vector<Entity* >& entities )
{
  // Mark all instances as unprocessed.
  std::for_each( m_emitters.begin(), m_emitters.end(), [](ParticleInstance& instance){ 
    instance.processed = false;
  });

  // Check for new ParticleComponents and update existing emitters.
  std::vector<Entity* >::const_iterator it = entities.begin();
  for( ; it != entities.end(); ++it )
  {
    std::vector<ParticleComponent* > particleComponents;
    m_model->GetComponents<ParticleComponent >( (*it), &particleComponents );   

    std::for_each( particleComponents.begin(), particleComponents.end(), [&](ParticleComponent* component){
      ParticleInstance* instance = FindInstance( component );
      if( instance != nullptr )
      {
        // Check for "finished" emitters
        NE_ASSERT( instance->emitter != nullptr, "ParticleInstance has a NULL emitter object." )();
        if( instance->emitter->IsEmpty() )
        {
          //@todo remove emitter from the scene.  
        }

        //@todo handle looping emitters. 
        
        instance->processed = true;
      }
      else
      {
        // Create the emitter.
      	ParticleEmitter* emitter = m_particleSystem->CreateEmitter( component->emitterName.c_str() );      	
        NE_ASSERT( emitter != nullptr, "Unable to create emitter with descripter file %s", component->emitterName.c_str() )();

        // Positioning the emitter.
        std::vector<PlacementComponent* > placementComponents;
        m_model->GetComponents<PlacementComponent >( (*it), &placementComponents );
        NE_ASSERT( !placementComponents.empty(), "Unable to find a PlacmenetComponent" )();
		    if( !placementComponents.empty() ) {        
          PlacementComponent* placementComponent = placementComponents[0];
          emitter->SetPosition( placementComponent->position );
        }

        emitter->Start();

        m_emitters.push_back( ParticleInstance() );
        ParticleInstance& instance = m_emitters.back();
        instance.component = component;
        instance.emitter   = emitter;
        instance.processed = true;
      }
    });
  }

  // Check for any dead instances.
  std::vector<ParticleInstance >::iterator itr = m_emitters.begin();
  for( ; itr != m_emitters.end(); ) 
  { 
    if( !(*itr).processed )
    {
      // Remove the emitter from the render system.
   	  m_particleSystem->DestroyEmitter( (*itr).emitter );
      
      m_emitters.erase( itr );
    }
    else
    {
      ++itr;
    }
  }

}

void 
ParticleViewSystem::Render() const
{
  PROFILE;

  m_particleSystem->Render();
}

void 
ParticleViewSystem::Update( const uint64 dt )
{
  m_particleSystem->Update( dt );
}

ParticleViewSystem::ParticleInstance* 
ParticleViewSystem::FindInstance( ParticleComponent* component )
{
  std::vector<ParticleInstance>::iterator it = std::find_if( m_emitters.begin(), m_emitters.end(), [&](ParticleInstance& instance){
    return instance.component == component;
  });

  return it != m_emitters.end() ? &(*it) : nullptr;
}