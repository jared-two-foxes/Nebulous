
#include "Model.h"

#include "Core/Entity.h"
#include "Core/EntityPool.h"
#include "Core/EntityTemplate.h"

#include "Components/MotionComponent.h"
#include "Components/PlacementComponent.h"

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimation.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimationManager.h>

using namespace Sample;
using namespace Nebulae;

Model::Model( std::shared_ptr<TemplateCache>& templateCache )
  : m_templates( templateCache ),
    m_entities()
{}

Model::~Model()
{}

void
Model::Init()
///
/// Runs setup procedures on internal structures that make up the model.
///
/// @return
///   Nothing.
/// 
{
  m_entities.Init( 256 );
  m_components.Init( 256 );
}

void
Model::Clear()
{
  m_entities.Clear();
  m_components.Clear();
}

ComponentCache& 
Model::GetComponentCache()
///
/// Gets the container for all of the entity components.
///
/// @return
///   The components.
///
{
  return m_components;
}

const std::vector<Entity*>& 
Model::GetEntities() const
///
/// Retrieves the GameWorlds Entity objects.
///
/// @return
///	  list of all of the entities in the world.
///
{
  return m_entities.GetEntities();
}

std::shared_ptr<TemplateCache>
Model::GetTemplates() 
///
/// Retrieves the Entity templates.
///
/// @return
///	  The template cache.
///
{
  return m_templates;
}

Entity* 
Model::CreateEntity( const char* templateName, const char* uniqueName )
///
/// @todo [jared.watt 05-10-2013]
///   Determine if an Entity with the given uniqueName already exists; If there is a valid uniqueName 
///   obviously.
///
{
  Entity* entity = m_entities.SpawnEntity( uniqueName );
  if( entity ) 
  {
    EntityTemplate* entityTemplate = m_templates->GetTemplateById( m_templates->GetTemplateIdentifier(templateName) );
    if( entityTemplate )
    {
      ApplyTemplate( *entity, *entityTemplate );
    }
  }
  
  return entity;
}

Entity*
Model::FindEntityByName( const char* uniqueName ) const
///
/// Search for an Entity of a given name.
///
/// @param uniqueName
///   The unique name of the Entity to search for.
///
/// @return
///   Pointer to the Entity if found else NULL
///
{
  return m_entities.FindEntityByIdentifier( uniqueName );
}

void    
Model::DestroyEntity( Entity* entity )
///
/// Removes an Entity from the model, completely destroying it and all its components.
/// 
/// @param entity
///   the entity to destroy.
///   
/// @return
///   Nothing
///
{
  if( entity != NULL ) {
    int32 identifier = entity->GetIdentifier();
    entity->SetEnabled( false );

    //m_components.RemoveComponentFromObject<AnimationComponent>( identifier );
    //m_components.RemoveComponentFromObject<AspectComponent>( identifier );
    //m_components.RemoveComponentFromObject<CollisionComponent>( identifier );
    //m_components.RemoveComponentFromObject<MotionComponent>( identifier );
    //m_components.RemoveComponentFromObject<ParticleComponent>( identifier );
    //m_components.RemoveComponentFromObject<PlacementComponent>( identifier );
    //m_components.RemoveComponentFromObject<PlacementConstraintComponent>( identifier );
    //m_components.RemoveComponentFromObject<ScoreComponent>( identifier );
  }
}

