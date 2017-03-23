
#include <Nebulae/Common/Common.h>  
#include <Nebulae/Common/FileSystem/ZipFile.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

using Nebulae::File;
using Nebulae::FileSystem;
using Nebulae::ZipFile;
using Nebulae::ZipFileDevice;

#define CASESENSITIVITY   0

ZipFileDevice::ZipFileDevice( const char* path, const char* archiveName, const char* extention )
: m_rootDirectory( path )
{
  std::string filename( archiveName );
  filename += ".";
  filename += extention;

  boost::filesystem::path fullPath( boost::filesystem::current_path() ); 
  fullPath /= path;
  fullPath /= filename;
  
  m_stream = unzOpen64( fullPath.c_str() );
}

ZipFileDevice::~ZipFileDevice()
{
  unzClose( m_stream );
}

File* 
ZipFileDevice::Open( const std::string& path, FileSystem::Mode mode )
{
  int err = unzLocateFile( m_stream, path.c_str(), CASESENSITIVITY );
  NE_ASSERT( err == UNZ_OK, "File %s not found in the zipfile\n", path.c_str() )();
  if( err != UNZ_OK )
  {  
    return nullptr;
  }

  return new ZipFile( m_stream );
}

File* 
ZipFileDevice::Open( File* file )
{
  NE_ASSERT( false, "Attempting to create a piggyback file type for a 'root' device type [Zip]" )();
  return nullptr;
}

void  
ZipFileDevice::Close( File* file )
{
  delete file;
}
