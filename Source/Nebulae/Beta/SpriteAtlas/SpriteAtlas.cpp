
#include "SpriteAtlas.h"

#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>


using namespace Nebulae;


SpriteAtlas::SpriteAtlas( const std::string& strName, RenderSystemPtr renderDevice )
	: m_name( strName ),
    m_renderDevice( renderDevice )
{}


SpriteAtlas::~SpriteAtlas()
{}


bool 
SpriteAtlas::Load( File* is )
{
  if( NULL == is ) 
  {
    return false;
  }

  Json::Value root;
  if( !ParseJSON(*is, &root) )
  {
    return false;
  }

  Json::Value meta         = root["meta"];
  const char* image_file   = json_cast_with_default<const char* >( meta["image"], "" );
	Json::Value size         = meta["size"];
  Real        image_width  = json_cast_with_default<Real>( size["w"], 0 );
  Real        image_height = json_cast_with_default<Real>( size["h"], 0 );
  
  // Grab the Texture.
	Texture* texture = m_renderDevice->FindTextureByName( image_file );
	if( !texture ) {
		texture = m_renderDevice->CreateTexture( image_file );
  }
	
  NE_ASSERT( texture != nullptr, "Unable to find the image '%s'", image_file )();
  NE_ASSERT( texture != nullptr && texture->GetWidth() == image_width, "Image width does not match specified width in atlas desc file." )();
  NE_ASSERT( texture != nullptr && texture->GetHeight() == image_height, "Image height does not match specified height in atlas desc file." )();

  Json::Value frames = root["frames"];
  NE_ASSERT( frames.isArray(), "Processing texture atlas failed.  Unable to find an array of frames" )();
  if( !frames.isArray() ) 
  {
    return false;
  }
  
  Json::ArrayIndex index = 0u;
  while( frames.isValidIndex(index) )
  {
    ProcessFrame( frames[index], texture );
    index++;
  }
  
	// Done. Return.
	return true;
}


const std::string& 	
SpriteAtlas:: GetName() const 
{ return m_name; }


std::size_t  	
SpriteAtlas::GetModuleCount() const
{ return m_modules.size(); }


SubTexture* 	
SpriteAtlas::FindModuleSubTexture( const std::string& moduleName ) const
///
/// Attempts to find a module struct by name.
///
/// @param moduleName
///		The name of the module to lookup.
///
/// @return
/// 	The module if found.
///
{
	SpriteAtlasModule* pModule = NULL;
	for( std::size_t i = 0, n = m_modules.size(); i<n; ++i ) {
		if( strcmp(m_modules[i]->m_name.c_str(), moduleName.c_str()) == 0 ) {
			pModule = m_modules[i];
			break;
		}
	}
  if( pModule ) {
    NE_ASSERT( pModule->m_subTexture, "" )();
	  return pModule->m_subTexture;
  }
  return NULL;
}


void
SpriteAtlas::ProcessFrame( Json::Value& data, Texture* texture )
{
  const char* name    = json_cast_with_default<const char* >( data["filename"], "" );
  bool        rotated = json_cast_with_default<bool>( data["rotated"], false );
  bool        trimmed = json_cast_with_default<bool>( data["trimmed"], false );

  NE_ASSERT( !rotated, "We dont currently support rotated textures." )();

  Real x, y, dx, dy;
  Json::Value frame = data["frame"];
  std::size_t w = texture->GetWidth(), h = texture->GetHeight();

  x  = json_cast_with_default<Real>( frame["x"], 0 ) / w;
  y  = json_cast_with_default<Real>( frame["y"], 0 ) / h;
  dx = json_cast_with_default<Real>( frame["w"], 0 ) / w;
  dy = json_cast_with_default<Real>( frame["h"], 0 ) / h;

  SpriteAtlasModule* pModule = new SpriteAtlasModule;
  pModule->m_identifier  = m_modules.size();
  pModule->m_name        = name;
  pModule->m_subTexture  = new SubTexture( texture, x, 1.0-(y+dy), x+dx, 1.0-y ); 
  
  m_modules.push_back( pModule );
}