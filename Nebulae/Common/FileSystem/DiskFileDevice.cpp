
#include <Nebulae/Common/Common.h>  
#include <Nebulae/Common/FileSystem/DiskFile.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

using Nebulae::File;
using Nebulae::FileSystem;
using Nebulae::DiskFile;
using Nebulae::DiskFileDevice;

const boost::filesystem::path
CreateAbsolutePath( const std::string& root, const std::string& filename )
{
  boost::filesystem::path fullPath( boost::filesystem::current_path() ); 
  fullPath /= root;
  fullPath /= filename;
  return fullPath;
}

DiskFileDevice::DiskFileDevice( const char* path )
: m_rootDirectory( path )
{}

DiskFileDevice::~DiskFileDevice()
{}

File* 
DiskFileDevice::Open( const std::string& path, FileSystem::Mode mode )
{
  boost::filesystem::path fullPath = CreateAbsolutePath( m_rootDirectory, path );

  return new DiskFile( fullPath.generic_string(), false );
}

File* 
DiskFileDevice::Open( File* file )
{
  NE_ASSERT( false, "Attempting to create a piggyback device for a 'root' file type [Disk]" )();
  return nullptr;
}

void  
DiskFileDevice::Close( File* file )
{
  delete file;
}
