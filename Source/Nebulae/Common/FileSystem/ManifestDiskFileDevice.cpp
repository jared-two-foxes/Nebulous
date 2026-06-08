#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/ManifestDiskFileDevice.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/DiskFile.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <windows.h>

namespace Nebulae {

ManifestDiskFileDevice::ManifestDiskFileDevice( const char* rootDirectory )
  : m_fallback( new DiskFileDevice( rootDirectory ) )
  , m_rootDirectory( rootDirectory ? rootDirectory : "" )
  , m_manifestLoaded( false )
{
}

ManifestDiskFileDevice::~ManifestDiskFileDevice()
{
  delete m_fallback;
}

void ManifestDiskFileDevice::LoadManifest()
{
  if( m_manifestLoaded ) return;
  m_manifestLoaded = true;

  // Try 3 sources in order:
  // 1. RUNFILES_MANIFEST_FILE env var (bazel test sets this)
  const wchar_t* manifestEnv = _wgetenv( L"RUNFILES_MANIFEST_FILE" );
  if( manifestEnv != nullptr && manifestEnv[0] != L'\0' ) {
    std::string narrowPath( manifestEnv, manifestEnv + wcslen( manifestEnv ) );
    LoadManifestFromFile( narrowPath, m_manifestMap );
    if( !m_manifestMap.empty() ) return;
  }

  // 2. CWD/MANIFEST (bazel run sets CWD to .runfiles/ directory)
  boost::filesystem::path cwdManifest = boost::filesystem::current_path() / "MANIFEST";
  if( boost::filesystem::exists( cwdManifest ) ) {
    LoadManifestFromFile( cwdManifest.generic_string(), m_manifestMap );
    if( !m_manifestMap.empty() ) return;
  }

  // 3. exe/.runfiles_manifest (next to the exe as build artifact)
  wchar_t exePath[MAX_PATH];
  if( GetModuleFileNameW( NULL, exePath, MAX_PATH ) > 0 ) {
    std::wstring exePathWide( exePath );
    std::wstring manifestPath = exePathWide + L".runfiles_manifest";
    std::string narrowManifestPath( manifestPath.begin(), manifestPath.end() );
    LoadManifestFromFile( narrowManifestPath, m_manifestMap );
  }
}

void ManifestDiskFileDevice::LoadManifestFromFile( const std::string& filepath, std::map<std::string,std::string>& map )
{
  std::ifstream manifestFile( filepath );
  if( !manifestFile.is_open() ) return;

  std::string line;
  while( std::getline( manifestFile, line ) ) {
    // Manifest format: "workspace_relative_path absolute_path" (space-separated)
    size_t spacePos = line.find( ' ' );
    if( spacePos != std::string::npos ) {
      std::string workspaceRelativePath = line.substr( 0, spacePos );
      std::string absolutePath = line.substr( spacePos + 1 );

      // Extract leaf filename from workspace-relative path (after last / or \)
      size_t lastSlash = workspaceRelativePath.find_last_of( "/\\" );
      std::string leafFilename;
      if( lastSlash != std::string::npos ) {
        leafFilename = workspaceRelativePath.substr( lastSlash + 1 );
      } else {
        leafFilename = workspaceRelativePath;
      }

      // Store in map keyed by leaf filename
      if( !leafFilename.empty() && !absolutePath.empty() ) {
        map[leafFilename] = absolutePath;
      }
    }
  }
  manifestFile.close();
}

File* ManifestDiskFileDevice::Open( const std::string& path, FileSystem::Mode mode )
{
  // Load manifest on first access
  LoadManifest();

  // Extract leaf filename from path
  size_t lastSlash = path.find_last_of( "/\\" );
  std::string leafFilename;
  if( lastSlash != std::string::npos ) {
    leafFilename = path.substr( lastSlash + 1 );
  } else {
    leafFilename = path;
  }

  // Check manifest map first
  auto it = m_manifestMap.find( leafFilename );
  if( it != m_manifestMap.end() ) {
    const std::string& absolutePath = it->second;
    if( boost::filesystem::exists( absolutePath ) ) {
      return new DiskFile( absolutePath, false );
    }
  }

  // Fall back to wrapped DiskFileDevice
  File* result = m_fallback->Open( path, mode );
  if( !result ) {
    NE_LOG( "ManifestDiskFileDevice::Open: failed to resolve '%s' (manifest and fallback both failed)", path.c_str() );
  }
  return result;
}

File* ManifestDiskFileDevice::Open( File* file )
{
  return m_fallback->Open( file );
}

void ManifestDiskFileDevice::Close( File* file )
{
  m_fallback->Close( file );
}

std::string ManifestDiskFileDevice::ResolveRunfile( const std::string& filename )
{
  // Static method to resolve a runfile filename to its absolute path
  // Load manifest from 3 sources
  std::map<std::string, std::string> manifestMap;

  // 1. RUNFILES_MANIFEST_FILE env var
  const wchar_t* manifestEnv = _wgetenv( L"RUNFILES_MANIFEST_FILE" );
  if( manifestEnv != nullptr && manifestEnv[0] != L'\0' ) {
    std::string narrowPath( manifestEnv, manifestEnv + wcslen( manifestEnv ) );
    std::ifstream manifestFile( narrowPath );
    if( manifestFile.is_open() ) {
      std::string line;
      while( std::getline( manifestFile, line ) ) {
        size_t spacePos = line.find( ' ' );
        if( spacePos != std::string::npos ) {
          std::string workspaceRelativePath = line.substr( 0, spacePos );
          std::string absolutePath = line.substr( spacePos + 1 );

          size_t lastSlash = workspaceRelativePath.find_last_of( "/\\" );
          std::string leafFilename;
          if( lastSlash != std::string::npos ) {
            leafFilename = workspaceRelativePath.substr( lastSlash + 1 );
          } else {
            leafFilename = workspaceRelativePath;
          }

          if( !leafFilename.empty() && !absolutePath.empty() ) {
            manifestMap[leafFilename] = absolutePath;
          }
        }
      }
      manifestFile.close();
      if( !manifestMap.empty() ) {
        auto it = manifestMap.find( filename );
        if( it != manifestMap.end() && boost::filesystem::exists( it->second ) ) {
          return it->second;
        }
      }
    }
  }

  // 2. CWD/MANIFEST
  boost::filesystem::path cwdManifest = boost::filesystem::current_path() / "MANIFEST";
  if( boost::filesystem::exists( cwdManifest ) ) {
    manifestMap.clear();
    std::ifstream manifestFile( cwdManifest.generic_string() );
    if( manifestFile.is_open() ) {
      std::string line;
      while( std::getline( manifestFile, line ) ) {
        size_t spacePos = line.find( ' ' );
        if( spacePos != std::string::npos ) {
          std::string workspaceRelativePath = line.substr( 0, spacePos );
          std::string absolutePath = line.substr( spacePos + 1 );

          size_t lastSlash = workspaceRelativePath.find_last_of( "/\\" );
          std::string leafFilename;
          if( lastSlash != std::string::npos ) {
            leafFilename = workspaceRelativePath.substr( lastSlash + 1 );
          } else {
            leafFilename = workspaceRelativePath;
          }

          if( !leafFilename.empty() && !absolutePath.empty() ) {
            manifestMap[leafFilename] = absolutePath;
          }
        }
      }
      manifestFile.close();
      if( !manifestMap.empty() ) {
        auto it = manifestMap.find( filename );
        if( it != manifestMap.end() && boost::filesystem::exists( it->second ) ) {
          return it->second;
        }
      }
    }
  }

  // 3. exe/.runfiles_manifest
  wchar_t exePath[MAX_PATH];
  if( GetModuleFileNameW( NULL, exePath, MAX_PATH ) > 0 ) {
    std::wstring exePathWide( exePath );
    std::wstring manifestPath = exePathWide + L".runfiles_manifest";
    std::string narrowManifestPath( manifestPath.begin(), manifestPath.end() );
    manifestMap.clear();
    std::ifstream manifestFile( narrowManifestPath );
    if( manifestFile.is_open() ) {
      std::string line;
      while( std::getline( manifestFile, line ) ) {
        size_t spacePos = line.find( ' ' );
        if( spacePos != std::string::npos ) {
          std::string workspaceRelativePath = line.substr( 0, spacePos );
          std::string absolutePath = line.substr( spacePos + 1 );

          size_t lastSlash = workspaceRelativePath.find_last_of( "/\\" );
          std::string leafFilename;
          if( lastSlash != std::string::npos ) {
            leafFilename = workspaceRelativePath.substr( lastSlash + 1 );
          } else {
            leafFilename = workspaceRelativePath;
          }

          if( !leafFilename.empty() && !absolutePath.empty() ) {
            manifestMap[leafFilename] = absolutePath;
          }
        }
      }
      manifestFile.close();
      if( !manifestMap.empty() ) {
        auto it = manifestMap.find( filename );
        if( it != manifestMap.end() && boost::filesystem::exists( it->second ) ) {
          return it->second;
        }
      }
    }
  }

  // Return empty string if not found
  return "";
}

} // namespace Nebulae
