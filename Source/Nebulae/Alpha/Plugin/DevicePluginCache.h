#ifndef ALPHA_PLUGIN_PLUGINCACHE_H__
#define ALPHA_PLUGIN_PLUGINCACHE_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Common/Platform/SharedLibrary.h>

namespace Nebulae {
  
class DevicePluginCache 
///
/// A storage cache for all registered RenderDevice plugins that are currently
/// known to the system.
///
{
private:
  std::map<RenderSystemType, SharedLibrary* > _plugins;

  public:
    ~DevicePluginCache();

    void AquirePlugin( RenderSystemType type, SharedLibrary* lib );
    void ReleasePlugin( RenderSystemType type );
    void ReleaseAll();

    const SharedLibrary* GetPlugin( RenderSystemType type ) const;
    SharedLibrary* GetPlugin( RenderSystemType type );

};

}

#endif // 