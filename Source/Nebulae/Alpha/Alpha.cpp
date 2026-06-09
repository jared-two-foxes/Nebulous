// Alpha.cpp

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Common/FileSystem/ManifestDiskFileDevice.h>
#include <Nebulae/Common/Platform/SharedLibrary.h>

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include <cstdlib>
#include <fstream>
#include <string>

std::wstring plugin_names[] = { L"nebulous_gl3", L"nebulous_gl3", L"nebulous_vulcan", L"nebulous_gles_2_0" };

std::shared_ptr<Nebulae::RenderSystem> CreateRenderSystem( RenderSystemType type,
                                                           std::shared_ptr<Nebulae::FileSystem> fileSystem,
                                                           std::shared_ptr<Nebulae::Window> window )
{
  // Set the plugin shared library location
  std::wstring path = plugin_names[type];

#if WIN32
  path += L".dll";
#else
  path += L".so";
#endif

  try
  {
    // Load the shared library
    Nebulae::SharedLibrary lib;
    int error = -1;

    // Use ManifestDiskFileDevice to resolve DLL via Bazel runfiles manifest
    {
      std::string narrowPath( path.begin(), path.end() );
      std::string absPath = Nebulae::ManifestDiskFileDevice::ResolveRunfile( narrowPath );
      if ( !absPath.empty() )
      {
        std::wstring widePath( absPath.begin(), absPath.end() );
        error = lib.Open( widePath );
      }
    }

    // If manifest lookup failed, try bare filename
    if ( error != 0 )
    {
      error = lib.Open( path );
    }

    // If bare filename fails, try runfiles-relative path (Bazel run)
    if ( error != 0 )
    {
      std::wstring runfilesPath = L"RenderSystems/GL/" + path;
      error = lib.Open( runfilesPath );
    }

    // If runfiles path also fails, try exe-directory path (manual deployment)
    if ( error != 0 )
    {
      wchar_t exePath[MAX_PATH];
      if ( GetModuleFileNameW( NULL, exePath, MAX_PATH ) > 0 )
      {
        std::wstring exeDir = exePath;
        size_t pos = exeDir.find_last_of( L"\\/" );
        if ( pos != std::wstring::npos )
        {
          exeDir = exeDir.substr( 0, pos + 1 );
          error = lib.Open( exeDir + path );
        }
      }
    }

    // If all attempts failed, return nullptr gracefully instead of crashing
    if ( error != 0 )
    {
      return std::shared_ptr<Nebulae::RenderSystem>();
    }

    // Get plugin descriptor and exports
    Nebulae::PluginDetails* info;
    lib.Symbol( "exports", reinterpret_cast<void**>( &info ) );

    // API Version checking
    if ( info->apiVersion != NE_PLUGIN_API_VERSION )
      throw std::runtime_error(
        "Plugin ABI version mismatch." ); // Expected %s, got %s.", NE_PLUGIN_API_VERSION, info->apiVersion ) );

    // Set up log callback for the DLL
    Nebulae::LPFNSETLOGCALLBACK SetLogCb;
    lib.Symbol( "SetLogCallback", (void**)&SetLogCb );
    if ( SetLogCb )
    {
      SetLogCb( Nebulae::DllFileCallback );
    }

    Nebulae::LPFNCREATERENDERDEVICE CreateRenderDevice;
    lib.Symbol( "CreateRenderer", (void**)&CreateRenderDevice );

    return std::shared_ptr<Nebulae::RenderSystem>( CreateRenderDevice( fileSystem, window ) );
  }
  catch ( std::exception& exc )
  {
    std::cerr << "Error: " << exc.what() << std::endl;
    assert( 0 );
  }

  return std::shared_ptr<Nebulae::RenderSystem>();
}