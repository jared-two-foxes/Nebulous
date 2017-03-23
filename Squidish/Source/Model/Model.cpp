
#include "Model.h"

#include "Core/Entity.h"
#include "Core/EntityPool.h"
#include "Core/EntityTemplate.h"

#include "Component/AnimationComponent.h"
#include "Component/AspectComponent.h"
#include "Component/BehaviourComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/MotionComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/PlacementComponent.h"
#include "Component/PlacementConstraintComponent.h"
#include "Component/ScoreComponent.h"

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimation.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimationManager.h>

using namespace Game;
using namespace Nebulae;

template <typename T> void
load_component( const Json::Value& data, T& object )
{
  component_loader<T>::load( data, object );
}

template <typename T> void 
load_components( const Entity* entity, Model& model, const Json::Value& value )
{
  std::vector<T* > components;
  model.GetComponents<T >( entity, &components );

  Json::Value component_data = value[ component_loader<T >::component_name() ];

  if( components.empty() ) return;

  if( component_data.isArray() )
  {
    NE_ASSERT( components.size() >= component_data.size(), "Not enough components associated with specified entity." )();

    for( int i = 0, n = component_data.size(); i < n; ++i )
    {
      load_component<T >( component_data[i], *components[i] );
    }
  }
  else
  {
    load_component<T >( component_data, *components[0] );
  }
}


template <class T >
class ComponentPoolImpl : public ComponentPool
///
/// Generic template based component pool implementation which can be used
/// as a pool for each of the component types.
///
{
private:
  typedef std::vector<int32 > IdentifierList;

private:
  ComponentType                m_type;
  std::vector<T >              m_components;
  IdentifierList               m_unusedIndices;
  std::vector<IdentifierList > m_allocations;

  public:
    ComponentPoolImpl( uint32 count ) 
    : m_type( T::GetType() )  
    {
      Init( count );
    }

    ~ComponentPoolImpl() { Clear(); }

    void Clear() 
    {
      // Flag all components as not enabled.
      std::for_each( m_components.begin(), m_components.end(), [&](T& component){
        component.SetEnabled( false );
      });

      // Push all indices into the unused list.
      m_unusedIndices.clear();
      for( size_t i = 0; i < m_components.size(); ++i )
      { 
        m_unusedIndices.push_back( i );
      }

      // Clear any entity allocations. 
      std::for_each( m_allocations.begin(), m_allocations.end(), [&](IdentifierList& list){
        list.clear();
      });
    }

    void Init( int32 count ) 
    {
      m_components.resize( count ); 

      int32 identifier = 0;
      std::for_each( m_components.begin(), m_components.end(), [&](T& component){
        component.SetIdentifier( identifier );
        identifier++;
      });

      m_allocations.resize( count );
      for( int32 i = 0; i < count; ++i )
      { 
        m_unusedIndices.push_back( i );
      }
    }

    ComponentType GetType() const { return m_type; }

    virtual T* GetComponent( int32 identifier )
    {
      NE_ASSERT( identifier >= 0, "identifier is less than zero" )( identifier );
      NE_ASSERT( identifier < (int32)m_components.size(), "identifier is greater than component list size" )( identifier, m_components.size() );
      NE_ASSERT( m_components[identifier].GetIdentifier() == identifier, "component at the specified identifier index doesn't match the identifier" )( identifier, m_components[identifier].GetIdentifier() );

      return &m_components[identifier];
    }

    virtual const int32 GetNextAvailableIndex() const
    {
      return m_unusedIndices.empty() ? Component::UNUSED : m_unusedIndices.back();
    }

    virtual void AllocateComponent( int32 component, int32 allocatedTo ) 
    {
      IdentifierList& allocations = m_allocations[component];

      // Allocation list will be empty if the component is yet to be assigned to
      // anyone.
      if( allocations.empty() )
      {
        IdentifierList::iterator it = std::find( m_unusedIndices.begin(), m_unusedIndices.end(), component );
        NE_ASSERT( it != m_unusedIndices.end(), "Unable to find identifier in the unused indices list" )( component );
        m_unusedIndices.erase( it );
      }

      // Only append the allocation identifier if it does not already exist in the list.
      IdentifierList::iterator it = std::find( allocations.begin(), allocations.end(), allocatedTo );
      if( it == allocations.end() )
      {
        allocations.push_back( allocatedTo );    
      }
    }

    virtual bool ReleaseComponent( int32 component, int32 deallocatedFrom )
    {
      IdentifierList& allocations = m_allocations[component];

      IdentifierList::iterator it = std::find( allocations.begin(), allocations.end(), deallocatedFrom );
      if( it != allocations.end() )
      {
        allocations.erase( it );
      }

      // Allocation list will be empty if the component is novlonger to be assigned to anyone.
      if( allocations.empty() )
      {
        m_unusedIndices.push_back( component );
      }

      return false;
    }

};


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

  // Add component pools to the component cache
  m_components.AddComponentPool( new ComponentPoolImpl<AnimationComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<AspectComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<BehaviourComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<CollisionComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<MotionComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<ParticleComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<PlacementComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<PlacementConstraintComponent>(256) );
  m_components.AddComponentPool( new ComponentPoolImpl<ScoreComponent>(256) );
}