void
Model::LoadSceneFile( File& stream )
/// @todo [jared.watt 10-10-2013] Comment this!
{
  const Real maximumVelocity = std::numeric_limits<Real>::max();

  Json::Value root;
  bool        success = ParseJSON( stream, &root );
  
  if( success ) {
    Json::Value& entities = root["objects"];

  //
  // Iterate each entity and load it from data
  //    
    for( uint32 i = 0, n = entities.size(); i < n; ++i ) {
      Json::Value& objectData       = entities[i];
      const char*  uniqueIdentifier = objectData["identifier"].asCString();
      const char*  templateName     = objectData["type"].asCString();

      Entity* entity = CreateEntity( templateName, uniqueIdentifier );

      int32        identifier    = entity->GetIdentifier();
      
      /*Json::Value& componentData = objectData["components"];

      AnimationComponent* animationComponent = m_components.GetComponent<AnimationComponent>( identifier );
      if( animationComponent != NULL ) {
        Json::Value& animationData = componentData["animation"];

        animationComponent->currentFrame   = json_cast_with_default<int>(         animationData["currentFrame"],   0 );
        animationComponent->currentElapsed = json_cast_with_default<float>(       animationData["currentElapsed"], 0.f );
        animationComponent->animationName  = json_cast_with_default<const char*>( animationData["animation"],      "" );
      }

      AspectComponent* aspectComponent = m_components.GetComponent<AspectComponent>( identifier );
      if( aspectComponent != NULL ) {
        Json::Value& aspectData = componentData["aspect"];

        aspectComponent->atlasName = json_cast_with_default<const char*>( aspectData["atlas"],  "" );
        aspectComponent->frameName = json_cast_with_default<const char*>( aspectData["frame"],  "" );
        aspectComponent->height    = json_cast_with_default<float>(       aspectData["height"], 1.0f );
        aspectComponent->width     = json_cast_with_default<float>(       aspectData["width"],  1.0f );
        aspectComponent->flags     = json_cast_with_default<int>(         aspectData["flags"],  0 );
      }

      MotionComponent* motionComponent = m_components.GetComponent<MotionComponent>( identifier );
      if( motionComponent != NULL ) {
        Json::Value& motionData = componentData["motion"];

        motionComponent->velocity        = json_cast_with_default<Vector4>( motionData["velocity"],        Vector4(0,0,0,0) );
        motionComponent->maximumVelocity = json_cast_with_default<Real>(    motionData["maximumVelocity"], maximumVelocity );
        motionComponent->acceleration    = json_cast_with_default<Vector4>( motionData["acceleration"],    Vector4(0,0,0,0) );
      }

      std::vector<PlacementComponent*> placementComponents = m_components.GetComponents<PlacementComponent>( identifier );
      if( !placementComponents.empty() ) {
        Json::Value& placementData = componentData["placement"];

        placementComponents[0]->position = json_cast_with_default<Vector4>(    placementData["position"], Vector4(0,0,0,0) );
        placementComponents[0]->rotation = json_cast_with_default<Quaternion>( placementData["rotation"], Quaternion(0,0,0,1) );
      }

      std::vector<PlacementConstraintComponent*> placementConstraintComponents = m_components.GetComponents<PlacementConstraintComponent>( identifier );
      if( !placementConstraintComponents.empty() ) {
        Json::Value& placementConstraintData = componentData["placementConstraint"];

        placementConstraintComponents[0]->leftBound   = json_cast_with_default<Real>( placementConstraintData["leftBound"],   std::numeric_limits<Real>::lowest() );
        placementConstraintComponents[0]->rightBound  = json_cast_with_default<Real>( placementConstraintData["rightBound"],  std::numeric_limits<Real>::max() );
        placementConstraintComponents[0]->topBound    = json_cast_with_default<Real>( placementConstraintData["topBound"],    std::numeric_limits<Real>::max() );
        placementConstraintComponents[0]->bottomBound = json_cast_with_default<Real>( placementConstraintData["bottomBound"], std::numeric_limits<Real>::lowest() );
        placementConstraintComponents[0]->backBound   = json_cast_with_default<Real>( placementConstraintData["backBound"],   std::numeric_limits<Real>::lowest() );
        placementConstraintComponents[0]->frontBound  = json_cast_with_default<Real>( placementConstraintData["frontBound"],  std::numeric_limits<Real>::max() );
      }

      std::vector<CollisionComponent*> collisionComponents = m_components.GetComponents<CollisionComponent>( identifier );
      if( !collisionComponents.empty() ) {
        Json::Value& collisionData = componentData["collision"];

        collisionComponents[0]->halfExtents = json_cast_with_default<Vector4>( collisionData["bounds"], Vector4(0,0,0,0) );
      }*/
    }
  }
}

void 
Model::ApplyTemplate( Entity& entity, EntityTemplate& entityTemplate )
///
/// Will associate a set of components to an entity based upon the contents of the EntityTemplate.
///
/// @param entity.
///   The entity to associate components to.
///
/// @param entityTemplate
///	  The descriptor of components to associate.
///
/// @return
///   Nothing.
///
{
  int identifier = entity.GetIdentifier();

  if( entityTemplate.ContainsComponent("Motion") ) {
    int32 index = m_components.GetNextAvailableIndex<MotionComponent>();
    if( index != Component::UNUSED ) {
      //@todo add the component to an entity.
    }
  }

  //if( entityTemplate.ContainsComponent("Placement") ) {
  //  m_components.AddComponentToObject<PlacementComponent>( identifier );
  //}

  //if( entityTemplate.ContainsComponent("PlacementConstraint") ) {
  //  m_components.AddComponentToObject<PlacementConstraintComponent>( identifier );
  //}
  //
  //if( entityTemplate.ContainsComponent("Collision") ) {
  //  m_components.AddComponentToObject<CollisionComponent>( identifier );
  //}

  //if( entityTemplate.ContainsComponent("Aspect") ) {
  //  m_components.AddComponentToObject<AspectComponent>( identifier );
  //}

  //if( entityTemplate.ContainsComponent("Animation") ) {
  //  m_components.AddComponentToObject<AnimationComponent>( identifier );
  //}

  //if( entityTemplate.ContainsComponent( "Particle" ) ) {
  //  m_components.AddComponentToObject<ParticleComponent>( identifier );
  //}
}