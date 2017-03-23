
#include "PropertyArray.h"


using namespace Nebulae;
using namespace Game;


PropertyArray::PropertyArray()
///
/// Default Constructor
///
{}

PropertyArray::~PropertyArray()
///
/// Default Destructor
///
{}

bool 
PropertyArray::Load( File& file ) {
  Json::Value root;

  bool success = ParseJSON( file, &root );
  if( success ) {
    Load( root );
    return true;
  }
  return false;
}

void 
PropertyArray::Load( const Json::Value& json ) {
  const Json::Value::Members& names = json.getMemberNames();
  for( Json::Value::Members::const_iterator i = names.begin(); i != names.end(); ++i ) {
    const char*        name  = i->c_str();
    const Json::Value& value = json[*i];
    
    if( value.isBool() ) {
      m_properties.insert( std::make_pair(name,boost::any(value.asBool())) );
    } else if( value.isInt() ) {
      m_properties.insert( std::make_pair(name,boost::any(value.asInt())) );
    } else if( value.isNumeric() ) {
      m_properties.insert( std::make_pair(name,boost::any(value.asDouble())) );
    } else if( value.isString() ) {
      m_properties.insert( std::make_pair(name,boost::any(std::string(value.asCString()))) );
    } else if( value.isArray() && value.size() > 0 ) {
      if( value.size() <= 2 && value[0].isDouble() ) {
        Vector2 vector = json_cast_with_default<Vector2 >(value, Vector2(0,0));
        m_properties.insert( std::make_pair(name,boost::any(vector)) );
      } else if( value.size() <= 4 && value[0].isDouble() ) {
        Vector4 vector = json_cast_with_default<Vector4 >(value, Vector4(0,0,0,0));
        m_properties.insert( std::make_pair(name,boost::any(vector)) );
      } else if( value[0].isDouble() ) {
        std::vector<float> array = json_cast_with_default<std::vector<float > >(value, std::vector<float>() );
        m_properties.insert( std::make_pair(name,boost::any(array)) );
      } else if( value[0].isString() ) {
        std::vector<std::string> array = json_cast_with_default<std::vector<std::string > >(value, std::vector<std::string>() );
        m_properties.insert( std::make_pair(name,boost::any(array)) );
      } else {
        assert(0);
      }
    }
  }
}

void 
PropertyArray::Put( const std::string& name, const boost::any& value )
///
/// Attempt to add a new property or modify an existing property.
///
/// @param name
///   The name of the property.
///
/// @param value
///   The value to associate with the name.
///
/// @return
///   Nothing.
///
{
  std::map<std::string,boost::any>::iterator it = m_properties.find( name );
  if( it != m_properties.end() ) {
    it->second = value;
  } else {
    m_properties[name] = value;
  }
}

bool
PropertyArray::Find( const std::string& name, boost::any& out ) const
///
/// Looks for a value associated with a given name.
///
/// @param name
///   The name of the value.
///
/// @return
///   The value associated with this name.
///
{
  std::map<std::string,boost::any>::const_iterator it = m_properties.find( name );
  if( it != m_properties.end() ) {
    out = it->second;
    return true;
  }
  
  return false;
}