void
Model::Clear()
///
/// Clears the contents of the Model by clearing the entity list and 
/// the Components from the ComponentCache.
///
/// @return
///   Nothing.
///
{
  m_entities.Clear();
  m_components.Clear();
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

std::shared_ptr<TemplateCache >
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

const ComponentCache&
Model::GetComponentCache() const
///
/// Retrieves the ComponentCache.
///
/// @return
///   The component cache.
///
{
  return m_components;
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
  return m_entities.FindEntityByUniqueName( uniqueName );
}

void    
Model::DestroyEntity( const Entity* const_entity )
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
  if( const_entity != nullptr ) 
  {
    Entity* entity = m_entities.FindEntityByIdentifier( const_entity->GetIdentifier() );

    entity->SetEnabled( false );
    
    for( int32 type = COMPONENT_TYPE_FIRST+1; type <= COMPONENT_TYPE_LAST; type++ )
    {
      std::vector<int32 > identifiers;
      entity->GetLinkedComponentIndices( ComponentType(type), &identifiers );

      std::for_each( identifiers.begin(), identifiers.end(), [&](int32 identifier) {
        entity->RemoveComponent( ComponentType(type), identifier );
        m_components.ReleaseComponent( ComponentType(type), identifier, entity->GetIdentifier() );
      });
    }
  }
}

void
Model::LoadSceneFile( File& file )
/// @todo [jared.watt 10-10-2013] Comment this!
{
  Json::Value root;
  bool        success = ParseJSON( file, &root );
  
  if( success ) 
  {
    Json::Value& entities = root["objects"];

    // Iterate each entity and load it from data  
    for( uint32 i = 0, n = entities.size(); i < n; ++i ) 
    {
      Json::Value& objectData       = entities[i];
      const char*  uniqueIdentifier = objectData["identifier"].asCString();
      const char*  templateName     = objectData["type"].asCString();

      const Entity* entity = CreateEntity( templateName, uniqueIdentifier );

      Json::Value& componentData = objectData["components"];
      NE_ASSERT( !componentData.isNull(), "Invalid component node found" )();

      load_components<AnimationComponent >( entity, *this, componentData );
      load_components<AspectComponent >( entity, *this, componentData );
      load_components<BehaviourComponent >( entity, *this, componentData );
      load_components<CollisionComponent >( entity, *this, componentData );
      load_components<MotionComponent >( entity, *this, componentData );
      load_components<PlacementComponent >( entity, *this, componentData );
      load_components<PlacementConstraintComponent >( entity, *this, componentData );
      load_components<ScoreComponent >( entity, *this, componentData );
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

  if( entityTemplate.ContainsComponent("Aspect") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_ASPECT );
  }

  if( entityTemplate.ContainsComponent("Animation") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_ANIMATION );
  }

  if( entityTemplate.ContainsComponent("Behaviour") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_BEHAVIOUR );
  }

  if( entityTemplate.ContainsComponent("Collision") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_COLLISION );
  }

  if( entityTemplate.ContainsComponent("Motion") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_MOTION );
  }

  if( entityTemplate.ContainsComponent( "Particle" ) ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_PARTICLE );
  }

  if( entityTemplate.ContainsComponent("Placement") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_PLACEMENT );
  }

  if( entityTemplate.ContainsComponent("PlacementConstraint") ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_PLACEMENTCONSTRAINT );
  }

  if( entityTemplate.ContainsComponent( "Score" ) ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_SCORE );
  }

  if( entityTemplate.ContainsComponent( "Steering" ) ) {
    AddComponentToEntity( entity, COMPONENT_TYPE_STEERING );
  }
}

int32
Model::AddComponentToEntity( Entity& entity, ComponentType type )
{
  int32 identifier = m_components.GetNextAvailableIndex( type );
  
  if( identifier != Component::UNUSED )
  {
    AddComponentToEntity( entity, type, identifier );  
  }

  return identifier;
}

void 
Model::AddComponentToEntity( Entity& entity, ComponentType type, int32 identifier )
{
  NE_ASSERT( identifier != Component::UNUSED, "Passed identifier is invalid." )( identifier );
  
  entity.AddComponent( type, identifier );

  m_components.AllocateComponent( type, identifier, entity.GetIdentifier() );
}

void 
Model::RemoveComponentFromEntity( Entity& entity, ComponentType type, int32 identifier )
{
  entity.RemoveComponent( type, identifier );

  m_components.ReleaseComponent( type, identifier, entity.GetIdentifier() );
}