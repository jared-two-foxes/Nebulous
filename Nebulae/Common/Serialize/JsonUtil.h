//
// JsonUtil.h
//

#ifndef __NEBULAE_UTILS_JSON_H_INCLUDED__
#define __NEBULAE_UTILS_JSON_H_INCLUDED__

namespace Json { 
  class Value; 
}

namespace Nebulae {

bool ParseJSON( File& file, Json::Value* root );

template <typename T> T json_cast( const Json::Value& value );
template <typename T> T json_cast_with_default( const Json::Value& value, const T& defaultValue );


template <typename T>
struct json_caster
{
  static T to_value( const Json::Value& value, const T& valueOnError )
  {
    return T::no_valid_json_cast_provided_for_type; // The general implementation is deliberately left blank; you cannot cast this type.
  }
};

template <>
struct json_caster<bool>
{
  static bool to_value( const Json::Value& value, bool valueOnError ) 
  { 
    NE_ASSERT( value.isBool(), "Bad json_cast; expected boolean, found %s", value.toStyledString().c_str() )();
    if( value.isBool() )
    {
      return value.asBool();
    }
    else
    {
      return valueOnError; 
    }
  }
};

template <>
struct json_caster<int>
{
  static int to_value( const Json::Value& value, int valueOnError ) 
  { 
    NE_ASSERT( value.isInt(), "Bad json_cast; expected integer, found %s", value.toStyledString().c_str())();
    if( value.isInt() )
    {
      return value.asInt();
    }
    else
    {
      return valueOnError; 
    }
  }
};

template <>
struct json_caster<uint32>
{
  static uint32 to_value( const Json::Value& value, uint32 valueOnError ) 
  { 
    NE_ASSERT( value.isUInt() || value.isInt(), "Bad json_cast; expected unsigned integer, got '%s'", value.toStyledString().c_str() )();
    NE_ASSERT( value.asInt() >= 0, "Bad json_cast; expected unsigned integer, got signed integer '%d'", value.asInt() )();

    if( value.isUInt() )
    {
      return value.asUInt(); 
    }
    else
    {
      return valueOnError;
    }
  }
};

template <>
struct json_caster<Real>
{
  static float to_value( const Json::Value& value, Real valueOnError ) 
  { 
    NE_ASSERT( value.isNumeric(), "Bad json_cast; expected float, found '%s'", value.toStyledString().c_str() )();
    NE_ASSERT( value.asDouble() <= std::numeric_limits<float>::max(), "Bad json_cast; real value is out of range for a float, value may not be larger than '%f'", std::numeric_limits<float>::max() )();

    if( value.isNumeric() && value.asDouble() <= std::numeric_limits<float>::max() )
    {
      return static_cast<float>( value.asDouble() );
    }
    else
    {
      return valueOnError;
    }
  }
};

template <>
struct json_caster<const char*>
{
  static const char* to_value( const Json::Value& value, const char* valueOnError ) 
  { 
    NE_ASSERT( value.isString(), "Bad json_cast; expected string, got '%s'", value.toStyledString().c_str() )();
    const char* string = value.asCString();
    return string ? string : valueOnError;
  }
};

template<>
struct json_caster<std::string>
{
  static std::string to_value( const Json::Value& value, const std::string& valueOnError ) 
  { 
    NE_ASSERT( value.isString(), "Bad json_cast; expected string, got '%s'", value.toStyledString().c_str() )();
    const char* string = value.asCString();
    return string ? string : valueOnError;
  }
};

template <>
struct json_caster<Vector2>
{
  static Vector2 to_value( const Json::Value& value, const Vector2& valueOnError )
  {
    NE_ASSERT( value.isArray() , "Bad json_cast; expected array, found '%s'", value.toStyledString().c_str() )();
    NE_ASSERT( value.size() <= 2, "Invalid vector array in json data. Expected 2 dimensions, found %d", value.size() )();

    if( value.isArray() && value.size() <= 2 )
    {
      ///@todo Add type and size checking for each element in the array; [ 43, "foobar" ] is not a valid vector.
      float x = (value.size() >= 1) ? static_cast<float>(value[0u].asDouble()) : 0;
      float y = (value.size() >= 2) ? static_cast<float>(value[1].asDouble()) : 0;

      return Vector2( x, y );
    }
    else
    {
      return valueOnError;
    }
  }
};

template <>
struct json_caster<Vector4>
{
  static Vector4 to_value( const Json::Value& value, const Vector4& valueOnError ) 
  {
    NE_ASSERT( value.isArray(), "Bad json_cast; expected array, found '%s'", value.toStyledString().c_str() )();
    NE_ASSERT( value.size() <= 4, "Invalid vector array in json data. We do not support %d-dimensional vectors", value.size() )();

    if( value.size() <= 4 )
    {
      float x = (value.size() >= 1) ? static_cast<float>(value[0u].asDouble()) : 0;
      float y = (value.size() >= 2) ? static_cast<float>(value[1].asDouble()) : 0;
      float z = (value.size() >= 3) ? static_cast<float>(value[2].asDouble()) : 0;
      float w = (value.size() == 4) ? static_cast<float>(value[3].asDouble()) : 1;

      return Vector4( x, y, z, w );
    }
    else
    {
      return valueOnError;
    }
  }
};

template <>
struct json_caster<Quaternion>
{
  static Quaternion to_value( const Json::Value& value, const Quaternion& valueOnError ) 
  {
    NE_ASSERT( value.isArray(), "Bad json_cast; expected array, found '%s'", value.toStyledString().c_str() )();
    NE_ASSERT( value.size() <= 4, "Invalid vector array in json data. We do not support %d-dimensional vectors", value.size() )();

    if( value.size() <= 4 )
    {
      float x = (value.size() >= 1) ? static_cast<float>(value[0u].asDouble()) : 0;
      float y = (value.size() >= 2) ? static_cast<float>(value[1].asDouble()) : 0;
      float z = (value.size() >= 3) ? static_cast<float>(value[2].asDouble()) : 0;
      float w = (value.size() == 4) ? static_cast<float>(value[3].asDouble()) : 1;

      return Quaternion( x, y, z, w );
    }
    else
    {
      return valueOnError;
    }
  }
};

template <typename T>
struct json_caster<std::vector<T> >
{
  static std::vector<T> to_value( const Json::Value& value, const std::vector<T>& valueOnError )
  {
    NE_ASSERT( value.isArray(), "Bad json_cast: expected array, found '%s'", value.toStyledString().c_str() )();

    if( value.isArray() )
    {
      std::vector<T> array;
      array.reserve( value.size() );

      for( uint32 i = 0; i < value.size(); ++i )
      {
        array.push_back( json_cast<T>(value[i]) );
      }

      return array;
    }
    else
    {
      return valueOnError;
    }
  }
};

}


template <typename T> T
Nebulae::json_cast( const Json::Value& value ) 
{
  return Nebulae::json_caster<T>::to_value( value, T() );
}


template <typename T> T
Nebulae::json_cast_with_default(const Json::Value& value, const T& defaultValue) 
{
  if( value.isNull() ) 
  {
    return defaultValue;
  } 
  else 
  {
    return Nebulae::json_caster<T>::to_value( value, defaultValue );
  }
}

#endif // __NEBULAE_UTILS_JSON_H_INCLUDED__
