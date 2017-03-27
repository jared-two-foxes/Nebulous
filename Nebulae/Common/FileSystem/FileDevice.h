#ifndef __NEBULAE_FILEDEVICE_H__
#define __NEBULAE_FILEDEVICE_H__

namespace Nebulae {

class File;

class FileDevice
{
public:
  FileDevice() {}

  virtual File* Open( const std::string& path, FileSystem::Mode mode ) = 0;
  virtual File* Open( File* file ) = 0; 
  virtual void  Close( File* file ) = 0;
 
};

}

#endif // __NEBULAE_FILESYSTEM_H__