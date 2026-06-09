
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFile.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <cstdlib>
#include <fstream>
#include <sstream>

using Nebulae::DiskFile;
using Nebulae::DiskFileDevice;
using Nebulae::File;
using Nebulae::FileSystem;

const boost::filesystem::path CreateAbsolutePath( const std::string& root, const std::string& filename )
{
  boost::filesystem::path fullPath( boost::filesystem::current_path() );
  fullPath /= root;
  fullPath /= filename;
  return fullPath;
}

DiskFileDevice::DiskFileDevice( const char* path ) : m_rootDirectory( path ) {}

DiskFileDevice::~DiskFileDevice() {}

File* DiskFileDevice::Open( const std::string& path, FileSystem::Mode mode )
{
  boost::filesystem::path fullPath = CreateAbsolutePath( m_rootDirectory, path );
  if ( boost::filesystem::exists( fullPath ) )
  {
    return new DiskFile( fullPath.generic_string(), false );
  }
  else
  {
    // Try RUNFILES_MANIFEST_FILE fallback for Bazel manifest-only runfiles
    const wchar_t* manifestPath = _wgetenv( L"RUNFILES_MANIFEST_FILE" );
    if ( manifestPath != nullptr )
    {
      std::ifstream manifestFile( manifestPath );
      if ( manifestFile.is_open() )
      {
        std::string line;
        while ( std::getline( manifestFile, line ) )
        {
          // Format: workspace_relative_path absolute_path (space-separated)
          size_t spacePos = line.find( ' ' );
          if ( spacePos != std::string::npos )
          {
            std::string workspaceRelativePath = line.substr( 0, spacePos );
            std::string absolutePath = line.substr( spacePos + 1 );

            // Check if workspace-relative path ends with the requested filename
            if ( workspaceRelativePath.length() >= path.length() )
            {
              size_t endPos = workspaceRelativePath.length() - path.length();
              if ( workspaceRelativePath.substr( endPos ) == path ||
                   workspaceRelativePath.find( "/" + path ) != std::string::npos ||
                   workspaceRelativePath.find( "\\" + path ) != std::string::npos )
              {
                // Verify the absolute path exists before returning
                if ( boost::filesystem::exists( absolutePath ) )
                {
                  return new DiskFile( absolutePath, false );
                }
              }
            }
          }
        }
        manifestFile.close();
      }
    }

    NE_LOG( "DiskFileDevice::Open: file not found '%s' (tried disk and manifest)", path.c_str() );
    return nullptr;
  }
}

File* DiskFileDevice::Open( File* file )
{
  NE_ASSERT( false, "Attempting to create a piggyback device for a 'root' file type [Disk]" );
  return nullptr;
}

void DiskFileDevice::Close( File* file ) { delete file; }
