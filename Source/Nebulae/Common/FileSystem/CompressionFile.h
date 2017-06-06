#ifndef __NEBULAE_FILESYSTEM_ZIPFILE_H__
#define __NEBULAE_FILESYSTEM_ZIPFILE_H__

#include <Nebulae/Common/Common.h>

#include <zlib.h>

namespace Nebulae {

/**
 *  Encapsulates interaction with opening and closing files from the device.
 */
class CompressionFile : public File
{
private:
  File*    m_file;  
  z_stream m_z_stream; ///< The zlib stream.

  public:
    CompressionFile( File* stream );
    virtual ~CompressionFile();

   
    /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
    virtual size_t Read( void* buffer, size_t length ) override;
   
    /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
    virtual size_t Write( const void* buffer, size_t length ) override;
   
    /// Seeks to the desired position
    virtual void Seek( size_t position ) override;
   
    /// Seeks to the end of the file
    virtual void SeekToEnd() override;
   
    /// Skips a certain amount of bytes
    virtual void Skip( size_t bytes ) override;
   
    /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
    virtual size_t Tell() const override;

}; //ZipFile


} //Nebulae

#endif // __NEBULAE_FILESYSTEM_ZIPFILE_H__