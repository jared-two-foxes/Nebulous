
#include "WidgetSerializer.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Layout.h>
#include <Nebulae/Beta/Gui/LayoutAssistant.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>
#include <Nebulae/Beta/Gui/Control/Control.h>
#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <Nebulae/Beta/Gui/Control/EditControl.h>
#include <Nebulae/Beta/Gui/Control/Slider.h>
#include <Nebulae/Beta/Gui/Control/StaticGraphic.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>

using Nebulae::File;
using Nebulae::SpriteAtlas;
using Nebulae::SpriteAtlasManager;
using Nebulae::Widget;
using Nebulae::WidgetSerializer;
using Nebulae::Window;


WidgetSerializer::WidgetSerializer( GuiManagerPtr& gui, const RenderSystemPtr& renderDevice, const SpriteAtlasManagerPtr& atlasManager )
: m_gui(gui),
  m_renderDevice(renderDevice),
  m_atlasManager(atlasManager),
  m_rootDeviceSize(640,960)
{}

void    
WidgetSerializer::SetRootDimensions( const Nebulae::Point& point )
{
  m_rootDeviceSize = point;
}

Widget* 
WidgetSerializer::ReadIn( File& stream )
///
/// Accepts a data stream and attempts to read from the data stream a gui element layout creating each
/// gui element it finds.  It assumes that the layout will be in a 'json' format.
///
///	@param stream
///   The data stream
///
/// @return
///   Will return null if a valid layout is not found or the 'root' element of the created layout.
///
{
  Json::Value root;
  bool success = ParseJSON( stream, &root );

  if( success ) {
    Widget* widget = LoadElement( root, m_gui );

    Point                    resolution;
    std::shared_ptr<Window > window = m_renderDevice->GetWindow();
    
    resolution.x = window->GetWidth();
    resolution.y = window->GetHeight();

    LayoutAssistant layoutAssistant( m_rootDeviceSize.x, m_rootDeviceSize.y );
    layoutAssistant.SetResolution( resolution );
    layoutAssistant.Process( widget );

    m_gui->Register( widget );

    return widget;
  }

  return NULL;	
}

