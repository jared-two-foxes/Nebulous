#ifndef NEBULAE_ENTITY_CORE_ENTITYTEMPLATE_H_
#define NEBULAE_ENTITY_CORE_ENTITYTEMPLATE_H_

#include <Nebulae/Common/Common.h>

namespace Sample
{

class Component;

class EntityTemplate
///
/// Describes the components contained by an Entity type.
///
/// @todo [jared.watt 04-10-2013]
///   Change the component identification to something less 'stringy'? :)
///
{
private:
  int m_id;
  std::string m_name;
  std::vector<std::string> m_components;

public:
  EntityTemplate( int id, const char* name, const std::vector<std::string>& components );

  int GetId() const;
  const std::string& GetName() const;

  bool ContainsComponent( const std::string& component ) const;
};

} // namespace Sample

#endif // NEBULAE_ENTITY_CORE_ENTITYTEMPLATE_H_