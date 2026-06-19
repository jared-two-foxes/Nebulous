#ifndef NEBULAE_COMMON_FILESYSTEM_FILEDEVICE_H_
#define NEBULAE_COMMON_FILESYSTEM_FILEDEVICE_H_

namespace Nebulae
{

class File;

class FileDevice
{
public:
  FileDevice() {}

  virtual File* Open( const std::string& path, FileSystem::Mode mode ) = 0;
  virtual File* Open( File* file ) = 0;
  virtual void Close( File* file ) = 0;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_FILESYSTEM_FILEDEVICE_H_