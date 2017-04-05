#ifndef __NEBULAE_ZIPFILEDEVICE_H__
#define __NEBULAE_ZIPFILEDEVICE_H__

#include <Nebulae/Common/Common.h>

//#include <minizip/unzip.h>

namespace Nebulae {

class ZipFileDevice : public FileDevice
{
private:
  std::string m_rootDirectory;
  //unzFile     m_stream;

  public:
    ZipFileDevice( const char* path, const char* archiveName, const char* extention = "zip" );
    virtual ~ZipFileDevice();

    virtual File* Open( const std::string& path, FileSystem::Mode mode ) override; 
    virtual File* Open( File* file ) override; 
    virtual void  Close( File* file ) override;

};

}

#endif // __NEBULAE_ZIPFILEDEVICE_H__