#ifndef MODEL_MODEL_H__
#define MODEL_MODEL_H__

#include <Nebulae/Common/Common.h>

#include "Core/ComponentCache.h"
#include "Core/EntityPool.h"
#include "Core/TemplateCache.h"

namespace Sample {

class Model
///
/// Encapsulates the all of the knowledge and data required describe the universe in which the game exists.
/// This single class wraps all interactions which would result in a change in the game in some significant
/// manner thus any low level game logic should reside here.
///
{
private:
  std::shared_ptr<TemplateCache > m_templates;  ///< The known entity type component layouts.
  ComponentCache                  m_components; ///< Pool of the components that make up the entities.
  EntityPool                      m_entities;   ///< The entities that comprise the world.
  
  public:
    Model( std::shared_ptr<TemplateCache>& templateCache );
    ~Model();

    void    Init();
    void    Clear();
    void    LoadSceneFile( Nebulae::File& stream );
    Entity* CreateEntity( const char* templateName, const char* uniqueName = 0 );
    Entity* FindEntityByName( const char* uniqueName ) const;
    void    DestroyEntity( Entity* entity );

    std::shared_ptr<TemplateCache >  GetTemplates();
    ComponentCache&                  GetComponentCache();  
    const std::vector<Entity*>&      GetEntities() const;

    template <class T >
    int32 GetComponents( const Entity* entity, std::vector<T* >* components ) const;

  private:
    void ApplyTemplate( Entity& entity, EntityTemplate& entityTemplate );

};

template <class T >
int32 
Model::GetComponents( const Entity* entity, std::vector<T* >* components ) const
{
  std::vector<int32 > identifiers;
  entity->GetLinkedComponentIndices( T::GetType(), &identifiers );

  return m_components.GetComponents( identifiers, components );
}

}

#endif // MODEL_MODEL_H__