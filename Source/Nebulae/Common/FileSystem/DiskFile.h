#ifndef __NEBULAE_FILESYSTEM_DISKFILE_H__
#define __NEBULAE_FILESYSTEM_DISKFILE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

/**
 *  Encapsulates interaction with opening and closing files from the
 *  device.
 */
class DiskFile : public File
{
private:
  std::fstream* m_stream{ nullptr }; //< the filestream used by the disk file.

public:
  DiskFile( const std::string& filename, bool bReadOnly = true );
  ~DiskFile() override;


  ////////////////////////////////////////////////////////////////////////
  // Synchronous API
  ////////////////////////////////////////////////////////////////////////

  /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
  size_t Read( void* buffer, size_t length ) override;

  /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation
  /// failed.
  size_t Write( const void* buffer, size_t length ) override;

  /// Seeks to the desired position
  void Seek( size_t position ) override;

  /// Seeks to the end of the file
  void SeekToEnd() override;

  /// Skips a certain amount of bytes
  void Skip( size_t bytes ) override;

  /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
  size_t Tell() const override;

}; // DiskFile


} // namespace Nebulae

#endif // __NEBULAE_FILESYSTEM_FILE_H__