Widget* 
WidgetSerializer::LoadElement( const Json::Value& elementData, std::shared_ptr<Nebulae::GuiManager > gui )
///
/// Interprets the contents of a json element node converting it to a widget.  Will, if required,
/// recursively iterate through any "child" ui elements and create each child widget. Assumes that the
/// Json::Value is a correctly formed Ui element description.
///
/// @param elementData
///   The node containing the description.
///
/// @param factory
///   Factory class that creates ui elements.
///
/// @param state
///   The object to use as context for button clicks.
///
/// @return
///   Nothing. 
///
{
  const std::shared_ptr<Nebulae::WidgetFactory> factory = gui->GetWidgetFactory();

  const char*        name           = json_cast_with_default<const char*>( elementData["name"], "" );
  const char*        type           = json_cast_with_default<const char*>( elementData["type"], "" );
  int                width          = json_cast_with_default<int>( elementData["width"], -1 );
  int                height         = json_cast_with_default<int>( elementData["height"], -1 );
  const char*        fontType       = json_cast_with_default<const char* >( elementData["font-type"], "grobold" );
  int                fontSize       = json_cast_with_default<int >( elementData["font-size"], 24 );
  const char*        fontFormat     = json_cast_with_default<const char*>( elementData["font-align"], "none" );
  Vector4            position       = json_cast_with_default<Vector4>( elementData["position"], Vector4(0,0,0,0) );
  const char*        image_name     = json_cast_with_default<const char*>( elementData["image"], "" );
  const char*        atlas_name     = json_cast_with_default<const char*>( elementData["atlas"], "" );
  const char*        frame_name     = json_cast_with_default<const char*>( elementData["frame"], "" );
  float              rotation       = json_cast_with_default<float>( elementData["rotation"], 0 );
  bool               visible        = json_cast_with_default<bool>( elementData["visible"], true );
  bool               enabled        = json_cast_with_default<bool>( elementData["enabled"], true );
  const char*        text           = json_cast_with_default<const char*>( elementData["text"], "" ); 
  const Json::Value& childArrayData = elementData["children"];

  NE_ASSERT(type, "Type value not present")();

  SubTexture* image = NULL;
  Control*    element = NULL;
  
  if( strlen(atlas_name) > 0 && strlen(frame_name) ) 
  {
    SpriteAtlas* atlas = m_atlasManager->GetByName( atlas_name );
    if( !atlas )
    {
      atlas = m_atlasManager->Create( atlas_name );
    }

    NE_ASSERT( atlas, "Unable to find an atlas named %s", atlas_name )();
    if( atlas )
    {
      image = atlas->FindModuleSubTexture( frame_name );
    }
  }
  else if( strlen(image_name) > 0 ) 
  {
    Texture* texture = m_renderDevice->FindTextureByName( image_name );
    if( !texture )
    {
      texture = m_renderDevice->CreateTexture( image_name );
    }

    NE_ASSERT( texture, "Unable to find a texture named %s", image_name )();
    if( texture )
    {
      image = new SubTexture( texture, 0, 0, 1.0f, 1.0f );
    }
  }
  
  Flags<TextFormat > text_format = FORMAT_NONE;
  if( strstr(fontFormat, "left") != nullptr )
  {
    text_format |= FORMAT_LEFT;
  }
  else if( strstr(fontFormat, "center") != nullptr )
  {
    text_format |= FORMAT_CENTER;
  }
  else if( strstr(fontFormat, "right") != nullptr )
  {
    text_format |= FORMAT_RIGHT;
  }

  if( strstr(fontFormat, "top") != nullptr )
  {
    text_format |= FORMAT_TOP;
  }
  else if( strstr(fontFormat, "vcenter") != nullptr )
  {
    text_format |= FORMAT_VCENTER;
  }
  else if( strstr(fontFormat, "bottom") != nullptr )
  {
    text_format |= FORMAT_BOTTOM;
  }

  std::shared_ptr<Nebulae::Font > font;
  if( strlen(fontType) > 0 )
  {
    char fontNameBuffer[128];
    sprintf( fontNameBuffer, "fonts/%s.ttf", fontType );
   font = gui->GetFont( fontNameBuffer, fontSize );
  }

  if( strcmp("button", type) == 0 ) 
  { 
    Nebulae::ButtonControl*     button      = factory->CreateButton( static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y)), width, height, text, font, CLR_WHITE );
    
    Nebulae::SubTexture* sub_texture = image;
    if( sub_texture != nullptr )
    {
      //@todo - handle seperate images for pressed & unpressed states.
      button->SetUnpressedGraphic( *sub_texture );
      button->SetPressedGraphic( *sub_texture );
    }
    
    element = button; 
  } 
  else if( strcmp("text", type) == 0 ) 
  {
    element = factory->CreateTextControl( static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y)), width, height, text, font, CLR_BLACK, text_format );
  } 
  else if( strcmp("edit", type) == 0 )
  {
    element = factory->CreateEditControl( static_cast<int32>(std::floor(position.x)), static_cast<int32>(std::floor(position.y)), width, text, font, CLR_GRAY, CLR_WHITE, CLR_BLACK );
  }
  else if( strcmp("image", type) == 0 ) 
  {
    element = factory->CreateStaticGraphic( static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y)), width, height, image );
  } 
  else if( strcmp("slider", type) == 0 ) 
  {
    element = factory->CreateSlider( static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y)), width, height, 0.0f, 1.0f, height );  
  } 
  else if( strcmp("panel", type) == 0 ) 
  {
    element = new Nebulae::Control( *factory, static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y)), width, height, 0 );
  } 
  else 
  {
  	NE_ASSERT( false, "Unknown widget type detected" )();
  	return NULL;
  }
  
  element->SetName( name );
  element->SetDisabled( !enabled );
  if( visible ) 
  {
    element->Show();
  } 
  else
  {
    element->Hide();
  }

  if( element ) 
  {
    for( int i = 0, n = childArrayData.size(); i<n; ++i ) 
    {
      const Json::Value& childData = childArrayData[i];

      Widget* child = LoadElement( childData, gui );
      if( child ) 
      {
        element->AttachChild( child );
      }
    }
  }

  return element;
}