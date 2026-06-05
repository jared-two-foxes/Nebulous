// Alpha.cpp

#include <Nebulae/Alpha/Alpha.h>

#include <Nebulae/Common/Platform/SharedLibrary.h>

#include <Nebulae/Alpha/Plugin/PluginAccessor.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include <cstdlib>
#include <fstream>
#include <string>

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
    int error = -1;

    // First, try RUNFILES_MANIFEST_FILE (Bazel manifest-only runfiles on Windows)
    {
      wchar_t* manifestPath = _wgetenv(L"RUNFILES_MANIFEST_FILE");
      if (manifestPath != nullptr && manifestPath[0] != L'\0') {
        std::ifstream manifest(manifestPath);
        if (manifest.is_open()) {
          // Convert DLL filename to narrow string for manifest search
          std::string searchStr(path.begin(), path.end());
          std::string line;
          while (std::getline(manifest, line)) {
            // Manifest format: "workspace_relative_path absolute_path"
            if (line.find(searchStr) != std::string::npos) {
              size_t spacePos = line.find(' ');
              if (spacePos != std::string::npos) {
                std::string absPath = line.substr(spacePos + 1);
                std::wstring widePath(absPath.begin(), absPath.end());
                error = lib.Open(widePath);
                if (error == 0) break; // Success!
              }
            }
          }
        }
      }
    }

    // If manifest lookup failed, try bare filename
    if (error != 0) {
      error = lib.Open( path );
    }
    
    // If bare filename fails, try runfiles-relative path (Bazel run)
    if (error != 0) {
      std::wstring runfilesPath = L"RenderSystems/GL/" + path;
      error = lib.Open(runfilesPath);
    }

    // If runfiles path also fails, try exe-directory path (manual deployment)
    if (error != 0) {
      wchar_t exePath[MAX_PATH];
      if (GetModuleFileNameW(NULL, exePath, MAX_PATH) > 0) {
        std::wstring exeDir = exePath;
        size_t pos = exeDir.find_last_of(L"\\/");
        if (pos != std::wstring::npos) {
          exeDir = exeDir.substr(0, pos + 1);
          error = lib.Open(exeDir + path);
        }
      }
    }

    // If all attempts failed, return nullptr gracefully instead of crashing
    if (error != 0) {
      return std::shared_ptr<Nebulae::RenderSystem>();
    }
    
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