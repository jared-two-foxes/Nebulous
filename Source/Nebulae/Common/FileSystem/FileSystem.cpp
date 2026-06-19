
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFile.h>
#include <Nebulae/Common/FileSystem/FileDevice.h>

using Nebulae::DiskFile;
using Nebulae::File;
using Nebulae::FileDevice;
using Nebulae::FileSystem;
using Nebulae::FileSystemModeFlags;
using Nebulae::FlagSpec;


///////////////////////////////////////
// ModKeys
///////////////////////////////////////
const FileSystemModeFlags Nebulae::IO_MODE_IN( 0x0000 );
const FileSystemModeFlags Nebulae::IO_MODE_OUT( 0x0001 );
const FileSystemModeFlags Nebulae::IO_MODE_BINARY( 0x0002 );
const FileSystemModeFlags Nebulae::IO_MODE_AT_END( 0x0040 );
const FileSystemModeFlags Nebulae::IO_MODE_APPEND( 0x0080 );
const FileSystemModeFlags Nebulae::IO_MODE_TRUNCATE( 0x0100 );

namespace
{
bool RegisterModeKeys()
{
  FlagSpec<FileSystemModeFlags>& spec = FlagSpec<FileSystemModeFlags>::instance();
  spec.insert( Nebulae::IO_MODE_IN, "IO_MODE_IN", true );
  spec.insert( Nebulae::IO_MODE_OUT, "IO_MODE_OUT", true );
  spec.insert( Nebulae::IO_MODE_BINARY, "IO_MODE_BINARY", true );
  spec.insert( Nebulae::IO_MODE_AT_END, "IO_MODE_AT_END", true );
  spec.insert( Nebulae::IO_MODE_APPEND, "IO_MODE_APPEND", true );
  spec.insert( Nebulae::IO_MODE_TRUNCATE, "IO_MODE_TRUNCATE", true );
  return true;
}
bool dummy = RegisterModeKeys();
} // namespace

NE_FLAGSPEC_IMPL( FileSystemModeFlags );


std::vector<std::string>& Split( const std::string& s, char delim, std::vector<std::string>& elems )
{
  std::stringstream ss( s );
  std::string item;
  while ( std::getline( ss, item, delim ) )
  {
    elems.push_back( item );
  }

  return elems;
}

std::vector<std::string> Split( const std::string& s, char delim )
{
  std::vector<std::string> elems;
  Split( s, delim, elems );
  return elems;
}

FileSystem::FileSystem() = default;

FileSystem::~FileSystem() = default;

File* FileSystem::Open( const char* devices, const std::string& path, Mode mode )
{
  std::vector<std::string> elems = Split( devices, ':' );

  File* file = nullptr;

  for ( auto it = elems.rbegin(); it != elems.rend(); ++it )
  {
    DeviceList& deviceList = m_devices[*it];

    NE_ASSERT( !deviceList.empty(), "Unable to find a Device of type '%s'" );

    for ( auto& deviceItr : deviceList )
    {
      File* createdFile = nullptr;

      if ( file != nullptr )
      {
        createdFile = deviceItr->Open( file );
      }
      else
      {
        createdFile = deviceItr->Open( path, mode );
      }

      if ( createdFile != nullptr )
      {
        file = createdFile;
        break;
      }
    }
  }

  NE_ASSERT( file, "Attempting to open file '%s' failed", path.c_str() );
  return file;
}

void FileSystem::Mount( const std::string& type, FileDevice* device )
{
  // Add to DeviceList if it is not already registered.
  DeviceList::iterator it = std::find( m_devices[type].begin(), m_devices[type].end(), device );
  NE_ASSERT( it == m_devices[type].end(), "Specified device is not registered for this device type" );
  if ( it == m_devices[type].end() )
  {
    m_devices[type].push_back( device );
  }
}

void FileSystem::Unmount( const std::string& type, FileDevice* device )
{
  // Remove from the FileDevice list.
  DeviceList::iterator it = std::find( m_devices[type].begin(), m_devices[type].end(), device );
  NE_ASSERT( it != m_devices[type].end(), "Specified device is not registered for this device type" );
  if ( it != m_devices[type].end() )
  {
    m_devices[type].erase( it );
  }
}
