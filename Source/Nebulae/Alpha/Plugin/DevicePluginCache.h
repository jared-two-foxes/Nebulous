#ifndef NEBULAE_ALPHA_PLUGIN_DEVICEPLUGINCACHE_H_
#define NEBULAE_ALPHA_PLUGIN_DEVICEPLUGINCACHE_H_

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Common/Common.h>

#include <Nebulae/Common/Platform/SharedLibrary.h>

namespace Nebulae
{

class DevicePluginCache
///
/// A storage cache for all registered RenderDevice plugins that are currently
/// known to the system.
///
{
private:
  std::map<RenderSystemType, SharedLibrary*> _plugins;

public:
  ~DevicePluginCache();

  void AquirePlugin( RenderSystemType type, SharedLibrary* lib );
  void ReleasePlugin( RenderSystemType type );
  void ReleaseAll();

  const SharedLibrary* GetPlugin( RenderSystemType type ) const;
  SharedLibrary* GetPlugin( RenderSystemType type );
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_PLUGIN_DEVICEPLUGINCACHE_H_