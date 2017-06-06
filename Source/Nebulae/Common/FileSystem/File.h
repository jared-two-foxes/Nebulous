#ifndef __NEBULAE_FILESYSTEM_FILE_H__
#define __NEBULAE_FILESYSTEM_FILE_H__

namespace Nebulae 
{


/**
 *  Encapsulates interaction with opening and closing files from the
 *  system.
 */
class File
{
  public:
    virtual ~File() {}


    ////////////////////////////////////////////////////////////////////////
    // Synchronous API
    ////////////////////////////////////////////////////////////////////////
   
    /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
    virtual size_t Read(void* buffer, size_t length) { return -1; }
   
    /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
    virtual size_t Write(const void* buffer, size_t length) { return -1; }
   
    /// Seeks to the desired position
    virtual void Seek(size_t position) {}
   
    /// Seeks to the end of the file
    virtual void SeekToEnd() {}
   
    /// Skips a certain amount of bytes
    virtual void Skip(size_t bytes) {}
   
    /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
    virtual size_t Tell() const { return -1; }

}; //File


} //Nebulae

#endif // __NEBULAE_FILESYSTEM_FILE_H__