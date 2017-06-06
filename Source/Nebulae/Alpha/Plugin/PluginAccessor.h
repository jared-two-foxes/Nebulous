#ifndef NE_ALPHA_PLUGIN_H
#define NE_ALPHA_PLUGIN_H

#include <Nebulae/Common/Common.h>

namespace Nebulae {


class FileSystem;
class RenderSystem;
class Window;

// Define the API version.
// This value is incremented whenever there are ABI breaking changes.
#define NE_PLUGIN_API_VERSION 1
    
#ifdef WIN32
# define NE_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
# define NE_PLUGIN_EXPORT // empty
#endif

// Define a type for the static function pointer.
NE_EXTERN typedef RenderSystem* (*LPFNCREATERENDERDEVICE)( std::shared_ptr<FileSystem >, std::shared_ptr<Window > );
NE_EXTERN typedef void (*LPFNDESTROYRENDERDEVICE)( RenderSystem* );

// Plugin details structure that's exposed to the application.
struct PluginDetails {
  int             apiVersion;
  const char*     fileName;
  const char*     className;
  const char*     pluginName;
  const char*     pluginVersion;
};

#define NE_STANDARD_PLUGIN_STUFF \
    NE_PLUGIN_API_VERSION,       \
    __FILE__

#define NE_RENDERER_PLUGIN( classType, pluginName, pluginVersion )    \
  extern "C" {                                                        \
    NE_PLUGIN_EXPORT Nebulae::PluginDetails exports =             \
    {                                                               \
      NE_STANDARD_PLUGIN_STUFF,                                   \
      #classType,                                                 \
      pluginName,                                                 \
      pluginVersion,                                              \
  };                                                              \
  }

} // namespace Nebulae


#endif // NE_RENDERSYSTEM_PLUGIN_H