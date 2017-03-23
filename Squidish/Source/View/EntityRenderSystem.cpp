
#include "EntityRenderSystem.h"

#include <Nebulae/Alpha/Texture/SubTexture.h>

#include <Nebulae/Beta/Application/Application.h>
#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Debug/DebugUtil.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/MaterialSerializer.h>
#include <Nebulae/Beta/Scene/SceneGraph.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>
#include <Nebulae/Beta/Scene/UniformParameters.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimationManager.h>

#include <Model/Component/AnimationComponent.h>
#include <Model/Component/AspectComponent.h>
#include <Model/Component/BehaviourComponent.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Model.h>
#include <Model/Core/Entity.h>

#include <Model/Systems/MotionSystem.h >

using namespace Nebulae;
using namespace Game;


PrimitiveBatch<Vertex >& EntityRenderSystem::GetPrimitiveBatch()
{
  return m_primitiveBatch;
}


EntityRenderSystem::EntityRenderSystem( FileArchivePtr fileSystem, RenderSystemPtr renderDevice, 
  SceneGraphPtr sceneGraph, SpriteAtlasManagerPtr spriteAtlasManager, 
  SpriteAnimationManagerPtr spriteAnimationManager, DebugRendererPtr debugRenderer, ModelPtr model  )
///
/// Constructor.
///
: m_fileSystem( fileSystem ),
  m_renderDevice( renderDevice ),
  m_camera( nullptr ),
  m_sceneGraph( sceneGraph ),
  m_atlasManager( spriteAtlasManager ),
  m_animationManager( spriteAnimationManager ),
  m_debugUtil( debugRenderer ),
  m_model( model ), 
  m_primitiveBatch( renderDevice, 2048U )
{}


EntityRenderSystem::~EntityRenderSystem()
///
/// Destructor.
///
{
  ///@todo [jared.watt 09-08-2013] we are currently leaking actor objects!
}


void 
EntityRenderSystem::Clear()
{
  std::vector<Actor*>::iterator it = m_actors.begin();
  while( it != m_actors.end() ) {
    Actor* actor      = *it;
    actor->identifier = -1;
    actor->processed  = false;
    actor->node->Clear();
    actor->node->SetVisible( false );
    m_unusedActors.push_back( actor );
    it = m_actors.erase( it );
  }
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
  File* stream = m_fileSystem->Open( SQUID_DEFAULT_ROOTDEVICE, "materials/default_billboard.material" );
  if( stream ) {
    Material*          material = new Material( "materials/default_billboard.material" );
    MaterialSerializer serializer( m_renderDevice );
    serializer.Serialize( *stream, material );

    m_materials.push_back( material );
  }

  //generate all the scene actors
  Actor* actorArray = new Actor[count];
  for( uint32 i = 0; i < count; ++i ) {
    Actor& actor     = actorArray[i];
    actor.identifier = -1;
    actor.node       = m_sceneGraph->GetRootSceneNode()->CreateChild();
    actor.node->SetVisible( false );
    actor.processed  = false;
    m_unusedActors.push_back( &actor );
  }

  m_effect.Init( m_renderDevice );
}

void
EntityRenderSystem::SetCamera( CameraPtr camera )
///
/// Sets the Camera to be used for rendering this system.
///
/// @param camera
///   The camera
///
/// @return
///   Nothing.
/// 
{
  m_camera = camera;
}

