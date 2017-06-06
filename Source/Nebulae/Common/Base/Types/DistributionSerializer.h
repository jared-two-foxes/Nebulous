#ifndef __NEBULAE_COMMON_BASE_DISTRIBUTIONSERIALIZER_H__
#define __NEBULAE_COMMON_BASE_DISTRIBUTIONSERIALIZER_H__

namespace Nebulae {

class DistributionSerializer
{
public:
  template <class T >
  void Serialize( const Json::Value& value, Distribution<T >& distribution );

};


template <>
void
DistributionSerializer::Serialize<Vector4 >( const Json::Value& value, Distribution<Vector4 >& distribution )
{
  if( !value["x"].isArray() )
  {
    Vector4 constant;
    constant.x = json_cast_with_default( value["x"], 0.0f );    
    constant.y = json_cast_with_default( value["y"], 0.0f );   
    constant.z = json_cast_with_default( value["z"], 0.0f );   
    constant.w = json_cast_with_default( value["w"], 0.0f );   
  }
  else
  {
    std::vector<Vector4 > points;
    
    for( std::size_t i = 0; i < value["x"].size(); ++i )
    {
      Vector4 point;
      point.x = json_cast_with_default( value["x"][(int)i], 0.0f );    
      point.y = json_cast_with_default( value["y"][(int)i], 0.0f );   
      point.z = json_cast_with_default( value["z"][(int)i], 0.0f );   
      point.w = json_cast_with_default( value["w"][(int)i], 0.0f );   

      points.push_back( point );
    }

    NE_ASSERT( points.size() > 0, "An invalid number of points were streamed into the points array" )( points.size() );

    if( points.size() == 1 )
    {
      distribution.SetConstant( points[0] );
    }
    else if( points.size() == 2 )
    {
      distribution.SetUniform( points[0], points[1] );
    }
    else // points.size() > 2
    {
      NE_ASSERT( false, "Curve types not yet supported" )();
    }
  }
}

} // Nebulae

#endif // __NEBULAE_COMMON_BASE_DISTRIBUTIONSERIALIZER_H__