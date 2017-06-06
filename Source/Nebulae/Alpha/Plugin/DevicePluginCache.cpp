//DeviePluginCache.cpp

#include "DevicePluginCache.h"

using Nebulae::DevicePluginCache;
using Nebulae::SharedLibrary;


DevicePluginCache::~DevicePluginCache()
{
  ReleaseAll();
}


void 
DevicePluginCache::AquirePlugin( RenderSystemType type, SharedLibrary* lib )
{
  //Assert that library doesnt already exist.  

  _plugins[type] = lib;
}


void 
DevicePluginCache::ReleasePlugin( RenderSystemType type )
{
  _plugins[type]->Close();
  delete _plugins[type];

  _plugins[type] = nullptr;
}


void 
DevicePluginCache::ReleaseAll()
{
  for ( auto library : _plugins )
  {
    library.second->Close();
    delete library.second;
  }
  
  _plugins.clear();
}


const SharedLibrary* 
DevicePluginCache::GetPlugin( RenderSystemType type ) const
{
	auto it = _plugins.find( type );
	if ( it != _plugins.end() )
	{
		return it->second;
	}

	return nullptr;
}


SharedLibrary* 
DevicePluginCache::GetPlugin( RenderSystemType type )
{
  return _plugins[type];
}