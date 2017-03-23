// Alpha.cpp

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Common/Platform/SharedLibrary.h>

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

std::string plugin_names[]= 
{
  "opengl_3",
  "opengl_3",
  "vulcan",
  "gles_2_0"
};

std::shared_ptr<Nebulae::RenderSystem > 
CreateRenderSystem( RenderSystemType type, std::shared_ptr<Nebulae::FileSystem > fileSystem, std::shared_ptr<Nebulae::Window > window )
{
  // Set the plugin shared library location
  std::string path = plugin_names[type];

#if WIN32
# ifdef _DEBUG
  path += "_d.dll";
# else
  path += ".dll";
# endif
#else
  path += ".so";
#endif

  try 
  {
    // Load the shared library        
    Nebulae::SharedLibrary lib;
    int error = lib.Open( path );
    
    // Get plugin descriptor and exports
    Nebulae::PluginDetails* info;
    lib.Symbol( "exports", reinterpret_cast<void**>(&info) );
    
    // API Version checking 
     if ( info->apiVersion != NE_PLUGIN_API_VERSION )
         throw std::runtime_error( "Plugin ABI version mismatch." ); //Expected %s, got %s.", NE_PLUGIN_API_VERSION, info->apiVersion ) );

		Nebulae::LPFNCREATERENDERDEVICE CreateRenderDevice;
		lib.Symbol( "CreateRenderer", (void**)&CreateRenderDevice );
  
		return std::shared_ptr<Nebulae::RenderSystem >( CreateRenderDevice( fileSystem, window ) );
  }
  catch ( std::exception& exc ) 
  {
    std::cerr << "Error: " << exc.what() << std::endl;
    assert( 0 );
  }

  return std::shared_ptr<Nebulae::RenderSystem >();
}