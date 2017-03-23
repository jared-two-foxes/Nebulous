#ifndef MODEL_MODEL_H__
#define MODEL_MODEL_H__

#include <Nebulae/Common/Common.h>

#include <Model/Core/ComponentCache.h>
#include <Model/Core/EntityPool.h>
#include <Model/Core/TemplateCache.h>

namespace Game {

class Model
///
/// Encapsulates the all of the knowledge and data required describe the universe in which the game exists.
/// This single class wraps all interactions which would result in a change in the game in some significant
/// manner thus any low level game logic should reside here.
///
{
public:
  struct PlayerData
  {
  public:
    std::vector<int32 > m_scores;           ///< List of the highest scores that the user has achieved
    int32               m_maximumDistance;  ///< The furtherest that the player has managed to reach.
  };

private:
  std::shared_ptr<TemplateCache > m_templates;     ///< The known entity type component layouts.
  ComponentCache                  m_components;    ///< Pool of the components that make up the entities.
  EntityPool                      m_entities;      ///< The entities that comprise the world.
  PlayerData                      m_playerData;    ///< Player specific information.  

  public:
    Model( std::shared_ptr<TemplateCache >& templateCache );
    ~Model();

    void    Init();
    void    Clear();
    void    LoadSceneFile( Nebulae::File& stream );
    Entity* CreateEntity( const char* templateName, const char* uniqueName = 0 );
    Entity* FindEntityByName( const char* uniqueName ) const;
    void    DestroyEntity( const Entity* entity );
    
    std::shared_ptr<TemplateCache >  GetTemplates();
    const ComponentCache&            GetComponentCache() const;  
    const std::vector<Entity*>&      GetEntities() const;

    template <class T >
    int32 GetComponents( const Entity* entity, std::vector<T* >* components ) const;

    template <class T>
    T*    AddComponentToEntity( Entity* entity );
    int32 AddComponentToEntity( Entity& entity, ComponentType type );
    void  AddComponentToEntity( Entity& entity, ComponentType type, int32 identifier );

    template <class T>
    void RemoveComponentFromEntity( Entity* entity, int32 identifier );
    void RemoveComponentFromEntity( Entity& entity, ComponentType type, int32 identifier );

  private:
    void ApplyTemplate( Entity& entity, EntityTemplate& entityTemplate );

};

template <class T >
int32 
Model::GetComponents( const Entity* entity, std::vector<T* >* components ) const
{
  NE_ASSERT( entity, "Passed entity was NULL." )();

  std::vector<int32 > identifiers;
  entity->GetLinkedComponentIndices( T::GetType(), &identifiers );

  return m_components.GetComponents( identifiers, components );
}

template <class T >
T* 
Model::AddComponentToEntity( Entity* entity )
{
  int32 identifier = AddComponentToEntity( *entity, T::GetType() );  
   
  return m_components.GetComponent<T >( identifier );
}

}

#endif // MODEL_MODEL_H__