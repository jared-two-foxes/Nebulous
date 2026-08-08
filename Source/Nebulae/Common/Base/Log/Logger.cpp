#include "Logger.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <limits>

namespace Nebulae
{

Logger::Logger( std::string name, Level minLevel )
  : m_name( std::move( name ) ), m_minLevel( static_cast<int>( minLevel ) )
{
  m_thread = std::thread( &Logger::ConsumerThreadFunc, this );
}

Logger::~Logger() { Shutdown(); }

void Logger::AddSink( std::shared_ptr<ISink> sink, Level minLevel )
{
  if ( !sink )
  {
    return;
  }

  std::lock_guard<std::mutex> lock( m_sinkMutex );
  m_sinks.emplace_back( std::move( sink ), minLevel );
}

void Logger::RemoveSink( const std::shared_ptr<ISink>& sink )
{
  std::lock_guard<std::mutex> lock( m_sinkMutex );
  m_sinks.erase(
    std::remove_if( m_sinks.begin(), m_sinks.end(),
                    [&]( const std::pair<std::shared_ptr<ISink>, Level>& value ) { return value.first == sink; } ),
    m_sinks.end() );
}

void Logger::SetMinLevel( Level level ) { m_minLevel.store( static_cast<int>( level ), std::memory_order_release ); }

Level Logger::GetMinLevel() const { return static_cast<Level>( m_minLevel.load( std::memory_order_acquire ) ); }

void Logger::Enqueue( LogRecord record )
{
  const bool isCritical = record.level == Level::Critical;
  auto completion = isCritical ? std::make_shared<std::promise<void>>() : nullptr;
  std::future<void> waitFuture;
  if ( completion )
  {
    waitFuture = completion->get_future();
  }

  {
    std::lock_guard<std::mutex> lock( m_mutex );
    if ( m_shutdown )
    {
      return;
    }

    if ( !isCritical && m_pendingCount.load( std::memory_order_relaxed ) >= kMaxQueueDepth )
    {
      m_droppedCount.fetch_add( 1, std::memory_order_relaxed );
      return;
    }

    m_queue.push_back( Entry{ std::move( record ), completion, false } );
    m_pendingCount.fetch_add( 1, std::memory_order_relaxed );
  }

  m_cv.notify_one();

  if ( completion )
  {
    waitFuture.wait();
  }
}

void Logger::Flush()
{
  auto completion = std::make_shared<std::promise<void>>();
  std::future<void> waitFuture = completion->get_future();

  {
    std::lock_guard<std::mutex> lock( m_mutex );
    if ( m_shutdown )
    {
      return;
    }

    m_queue.push_back( Entry{ LogRecord{}, completion, true } );
  }

  m_cv.notify_one();
  waitFuture.wait();

  std::vector<std::shared_ptr<ISink>> sinks;
  {
    std::lock_guard<std::mutex> sinkLock( m_sinkMutex );
    sinks.reserve( m_sinks.size() );
    for ( const auto& sink : m_sinks )
    {
      sinks.push_back( sink.first );
    }
  }

  for ( const auto& sink : sinks )
  {
    if ( sink )
    {
      sink->Flush();
    }
  }
}

void Logger::Shutdown()
{
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    if ( m_shutdown )
    {
      return;
    }
    m_shutdown = true;
  }

  m_cv.notify_one();

  if ( m_thread.joinable() )
  {
    m_thread.join();
  }

  std::vector<std::shared_ptr<ISink>> sinks;
  {
    std::lock_guard<std::mutex> sinkLock( m_sinkMutex );
    sinks.reserve( m_sinks.size() );
    for ( const auto& sink : m_sinks )
    {
      sinks.push_back( sink.first );
    }
  }

  for ( const auto& sink : sinks )
  {
    if ( sink )
    {
      sink->Flush();
    }
  }
}

std::string Logger::GetName() const { return m_name; }

uint64_t Logger::DroppedCount() const { return m_droppedCount.load( std::memory_order_relaxed ); }

uint64_t Logger::DroppedCriticalCount() const { return m_droppedCriticalCount.load( std::memory_order_relaxed ); }

void Logger::Log( const char* message, ... )
{
  if ( message == nullptr )
  {
    return;
  }

  va_list args;
  va_start( args, message );
  const std::string formatted = VFormat( message, args );
  va_end( args );

  if ( formatted.empty() )
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock( m_messagesMutex );
    m_messages.push_back( formatted );
  }

  LogRecord record;
  record.message = formatted;
  record.system = "Legacy";
  record.level = Level::Info;
  record.location = std::source_location::current();
  record.timestamp = std::time( nullptr );
  Enqueue( std::move( record ) );
}

void Logger::ConsumerThreadFunc()
{
  for ( ;; )
  {
    std::deque<Entry> batch;

    {
      std::unique_lock<std::mutex> lock( m_mutex );
      m_cv.wait( lock, [&] { return m_shutdown || !m_queue.empty(); } );

      if ( m_queue.empty() )
      {
        if ( m_shutdown )
        {
          break;
        }
        continue;
      }

      batch.swap( m_queue );
    }

    for ( Entry& entry : batch )
    {
      if ( entry.isFlushBarrier )
      {
        if ( entry.completion )
        {
          entry.completion->set_value();
        }
        continue;
      }

      DispatchRecord( entry.record );
      m_pendingCount.fetch_sub( 1, std::memory_order_relaxed );

      if ( entry.completion )
      {
        entry.completion->set_value();
      }
    }
  }

  std::deque<Entry> remaining;
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    remaining.swap( m_queue );
  }

  for ( Entry& entry : remaining )
  {
    if ( entry.isFlushBarrier )
    {
      if ( entry.completion )
      {
        entry.completion->set_value();
      }
      continue;
    }

    DispatchRecord( entry.record );
    m_pendingCount.fetch_sub( 1, std::memory_order_relaxed );
    if ( entry.completion )
    {
      entry.completion->set_value();
    }
  }
}

void Logger::DispatchRecord( const LogRecord& record )
{
  std::vector<std::pair<std::shared_ptr<ISink>, Level>> sinks;
  {
    std::lock_guard<std::mutex> lock( m_sinkMutex );
    sinks = m_sinks;
  }

  for ( const auto& sink : sinks )
  {
    if ( !sink.first )
    {
      continue;
    }
    if ( record.level < sink.second )
    {
      continue;
    }
    sink.first->Write( record );
  }
}

std::string Logger::VFormat( const char* message, va_list args )
{
  if ( message == nullptr )
  {
    return {};
  }

  va_list argsCopy;
  va_copy( argsCopy, args );
  const int needed = std::vsnprintf( nullptr, 0, message, argsCopy );
  va_end( argsCopy );

  if ( needed < 0 )
  {
    return std::string( message );
  }

  std::string out;
  out.resize( static_cast<size_t>( needed ) );

  if ( needed > 0 )
  {
    std::vsnprintf( &out[0], static_cast<size_t>( needed ) + 1, message, args );
  }

  return out;
}

} // namespace Nebulae