void 
EntityRenderSystem::Process( const std::vector<Entity*>& entities )
///
/// Update the renderscape of the current simulation step; Updating visual positions, appearances,
/// and removing 'dead' entities and createing new Actors for new entities.
///
/// @return
///   Nothing.
///
{
  Nebulae::Material* billboardingMaterial = FindMaterialByName( "materials/default_billboard.material" );

  //flag actors initial processing state.
  std::vector<Actor*>::iterator end_it = m_actors.end();
  for( std::vector<Actor*>::iterator it = m_actors.begin(); it != end_it; ++it ) {
    Actor& actor = *(*it);
    actor.node->SetVisible( false );
    actor.processed = false;
  }

  std::vector<Entity* > entitiesCopy;

  std::for_each( entities.begin(), entities.end(), [&](Entity* entity){
    if( entity->IsEnabled() ) entitiesCopy.push_back(entity);
  });

  std::sort( entitiesCopy.begin(), entitiesCopy.end(), [](Entity* entity1, Entity* entity2){
    if( entity1->IsEnabled() && !entity2->IsEnabled() ) 
      return false;
    else if( !entity1->IsEnabled() && entity2->IsEnabled() ) 
      return true;
    else 
      return (int32)entity1 < (int32)entity2; 
  });
  
  Matrix4 identity;
  identity.SetIdentity();

  m_effect.SetWorld( identity );
  m_effect.SetView( m_camera->GetViewMatrix() );
  m_effect.SetProjection( m_camera->GetProjectionMatrix() );

  m_primitiveBatch.Begin( &m_effect );

  Entity* player         = m_model->FindEntityByName( "squid" );
  Vector4 playerPosition = MotionSystem::GetPosition( m_model, *player );

  //process!
  std::vector<Entity*>::const_iterator end_entity_it = entitiesCopy.end();
  for( std::vector<Entity*>::const_iterator entity_it = entitiesCopy.begin(); entity_it != end_entity_it; ++entity_it ) {
    //if( !(*entity_it)->IsEnabled() ) break;

    // determine if this entity has the appropriate "aspect" component for this system to draw.
    std::vector<AnimationComponent* > animationComponents;
    std::vector<AspectComponent* >    aspectComponents; 
    std::vector<PlacementComponent* > placementComponents;

    m_model->GetComponents<AnimationComponent>( *entity_it, &animationComponents );
    m_model->GetComponents<AspectComponent>( *entity_it, &aspectComponents );
    m_model->GetComponents<PlacementComponent>( *entity_it, &placementComponents );

    if( !aspectComponents.empty() && !placementComponents.empty() ) {
      //find actor associated with this entity.
      bool   newActor = false;
      Actor* actor = FindActorByIdentifier( (*entity_it)->GetIdentifier() );
      if( !actor ) {
        //grab an unused actor?
        actor = m_unusedActors.back();
        m_unusedActors.pop_back();

        actor->identifier = (*entity_it)->GetIdentifier();
        actor->node->SetVisible( true );

        newActor = true;
        m_actors.push_back( actor );
      }

      if( newActor || !animationComponents.empty() ) {
        actor->node->Clear();

        SpriteAtlas* atlas = m_atlasManager->GetByName( aspectComponents[0]->atlasName.c_str() );
        if( !atlas ) {
          atlas = m_atlasManager->Create( aspectComponents[0]->atlasName.c_str() );
        } 

        //@todo [jared.watt 09-08-2013] this is here because its convenient not cause its good.
        SpriteAtlasUtils::AttachFrameToNode( m_renderDevice, billboardingMaterial, actor->node, atlas, aspectComponents[0]->frameName.c_str(), aspectComponents[0]->flags );

        SubTexture* subTexture = atlas->FindModuleSubTexture( aspectComponents[0]->frameName );
        NE_ASSERT( subTexture, "")();
        if ( subTexture != NULL )
        {
          aspectComponents[0]->width  = subTexture->GetWidth();
          aspectComponents[0]->height = subTexture->GetHeight();
        }
      }

      //update the nodes position.
      Vector4 rect;
      aspectComponents[0]->GetRect( placementComponents[0]->position, &rect );

      Vector4 position;
      position.x = rect.x;
      position.y = rect.w;
      position.z = placementComponents[0]->position.z;

      actor->node->SetPosition( position );
      actor->node->SetVisible( true );

      SceneObject* object = actor->node->FindSubObject( billboardingMaterial );
      NE_ASSERT( object, "Unable to find SceneObject for material '%s'", billboardingMaterial->GetName() )();
      UniformParameters& uniforms = object->GetUniformParameters();
      uniforms.SetNamedUniform( "tintColour", actor->tintColour );
      uniforms.SetNamedUniform( "tintFactor", actor->tintStrength );
      uniforms.SetNamedUniform( "depth", placementComponents[0]->position.z );

      
      Vertex v1, v2, v3, v4;
      v1.position = Vector4( rect.x, rect.y, 0 );
      v1.colour   = Vector4( 0, 0, 0, 1.0f );
      v2.position = Vector4( rect.z, rect.y, 0 );
      v2.colour   = Vector4( 0, 0, 0, 1.0f );
      v3.position = Vector4( rect.z, rect.w, 0 );
      v3.colour   = Vector4( 0, 0, 0, 1.0f );
      v4.position = Vector4( rect.x, rect.w, 0 );
      v4.colour   = Vector4( 0, 0, 0, 1.0f );

      m_primitiveBatch.DrawLine( v1, v2 );
      m_primitiveBatch.DrawLine( v2, v3 );
      m_primitiveBatch.DrawLine( v3, v4 );
      m_primitiveBatch.DrawLine( v4, v1 );


      //@todo [jared.watt 08-09-2013] Update the nodes animation state?
      actor->processed = true;
    }
  
    std::vector<BehaviourComponent* > behaviourComponents;

    m_model->GetComponents<BehaviourComponent >( *entity_it, &behaviourComponents );

    if ( !behaviourComponents.empty() )
    {
      if( behaviourComponents[0]->m_attractionStrength <= 0 ) continue;

      Vector4 entityPosition = MotionSystem::GetPosition( m_model, *(*entity_it) );
      Vector4 direction      = MotionSystem::GetDistanceBetweenEntities( m_model, *player, *(*entity_it) );
	  Real    distance       = direction.length();
      Vector4 colour         = distance < behaviourComponents[0]->m_attractionInfluenceRadius ? Vector4(1.0f, 0, 0, 1.0f) : Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	  if ( distance < 2*behaviourComponents[0]->m_attractionInfluenceRadius )
	  {
		  Vertex v1, v2;
		  v1.position = Vector4( playerPosition.x, playerPosition.y, 0 );
		  v1.colour   = colour;
		  v2.position = Vector4( entityPosition.x, entityPosition.y, 0 );
		  v2.colour   = colour;

		  m_primitiveBatch.DrawLine( v1, v2 );
	  }
    }
  }

  //check for unprocessed actors.  If they haven't been processed then we can remove them.
  std::vector<Actor*>::iterator it = m_actors.begin();
  while( it != m_actors.end() ) {
    if( !(*it)->processed ) { 
      Actor* actor = *it;
      actor->identifier = -1;
      actor->processed  = false;
      actor->node->Clear();
      actor->node->SetVisible( false );
      actor->tintStrength = 0.0f;
      actor->tintColour = CLR_WHITE;
      m_unusedActors.push_back( actor );
      it = m_actors.erase( it );
    } else {
      ++it;
    }
  }
}


