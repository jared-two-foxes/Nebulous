#include "SpriteAnimationManager.h"
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimation.h>

using namespace Nebulae;
	

SpriteAnimationManager::SpriteAnimationManager( const FileArchivePtr& fileSystem, const AtlasManagerPtr& atlasManager )
  : m_fileSystem(fileSystem),
    m_atlasManager(atlasManager)
{}

SpriteAnimationManager::~SpriteAnimationManager()
{
	// Delete the resource from the list.
	for( std::size_t i = 0, n = m_resources.size(); i<n; i++) {
		delete m_resources[i];
	}
	m_resources.clear();
}

SpriteAnimation*
SpriteAnimationManager::Create( const std::string& name )
{
	// Create SpriteSheet object.
	SpriteAnimation* res = new SpriteAnimation( name, m_atlasManager );
  res->Load( m_fileSystem->Open(NE_DEFAULT_ROOTDEVICE,name) );
	// Store the resource.
	addImpl( res );
	// return the object
	return res;
}

SpriteAnimation*
SpriteAnimationManager::GetByName( const std::string& name ) const
{
	std::vector< SpriteAnimation* >::const_iterator end_it = m_resources.end();
  for ( std::vector< SpriteAnimation* >::const_iterator it = m_resources.begin(); it != end_it; ++it )
  {
    if ( (*it)->GetName().compare( name ) == 0 )
    {
      return (*it);
    }
  }

  return NULL;
}

void 
SpriteAnimationManager::addImpl( SpriteAnimation* res )
{
	// Add the resource to the resource list
	m_resources.push_back( res );
}
