// main.cpp

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>

#include <RenderSystems/GLES2/Win32/RenderSystem_GLES2_Win32.h>


NE_RENDERER_PLUGIN( RendererPlugin, "GLES 2.0 Renderer", "0.0.1" )


NE_PLUGIN_EXPORT Nebulae::RenderSystem* 
CreateRenderer( std::shared_ptr<Nebulae::FileSystem > fileSystem, std::shared_ptr<Nebulae::Window > window )
{
  return new Nebulae::RenderSystem_GLES2_Win32( fileSystem, window );
}

NE_PLUGIN_EXPORT void 
DestroyRenderer( Nebulae::RenderSystem* renderer )
{
  delete renderer;
  renderer = nullptr;
}