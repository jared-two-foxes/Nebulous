#ifndef MODEL_COMPONENT_ASPECTCOMPONENT_H__
#define MODEL_COMPONENT_ASPECTCOMPONENT_H__

#include <Model/Core/Component.h>

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>

namespace Game
{

class AspectComponent : public Component
///
/// Component which describes a static image.
///
{
public:
  enum Alignment
  {
    TOP_LEFT, 
    TOP, 
    TOP_RIGHT, 
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT, 
    BOTTOM_LEFT, 
    BOTTOM, 
    BOTTOM_RIGHT
  };

public:
  std::string atlasName;
  std::string frameName;
  float       width;
  float       height;
  int         flags;
  Alignment   alignment;  

  public:
    AspectComponent() : Component()
      , atlasName( "" )
      , frameName( "" )
      , width( 1.f )
      , height( 1.f )
      , flags( 0 )
      , alignment( BOTTOM_LEFT )
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_ASPECT; }

    void GetRect( const Vector4& position, Vector4* rect )
    {
      switch( alignment )
      {
        case TOP_LEFT:
          *rect = Vector4( position.x, position.y, position.x + width, position.y - height );
          break;

        case TOP:
          *rect = Vector4( position.x - width * 0.5f, position.y, position.x + width * 0.5f, position.y - height );
          break;

        case TOP_RIGHT:
          *rect = Vector4( position.x - width, position.y, position.x, position.y - height );
          break;

        case CENTER_LEFT:
          *rect = Vector4( position.x, position.y + height * 0.5f, position.x + width, position.y - height * 0.5f );
          break;

        case CENTER:
          *rect = Vector4( position.x - width * 0.5f, position.y + height * 0.5f, position.x + width * 0.5f, position.y - height * 0.5f );
          break;

        case CENTER_RIGHT:
          *rect = Vector4( position.x - width, position.y + height * 0.5f, position.x, position.y - height * 0.5f );
          break;

        case BOTTOM_LEFT:
          *rect = Vector4( position.x, position.y + height, position.x + width, position.y );
          break;

        case BOTTOM:
          *rect = Vector4( position.x - width * 0.5f, position.y + height, position.x + width * 0.5f, position.y );
          break;

        case BOTTOM_RIGHT:
          *rect = Vector4( position.x - width,position.y + height, position.x, position.y );
          break;
      }
    }

};


template <>
struct component_loader<AspectComponent>
{
  static const char* component_name() { return "aspect"; }

  static AspectComponent& load( const Json::Value& value, AspectComponent& component ) 
  { 
    component.atlasName = Nebulae::json_cast_with_default<const char*>( value["atlas"],  "" );
    component.frameName = Nebulae::json_cast_with_default<const char*>( value["frame"],  "" );
    component.height    = Nebulae::json_cast_with_default<float>(       value["height"], 1.0f );
    component.width     = Nebulae::json_cast_with_default<float>(       value["width"],  1.0f );
    
    if( value["flags"].isInt() ) {
      component.flags     = Nebulae::json_cast_with_default<int>( value["flags"], Nebulae::SAF_NONE );
    } else if( value["flags"].isString() ) {
      std::string location = Nebulae::json_cast_with_default<std::string >( value["flags"], "" ); 
	  //location = std::to_upper( location );
	  if( location == "LOCATION_LEFT" ) {
        component.flags = Nebulae::SAF_FLIPX;
      } else if( location == "LOCATION_CENTER" ) {
        component.flags = Nebulae::SAF_NONE;
      } else if( location == "LOCATION_RIGHT" ) {
        component.flags = Nebulae::SAF_NONE;
      } else {
        NE_ASSERT( false, "Unknown flag type" )( location );
      }
    }

	std::string align_value = Nebulae::json_cast_with_default<std::string >( value["alignment"], "" );
	//value = std::to_upper( value ); //< ??
	if( align_value == "BOTTOM_LEFT" ) {
		component.alignment = AspectComponent::BOTTOM_LEFT;
	} else if( align_value == "BOTTOM" ) {
		component.alignment = AspectComponent::BOTTOM;
	} else if( align_value == "BOTTOM_RIGHT" ) {
		component.alignment = AspectComponent::BOTTOM_RIGHT;
	} else if( align_value == "CENTER_LEFT" ) {
		component.alignment = AspectComponent::CENTER_LEFT;
	} else if( align_value == "CENTER" ) {
		component.alignment = AspectComponent::CENTER;
	} else if( align_value == "CENTER_RIGHT" ) {
		component.alignment = AspectComponent::CENTER_RIGHT;
	} else if( align_value == "TOP_LEFT" ) {
		component.alignment = AspectComponent::TOP_LEFT;
	} else if( align_value == "TOP" ) {
		component.alignment = AspectComponent::TOP;
	} else if( align_value == "TOP_RIGHT" ) {
		component.alignment = AspectComponent::TOP_RIGHT;
	} else if( align_value.length() > 0 ) {
		NE_ASSERT( false, "Unknown alignment type" )( align_value );
	}
		
    return component; 
  }
};

}

#endif // MODEL_COMPONENT_ASPECTCOMPONENT_H__