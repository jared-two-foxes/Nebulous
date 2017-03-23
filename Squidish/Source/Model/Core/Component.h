#ifndef __MODEL_CORE_COMPONENT_H__
#define __MODEL_CORE_COMPONENT_H__

#include <Nebulae/Common/Common.h>

#include <Model/Core/ComponentType.h>

namespace Game
{
  
class Component
///
/// Base class for describing various properties of an Entity object.
///
{
public:
  static const int UNUSED = -1;

private:
  int32         m_identifier;
  bool          m_enabled;

  public:
    Component() : m_identifier(UNUSED), m_enabled(true) {}
    
    int32         GetIdentifier() const     { return m_identifier; }
    bool          IsEnabled() const         { return m_enabled; }
    
    void  SetIdentifier( int32 identifier ) { m_identifier = identifier; }
    void  SetEnabled( bool enabled )        { m_enabled = enabled; }
    
};

template <typename T>
struct component_loader
{
  static T load( const Json::Value& value, T& valueOnError )
  {
    return T::no_valid_component_loader_provided_for_type; // The general implementation is deliberately left blank; you cannot cast this type.
  }
};

}

#endif // __MODEL_CORE_COMPONENT_H__