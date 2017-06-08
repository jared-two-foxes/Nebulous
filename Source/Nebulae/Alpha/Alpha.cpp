// Alpha.cpp

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Common/Platform/SharedLibrary.h>

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

std::wstring plugin_names[]= 
{
  L"nebulous_gl3",
  L"nebulous_gl3",
  L"nebulous_vulcan",
  L"nebulous_gles_2_0"
};

std::shared_ptr<Nebulae::RenderSystem > 
CreateRenderSystem( RenderSystemType type, std::shared_ptr<Nebulae::FileSystem > fileSystem, std::shared_ptr<Nebulae::Window > window )
{
  // Set the plugin shared library location
  std::wstring path = plugin_names[type];

#if WIN32
# ifdef _DEBUG
  path += L"_d.dll";
# else
  path += L".dll";
# endif
#else
  path += L".so";
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