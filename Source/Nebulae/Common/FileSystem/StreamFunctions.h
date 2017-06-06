#ifndef __NEBULAE_FILE_STREAMFUNCTIONS_H__
#define __NEBULAE_FILE_STREAMFUNCTIONS_H__


namespace Nebulae
{

std::size_t file_read( void *ptr, std::size_t size, std::size_t nmemb, void *datasource );

unsigned file_write( void *ptr, unsigned size, unsigned nmemb, void *datasource );

int file_seek( void *datasource, long offset, int whence );

int file_close( void *datasource );

long file_tell( void *datasource );

}

#endif // __NEBULAE_FILE_STREAMFUNCTIONS_H__