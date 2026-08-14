#include "FileSink.h"

#include <Nebulae/Common/Base/Log/Level.h>

namespace Nebulae
{

FileSink::FileSink( std::string filePath ) : m_filePath( std::move( filePath ) ) {}

FileSink::~FileSink()
{
  Flush();
  if ( m_file.is_open() )
  {
    m_file.close();
  }
}

void FileSink::Write( const LogRecord& record )
{
  if ( !m_file.is_open() )
  {
    m_file.open( m_filePath.c_str(), std::ios::out | std::ios::app );
    if ( !m_file.is_open() )
    {
      return;
    }
  }

  m_file << "[" << LevelToString( record.level ) << "]"
         << " [" << record.system << "] " << record.message << "\n";
}

void FileSink::Flush()
{
  if ( m_file.is_open() )
  {
    m_file.flush();
  }
}

} // namespace Nebulae
