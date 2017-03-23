
#include "TemplateCache.h"

#include <Model/Core/EntityTemplate.h>

//#include <MurmurHash3.h>

using namespace Nebulae;
using namespace Game;


//-----------------------------------------------------------------------------
// Microsoft Visual Studio

#if defined(_MSC_VER)

//typedef unsigned char    uint8_t;
//typedef unsigned long    uint32_t;
//typedef unsigned __int64 uint64_t;

#define FORCE_INLINE    __forceinline
#define NEVER_INLINE    __declspec(noinline)

#include <stdlib.h>

#define BIG_CONSTANT(x) (x)


//-----------------------------------------------------------------------------
// Other compilers

#else 

#include <stdint.h>

#define FORCE_INLINE inline __attribute__((always_inline))
#define NEVER_INLINE __attribute__((noinline))

#define BIG_CONSTANT(x) (x##LLU)

#endif

void FNV( const void * key, int len, uint32_t seed, void * out )
{
  unsigned int h = seed;

  const uint8_t * data = (const uint8_t*)key;

  h ^= BIG_CONSTANT(2166136261);

  for(int i = 0; i < len; i++)
  {
    h ^= data[i];
    h *= 16777619;
  }

  *(uint32_t*)out = h;
}


TemplateCache::TemplateCache()
///
/// Constructor
///
{

}


void 
TemplateCache::Load( File& stream )
///
/// Loads the template data from the data file.
///
/// @return
///   Nothing.
///
{
  Json::Value root;
  ParseJSON( stream, &root );

  //
  // Iterate each pass and load it from data
  //
  Json::Value::Members templates = root.getMemberNames();
  for( uint32 i = 0, n = templates.size(); i < n; ++i )
  {
    const char*        name         = templates[i].c_str();
    const Json::Value& templateData = root[name];
    uint32_t           identifier   = GetTemplateIdentifier( name );

    std::vector<std::string> components;
    for( uint32 j = 0, m = templateData.size(); j<m; ++j )
    {
      const char* componentName = json_cast<const char*>(templateData[j]);
      components.push_back( componentName );
    }

    EntityTemplate *entityTemplate = new EntityTemplate( identifier, name, components );

    AddTemplate( entityTemplate, true );
  }
}


void 
TemplateCache::AddTemplate( EntityTemplate* entityTemplate, bool overwrite )
///
/// Add an EntityTemplate to the cache.
///
/// @param entityTemplate
///   The template to cache.
///
/// @param overwrite
///   overwrite existing template?
///
/// @return
///   Nothing.
///
{
  int id = entityTemplate->GetId();
  NE_ASSERT( overwrite || !GetTemplateById(id), "Template named '%s' already exists.", entityTemplate->GetName().c_str() )();
  m_templates[id] = entityTemplate;
}


EntityTemplate* 
TemplateCache::GetTemplateById( int id ) const
///
/// Retrieves an EntityTemplate from the cache based on the index.
///
/// @param id
///   The index to use for lookup
///
/// @return
///   The EntityTemplate.
///
/// @todo [jared 27-08-2013]
///   Is it valid to pass in the index?  Ie should other systems know what a templates index/hash is?
///
{
  std::map<int,EntityTemplate*>::const_iterator it = m_templates.find( id );
  if( it != m_templates.end() ) {
    return it->second;
  }
  return NULL;
}


int  
TemplateCache::GetTemplateIdentifier( const char* templateName ) const
///
/// Determines the id used to insert the Template into the map with.
///
/// @param templateName
///   The name of the template.
///
/// @return
///   The insertion id.
///
{
  uint32_t id = -1; 
  FNV( templateName, strlen(templateName), 0, &id );
  return id;
}