// main.cpp

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>

#include <RenderSystem_OGL.h>


NE_RENDERER_PLUGIN( RendererPlugin, "OpenGL 3.0 Renderer", "0.0.1" )


NE_PLUGIN_EXPORT Nebulae::RenderSystem* 
CreateRenderer( std::shared_ptr<Nebulae::FileSystem > fileSystem, std::shared_ptr<Nebulae::Window > window )
{
  return new Nebulae::RenderSystem_OGL( fileSystem, window );
}

NE_PLUGIN_EXPORT void 
DestroyRenderer( Nebulae::RenderSystem* renderer )
{
  delete renderer;
  renderer = nullptr;
}