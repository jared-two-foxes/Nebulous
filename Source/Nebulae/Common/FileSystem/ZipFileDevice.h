#ifndef NEBULAE_COMMON_FILESYSTEM_ZIPFILEDEVICE_H_
#define NEBULAE_COMMON_FILESYSTEM_ZIPFILEDEVICE_H_

#include <Nebulae/Common/Common.h>

// #include <minizip/unzip.h>

namespace Nebulae
{

class ZipFileDevice : public FileDevice
{
private:
  std::string m_rootDirectory;
  // unzFile     m_stream;

public:
  ZipFileDevice( const char* path, const char* archiveName, const char* extention = "zip" );
  virtual ~ZipFileDevice();

  virtual File* Open( const std::string& path, FileSystem::Mode mode ) override;
  virtual File* Open( File* file ) override;
  virtual void Close( File* file ) override;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_FILESYSTEM_ZIPFILEDEVICE_H_