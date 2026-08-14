
#include "FontManager.h"
#include "Font.h"

#include <Nebulae/Alpha/Alpha.h>


using namespace Nebulae;


FontManager::FontManager() {}


FontManager::~FontManager()
{
  for ( auto& m_Resource : m_Resources )
  {
    delete m_Resource;
  }
  m_Resources.clear();
}


Font* FontManager::Create( const std::string& name, int iFontSize )
{
  Font* res = new Font( name, iFontSize );
  if ( res )
  {
    addImpl( res );
  }
  return res;
}


Font* FontManager::GetByName( const std::string& name ) const
{
  std::vector<Font*>::const_iterator end_it = m_Resources.end();
  for ( std::vector<Font*>::const_iterator it = m_Resources.begin(); it != end_it; ++it )
  {
    if ( ( *it )->FontName().compare( name ) == 0 )
    {
      return ( *it );
    }
  }

  return nullptr;
}


void FontManager::addImpl( Font* res )
{
  // Add the resource to the resource list
  m_Resources.push_back( res );
}
