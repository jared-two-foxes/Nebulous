#ifndef NEBULAE_COMMON_BASE_LOG_LOGGER_H_
#define NEBULAE_COMMON_BASE_LOG_LOGGER_H_

#include <Nebulae/Common/Base/Log/Level.h>
#include <Nebulae/Common/Base/Log/ISink.h>
#include <Nebulae/Common/Base/Log/LogRecord.h>

namespace Nebulae
{
class Logger
{
public:
  explicit Logger( std::string name, Level minLevel = Level::Trace );
  ~Logger();

  Logger( const Logger& ) = delete;
  Logger& operator=( const Logger& ) = delete;

  void AddSink( std::shared_ptr<ISink> sink, Level minLevel = Level::Trace );
  void RemoveSink( const std::shared_ptr<ISink>& sink );

  void SetMinLevel( Level level );
  Level GetMinLevel() const;

  void Enqueue( LogRecord record );
  void Flush();
  void Shutdown();

  std::string GetName() const;

  uint64_t DroppedCount() const;
  uint64_t DroppedCriticalCount() const;

  // Legacy printf-style API kept for existing tests and call sites.
  void Log( const char* message, ... );

  std::vector<std::string> GetMessages() const { return m_messages; }

private:
  struct Entry
  {
    LogRecord record;
    std::shared_ptr<std::promise<void>> completion;
    bool isFlushBarrier{ false };
  };

  void ConsumerThreadFunc();
  void DispatchRecord( const LogRecord& record );
  static std::string VFormat( const char* message, va_list args );

  std::string m_name;
  std::atomic<int> m_minLevel;
  std::vector<std::pair<std::shared_ptr<ISink>, Level>> m_sinks;
  mutable std::mutex m_sinkMutex;

  mutable std::mutex m_mutex;
  std::condition_variable m_cv;
  std::deque<Entry> m_queue;
  bool m_shutdown{ false };
  std::thread m_thread;

  std::atomic<uint64_t> m_droppedCount{ 0 };
  std::atomic<uint64_t> m_droppedCriticalCount{ 0 };
  std::atomic<size_t> m_pendingCount{ 0 };

  mutable std::mutex m_messagesMutex;
  std::vector<std::string> m_messages;

  static constexpr size_t kMaxQueueDepth = 8192;
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_BASE_LOG_LOGGER_H_
