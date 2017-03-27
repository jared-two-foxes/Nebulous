#ifndef __NEBULAE_DISKFILEDEVICE_H__
#define __NEBULAE_DISKFILEDEVICE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Platform;

class DiskFileDevice : public FileDevice
{
private:
  std::string m_rootDirectory;

  public:
    DiskFileDevice( const char* path );
    virtual ~DiskFileDevice();

    virtual File* Open( const std::string& path, FileSystem::Mode mode ) override; 
    virtual File* Open( File* file ) override; 
    virtual void  Close( File* file ) override;

};

}

#endif // __NEBULAE_FILESYSTEM_H__