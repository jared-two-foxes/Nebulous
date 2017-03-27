#ifndef __NEBULAE_FILESYSTEM_H__
#define __NEBULAE_FILESYSTEM_H__

#include <Nebulae/Common/FileSystem/File.h>
#include <Nebulae/Common/FileSystem/StreamFunctions.h>

namespace Nebulae {

class FileDevice;

NE_FLAG_TYPE(FileSystemModeFlags);
extern const FileSystemModeFlags IO_MODE_IN;        ///< File open for reading.
extern const FileSystemModeFlags IO_MODE_OUT;       ///< The left Shift key.
extern const FileSystemModeFlags IO_MODE_BINARY;    ///< The right Shift key.
extern const FileSystemModeFlags IO_MODE_AT_END;    ///< The left Control key.
extern const FileSystemModeFlags IO_MODE_APPEND;    ///< The left Control key.
extern const FileSystemModeFlags IO_MODE_TRUNCATE;  ///< The left Control key.

class FileSystem
{
public:
  typedef Flags<FileSystemModeFlags> Mode;
  typedef std::vector<FileDevice* >  DeviceList;

private:
  std::map<std::string, DeviceList > m_devices; ///< map of registered FileDevices.

  public:
    FileSystem();
    ~FileSystem();

    /// Mounts a file device to the file system
    void Mount( const std::string& type, FileDevice* device );
   
    /// Unmounts a file device from the file system
    void Unmount( const std::string& type, FileDevice* device );
   
    /// Opens a file for synchronous operations.
    /// NOTE: A nullptr is returned if no device for opening the file could be found.
    File* Open( const char* deviceList, const std::string& path, Mode mode = IO_MODE_IN | IO_MODE_BINARY );
   
    /// Closes a file previously returned by a call to Open()
    void Close( File* file );
 
};

}

#endif // __NEBULAE_FILESYSTEM_H__