void 
EntityRenderSystem::ApplyTintFactor( const Entity& entity, const Real factor )
///
/// Sets the tint strength for an entity.
///
/// @param entity
///   The entity to apply the tint colour too.
///
/// @param tintValue
///   The tintColour.
///
/// @return
///   Nothing.
///
{
  Actor* actor = FindActorByIdentifier( entity.GetIdentifier() );
  if( actor != NULL ) {
    actor->tintStrength = factor;
  }
}


void
EntityRenderSystem::ApplyTintColour( const Entity& entity, const Nebulae::Colour& colour )
///
/// Sets the tint colour for an entity.
///
/// @param entity
///   The entity to apply the tint colour too.
///
/// @param tintValue
///   The tintColour.
///
/// @return
///   Nothing.
///
{
  Actor* actor = FindActorByIdentifier( entity.GetIdentifier() );
  if( actor != NULL ) {
    actor->tintColour = colour;
  }
}


Nebulae::Material* 
EntityRenderSystem::FindMaterialByName( const std::string& name ) const
///
/// Searches the list of Materials for one with a given name and returns it.
///
/// @param name
///   The name of the material to search for.
///
/// @return
///   The material, or NULL if it cant be found.
///
{
  std::vector<Nebulae::Material* >::const_iterator i = m_materials.begin();
  for( ; i != m_materials.end(); ++i ) {
    if( name.compare((*i)->GetName()) == 0 ) {
      return *i;
    }
  }

  return NULL;
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
  NE_ASSERT( identifier != -1, "Identifier is Invalid" )();

  std::vector<Actor*>::const_iterator end_it = m_actors.end();
  for( std::vector<Actor*>::const_iterator it = m_actors.begin(); it != end_it; ++it ) {
    if( (*it)->identifier == identifier ) {
      return (*it);
    }
  }

  return NULL;
}

