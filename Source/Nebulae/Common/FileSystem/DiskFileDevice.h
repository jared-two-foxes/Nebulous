#ifndef NEBULAE_COMMON_FILESYSTEM_DISKFILEDEVICE_H_
#define NEBULAE_COMMON_FILESYSTEM_DISKFILEDEVICE_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Platform;

class DiskFileDevice : public FileDevice
{
private:
  std::string m_rootDirectory;

public:
  DiskFileDevice( const char* path );
  virtual ~DiskFileDevice();

  File* Open( const std::string& path, FileSystem::Mode mode ) override;
  File* Open( File* file ) override;
  void Close( File* file ) override;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_FILESYSTEM_DISKFILEDEVICE_H_