#ifndef __MODEL_CORE_TEMPLATECACHE_H__
#define __MODEL_CORE_TEMPLATECACHE_H__

#include <Nebulae/Common/Common.h>

namespace Game
{

class EntityTemplate;

class TemplateCache
///
/// Storage for the EntityTemplate objects.  Cache is responsible only for the storage and
/// lifetime of the objects but not there creation.
///
{
private: 
  std::map<int, EntityTemplate*> m_templates; ///< EntityTemplates by type.

  public:
    TemplateCache();

    void Load( Nebulae::File& stream );
    void AddTemplate( EntityTemplate* entityTemplate, bool overwrite = false );

    EntityTemplate* GetTemplateById( int id ) const;
    int             GetTemplateIdentifier( const char* templateName ) const;
    
};

}

#endif // __MODEL_CORE_TEMPLATECACHE_H__