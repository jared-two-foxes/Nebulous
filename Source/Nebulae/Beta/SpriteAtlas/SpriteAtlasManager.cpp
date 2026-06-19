
#include "SpriteAtlasManager.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>


using namespace Nebulae;


SpriteAtlasManager::SpriteAtlasManager( FileArchivePtr fileSystem, RenderSystemPtr pRenderSystem )
  : m_fileSystem( fileSystem ), m_renderSystem( pRenderSystem )
{
}


SpriteAtlasManager::~SpriteAtlasManager()
{
  // Delete the resource from the list.
  for ( auto& resource : m_Resources )
  {
    delete resource;
  }
  m_Resources.clear();
}


SpriteAtlas* SpriteAtlasManager::Create( const std::string& name )
{
  SpriteAtlas* res = new SpriteAtlas( name, m_renderSystem );
  res->Load( m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, name ) );

  addImpl( res );

  return res;
}


SpriteAtlas* SpriteAtlasManager::GetByName( const std::string& name ) const
{
  std::vector<SpriteAtlas*>::const_iterator end_it = m_Resources.end();
  for ( std::vector<SpriteAtlas*>::const_iterator it = m_Resources.begin(); it != end_it; ++it )
  {
    if ( ( *it )->GetName().compare( name ) == 0 )
    {
      return ( *it );
    }
  }

  return nullptr;
}


void SpriteAtlasManager::addImpl( SpriteAtlas* res ) { m_Resources.push_back( res ); }
