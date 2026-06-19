#ifndef NEBULAE_COMMON_FILESYSTEM_STREAMFUNCTIONS_H_
#define NEBULAE_COMMON_FILESYSTEM_STREAMFUNCTIONS_H_


namespace Nebulae
{

std::size_t file_read( void* ptr, std::size_t size, std::size_t nmemb, void* datasource );

unsigned file_write( void* ptr, unsigned size, unsigned nmemb, void* datasource );

int file_seek( void* datasource, long offset, int whence );

int file_close( void* datasource );

long file_tell( void* datasource );

} // namespace Nebulae

#endif // NEBULAE_COMMON_FILESYSTEM_STREAMFUNCTIONS_H_