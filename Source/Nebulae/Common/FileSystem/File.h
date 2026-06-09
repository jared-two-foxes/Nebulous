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
  virtual size_t Read( [[maybe_unused]] void* buffer, [[maybe_unused]] size_t size )
  {
    return static_cast<size_t>( -1 );
  }

  /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation
  /// failed.
  virtual size_t Write( [[maybe_unused]] const void* buffer, [[maybe_unused]] size_t size )
  {
    return static_cast<size_t>( -1 );
  }

  /// Seeks to the desired position
  virtual void Seek( [[maybe_unused]] size_t position ) {}

  /// Seeks to the end of the file
  virtual void SeekToEnd() {}

  /// Skips a certain amount of bytes
  virtual void Skip( [[maybe_unused]] size_t bytes ) {}

  /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
  virtual size_t Tell() const { return static_cast<size_t>( -1 ); }

}; // File


} // namespace Nebulae

#endif // __NEBULAE_FILESYSTEM_FILE_H__