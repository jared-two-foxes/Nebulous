
#include "EntityRenderSystem.h"
#include "Samples/Entity/Entities/Boid.h"
#include <Nebulae/Beta/Application/Application.h>
#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Scene/SceneGraph.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>

using namespace Nebulae;

namespace Sample
{

EntityRenderSystem::EntityRenderSystem( const RenderDevicePtr& renderer )
///
///
///
: m_renderer( renderer ),
  m_camera( NULL ),
  m_sceneGraph( NULL ),
  m_atlasManager( NULL )
{
}


EntityRenderSystem::~EntityRenderSystem()
///
///
///
{
  ///@todo [jared.watt 09-08-2013] we are currently leaking actor objects!
}


void
EntityRenderSystem::Init( uint32 count )
///
/// Generates all internal structures. 
///
/// @return 
///   Nothing
///
{
  //setup camera
  m_camera = std::shared_ptr<Camera>( new Camera() );
  Vector4 vCameraEye( 0.0f, 0.0f, 500.0f );
  Vector4 vLookAt( 0.0f, 0.0f, 0.0f );
  Vector4 vUp( 0.0f, 1.0f, 0.0f );
  m_camera->LookAt( vCameraEye, vLookAt, vUp );
  m_camera->SetOrtho( 800.0f, 600.0f, 0.1f, 100.0f );

  //create the scene graph.
  m_sceneGraph = new SceneGraph( m_renderer );
  m_sceneGraph->Initialize();

  //generate all the scene actors
  Actor* actorArray = new Actor[count];
  for( uint32 i = 0; i < count; ++i )
  {
    Actor& actor     = actorArray[i];
    actor.identifier = -1;
    actor.node       = m_sceneGraph->GetRootSceneNode()->CreateChild();
    actor.node->SetVisible( false );
    actor.processed  = false;
    m_unusedActors.push_back( &actor );
  }

  m_atlasManager = new SpriteAtlasManager( nullptr, m_renderer ); //< @todo - need to pass the FileSystem ptr here.
}


void 
EntityRenderSystem::Render( const std::vector<Entity*>& entities )
///
/// Update the renderscape of the current simulation step; Updating visual positions, appearances,
/// and removing 'dead' entities and createing new Actors for new entities.
///
/// @return
///   Nothing.
///
/// @todo [jared.watt 09-08-2013] Currently casts all entities to boids which is obviously an assumption
///       that the only type of Entity are Boids and creates a _bad_ dependency on the boid object.
///
{
  //flag actors initial processing state.
  std::vector<Actor*>::iterator end_it = m_actors.end();
  for( std::vector<Actor*>::iterator it = m_actors.begin(); it != end_it; ++it )
  {
    Actor& actor = *(*it);
    actor.node->SetVisible( false );
    actor.processed = false;
  }

  //process!
  std::vector<Entity*>::const_iterator end_entity_it = entities.end();
  for( std::vector<Entity*>::const_iterator entity_it = entities.begin(); entity_it != end_entity_it; ++entity_it )
  {
    Boid* boid = static_cast<Boid*>( *entity_it );

    //find actor associated with this entity.
    Actor* actor = FindActorByIdentifier( boid->GetIdentifier() );
    if( !actor )
    {
      //grab an unused actor?
      actor = m_unusedActors.back();
      m_unusedActors.pop_back();

      actor->identifier = boid->GetIdentifier();
      actor->node->SetVisible( true );
      
      SpriteAtlas* atlas = m_atlasManager->GetByName("sparkle_01.atlas");
      if( !atlas )
      {
        atlas = m_atlasManager->Create("sparkle_01.atlas");
      }

      //@todo [jared.watt 09-08-2013] this is here cause its convienent not cause its good.
      SpriteAtlasUtils::AttachFrameToNode( m_renderer, nullptr, actor->node, atlas, "module0" ); //< @todo - need to pass the material object here.

      m_actors.push_back( actor );
    }

    //update the nodes position.
    actor->node->SetPosition( boid->GetPlacement().position );
    actor->node->SetVisible( true );
    //@todo [jared.watt 08-09-2013] update the nodes animation state?
    actor->processed = true;
  }

  //check for unprocessed actors.  If they haven't been processed then we can remove them.
  std::vector<Actor*>::iterator it = m_actors.begin();
  while( it != m_actors.end() )
  {
    if( !(*it)->processed )
    { 
      Actor* actor = *it;
      m_unusedActors.push_back( actor );
      it = m_actors.erase( it );
    }
    else
    {
      ++it;
    }
  }

  //draw the world as it exists in this simulation step.
  if( m_sceneGraph )
  {
    m_sceneGraph->Render( m_camera.get() );
  }
}

EntityRenderSystem::Actor* 
EntityRenderSystem::FindActorByIdentifier( uint32 identifier ) const
///
/// Searches for an Actor with a given identifier.
///
/// @param identifier
///   The identifier of the Actor to find.
///
/// @return
///   The Actor object of a given identifier.
///
/// @todo [jared.watt 09-08-2013] Could probably just assert the range and index directly based upon
///       identifier?
///
{
  assert( identifier != -1 );

  std::vector<Actor*>::const_iterator end_it = m_actors.end();
  for( std::vector<Actor*>::const_iterator it = m_actors.begin(); it != end_it; ++it )
  {
    if( (*it)->identifier == identifier )
    {
      return (*it);
    }
  }

  return NULL;
}

}
