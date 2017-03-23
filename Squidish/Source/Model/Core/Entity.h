#ifndef __CORE_ENTITY_H__
#define __CORE_ENTITY_H__

#include <Nebulae/Common/Common.h>

#include <Model/Core/ComponentType.h>

namespace Game {

class Entity
///
/// Describes an interface for objects part of the game 'Model'.
///
{
private:
  struct ComponentLink
  {
    ComponentType type;       ///< String to describe the type of component.
    int32         identifier; ///< The identifier of the component.
  };

private:
  int32                       m_identifier; ///< A system identifier.
  std::string                 m_uniqueName; ///< An optional human readable name identifer. 
  bool                        m_enabled;    ///< Is the entity currently valid.
  std::vector<ComponentLink > m_components;
  
  public:
    Entity( uint32 identifier );
    virtual     ~Entity();
  
    int32       GetIdentifier() const;
    const char* GetUniqueName() const;
    bool        IsEnabled() const;
    int32       GetLinkedComponentIndices( ComponentType componentType, std::vector<int32 >* indices ) const;
     
    void        SetUniqueName( const char* uniqueName );
    void        SetEnabled( bool enabled );
    bool        AddComponent( ComponentType componentType, int32 identifier );
    void        RemoveComponent( ComponentType componentType, int32 identifier );

  private:
    bool        HasComponentLinkage( ComponentType componentType, int32 identifier ) const;

};

}

#endif // __CORE_ENTITY_H__