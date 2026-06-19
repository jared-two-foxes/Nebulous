#ifndef NEBULAE_COMMON_BASE_LOG_SINKS_FILESINK_H_
#define NEBULAE_COMMON_BASE_LOG_SINKS_FILESINK_H_

#include <Nebulae/Common/Base/Log/ISink.h>

#include <fstream>
#include <string>

namespace Nebulae
{

class FileSink : public ISink
{
public:
  explicit FileSink( std::string filePath );
  ~FileSink() override;

  void Write( const LogRecord& record ) override;
  void Flush() override;

private:
  std::string m_filePath;
  std::ofstream m_file;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_BASE_LOG_SINKS_FILESINK_H_
