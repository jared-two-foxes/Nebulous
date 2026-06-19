#ifndef NEBULAE_ENTITY_CORE_COMPONENT_H_
#define NEBULAE_ENTITY_CORE_COMPONENT_H_

#include <Nebulae/Common/Common.h>

#include "ComponentType.h"

namespace Sample
{

class Component
///
/// Base class for describing various properties of an Entity object.
///
{
public:
  static const int UNUSED = -1;

private:
  int32 m_identifier;
  bool m_enabled;

public:
  Component() : m_identifier( UNUSED ), m_enabled( true ) {}

  int32 GetIdentifier() const { return m_identifier; }
  bool IsEnabled() const { return m_enabled; }

  void SetIdentifier( int32 identifier ) { m_identifier = identifier; }
  void SetEnabled( bool enabled ) { m_enabled = enabled; }
};

} // namespace Sample

#endif // NEBULAE_ENTITY_CORE_COMPONENT_H_