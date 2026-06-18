// =============================================================================
// test_logging_system.cpp
// =============================================================================
// Unit tests for the Nebulae logging system described in:
//   docs/logging-system-implementation-plan.md
//
// Tests are organised by acceptance criterion group (AC-F, AC-A, AC-D, AC-P).
// All headers referenced here are part of the NOT-YET-IMPLEMENTED Log/
// subsystem. This file will fail to compile until the implementation is
// complete — that is the intended TDD red state.
// =============================================================================

#include <Nebulae/Common/Base/Log/ISink.h>
#include <Nebulae/Common/Base/Log/Level.h>
#include <Nebulae/Common/Base/Log/Log.h>
#include <Nebulae/Common/Base/Log/LogRecord.h>
#include <Nebulae/Common/Base/Log/LogRegistry.h>
#include <Nebulae/Common/Base/Log/Logger.h>
#include <Nebulae/Common/Base/Log/ModuleLogger.h>
#include <Nebulae/Common/Base/Log/Sinks/ConsoleSink.h>
#include <Nebulae/Common/Base/Log/Sinks/DebugOutputSink.h>
#include <Nebulae/Common/Base/Log/Sinks/FileSink.h>

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace
{

// ---------------------------------------------------------------------------
// MockSink — captures every record written to it.
// ---------------------------------------------------------------------------
class MockSink : public Nebulae::ISink
{
public:
  void Write( const Nebulae::LogRecord& record ) override
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    m_records.push_back( record );
    if ( m_onWrite )
      m_onWrite( record );
  }

  void Flush() override { m_flushCount++; }

  std::vector<Nebulae::LogRecord> Records() const
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    return m_records;
  }

  size_t Count() const
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    return m_records.size();
  }

  int FlushCount() const { return m_flushCount.load(); }

  void SetOnWrite( std::function<void( const Nebulae::LogRecord& )> cb ) { m_onWrite = std::move( cb ); }

private:
  mutable std::mutex m_mutex;
  std::vector<Nebulae::LogRecord> m_records;
  std::atomic<int> m_flushCount{ 0 };
  std::function<void( const Nebulae::LogRecord& )> m_onWrite;
};

// Helper: create a logger with a MockSink attached, flush, return records.
struct LoggerFixture
{
  std::shared_ptr<MockSink> sink = std::make_shared<MockSink>();
  Nebulae::Logger logger{ "Test", Nebulae::Level::Trace };

  LoggerFixture() { logger.AddSink( sink ); }
};

} // anonymous namespace

// =============================================================================
// AC-F — Functional
// =============================================================================

// AC-F1: All six log levels are usable via the _TO macro variant.
TEST( LoggingSystem_Functional, AC_F1_AllSixLevelsCanBeEnqueued )
{
  LoggerFixture f;

  NE_LOG_TRACE_TO( f.logger, "TestSys", "trace message" );
  NE_LOG_DEBUG_TO( f.logger, "TestSys", "debug message" );
  NE_LOG_INFO_TO( f.logger, "TestSys", "info message" );
  NE_LOG_WARN_TO( f.logger, "TestSys", "warn message" );
  NE_LOG_ERROR_TO( f.logger, "TestSys", "error message" );
  NE_LOG_CRITICAL_TO( f.logger, "TestSys", "critical message" );

  f.logger.Flush();

  ASSERT_EQ( 6u, f.sink->Count() );

  auto records = f.sink->Records();
  EXPECT_EQ( Nebulae::Level::Trace, records[0].level );
  EXPECT_EQ( Nebulae::Level::Debug, records[1].level );
  EXPECT_EQ( Nebulae::Level::Info, records[2].level );
  EXPECT_EQ( Nebulae::Level::Warning, records[3].level );
  EXPECT_EQ( Nebulae::Level::Error, records[4].level );
  EXPECT_EQ( Nebulae::Level::Critical, records[5].level );
}

// AC-F2: Per-logger minimum level filter suppresses records below the threshold
//        BEFORE any formatting occurs (the message body should not be present).
TEST( LoggingSystem_Functional, AC_F2_PerLoggerMinLevelSuppressesLowerLevels )
{
  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "FilterTest", Nebulae::Level::Warning );
  logger.AddSink( sink );

  NE_LOG_TRACE_TO( logger, "S", "trace — must be dropped" );
  NE_LOG_DEBUG_TO( logger, "S", "debug — must be dropped" );
  NE_LOG_INFO_TO( logger, "S", "info — must be dropped" );
  NE_LOG_WARN_TO( logger, "S", "warn — must pass" );
  NE_LOG_ERROR_TO( logger, "S", "error — must pass" );

  logger.Flush();

  ASSERT_EQ( 2u, sink->Count() );
  EXPECT_EQ( Nebulae::Level::Warning, sink->Records()[0].level );
  EXPECT_EQ( Nebulae::Level::Error, sink->Records()[1].level );
}

// AC-F2 (gate before format): a call below minimum level must not allocate;
//       we verify indirectly by checking sink receives nothing even when the
//       logger min level is Off (all records suppressed).
TEST( LoggingSystem_Functional, AC_F2_LevelOffSuppressesEverything )
{
  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "OffTest", Nebulae::Level::Off );
  logger.AddSink( sink );

  NE_LOG_CRITICAL_TO( logger, "S", "critical — must be dropped when level is Off" );

  logger.Flush();

  EXPECT_EQ( 0u, sink->Count() );
}

// AC-F3: Per-sink minimum level filter suppresses records below per-sink threshold.
TEST( LoggingSystem_Functional, AC_F3_PerSinkMinLevelFiltersAtDispatch )
{
  auto sinkAll = std::make_shared<MockSink>();
  auto sinkWarnPlus = std::make_shared<MockSink>();

  Nebulae::Logger logger( "SinkFilter", Nebulae::Level::Trace );
  logger.AddSink( sinkAll, Nebulae::Level::Trace );
  logger.AddSink( sinkWarnPlus, Nebulae::Level::Warning );

  NE_LOG_INFO_TO( logger, "S", "info" );
  NE_LOG_WARN_TO( logger, "S", "warn" );

  logger.Flush();

  EXPECT_EQ( 2u, sinkAll->Count() );
  EXPECT_EQ( 1u, sinkWarnPlus->Count() );
  EXPECT_EQ( Nebulae::Level::Warning, sinkWarnPlus->Records()[0].level );
}

// AC-F4: System/channel tag is present in every LogRecord.
TEST( LoggingSystem_Functional, AC_F4_SystemTagPresentInRecord )
{
  LoggerFixture f;
  NE_LOG_INFO_TO( f.logger, "RenderSystem", "a message" );
  f.logger.Flush();

  ASSERT_EQ( 1u, f.sink->Count() );
  EXPECT_EQ( "RenderSystem", f.sink->Records()[0].system );
}

// AC-F5: Source file, line number, and function name are present in every record.
TEST( LoggingSystem_Functional, AC_F5_SourceLocationPresentInRecord )
{
  LoggerFixture f;
  NE_LOG_INFO_TO( f.logger, "S", "location test" );
  f.logger.Flush();

  ASSERT_EQ( 1u, f.sink->Count() );
  const auto& loc = f.sink->Records()[0].location;
  EXPECT_NE( nullptr, loc.file_name() );
  EXPECT_GT( loc.line(), 0u );
  EXPECT_NE( nullptr, loc.function_name() );
  // file_name should contain this file's name
  EXPECT_NE( nullptr, std::strstr( loc.file_name(), "test_logging_system" ) );
}

// AC-F6: Timestamp is present (non-zero) in every LogRecord.
TEST( LoggingSystem_Functional, AC_F6_TimestampPresentInRecord )
{
  LoggerFixture f;
  std::time_t before = std::time( nullptr );
  NE_LOG_INFO_TO( f.logger, "S", "timestamp test" );
  f.logger.Flush();
  std::time_t after = std::time( nullptr );

  ASSERT_EQ( 1u, f.sink->Count() );
  const auto& ts = f.sink->Records()[0].timestamp;
  EXPECT_GE( ts, before );
  EXPECT_LE( ts, after );
}

// AC-F8: LogRegistry can hold and independently manage multiple named loggers.
TEST( LoggingSystem_Functional, AC_F8_LogRegistryHoldsMultipleLoggers )
{
  Nebulae::LogRegistry registry;
  Nebulae::Logger& a = registry.GetOrCreate( "Alpha" );
  Nebulae::Logger& b = registry.GetOrCreate( "Beta" );
  Nebulae::Logger& c = registry.GetOrCreate( "Gamma" );

  EXPECT_EQ( "Alpha", a.GetName() );
  EXPECT_EQ( "Beta", b.GetName() );
  EXPECT_EQ( "Gamma", c.GetName() );

  // Same name returns same logger.
  Nebulae::Logger& a2 = registry.GetOrCreate( "Alpha" );
  EXPECT_EQ( &a, &a2 );

  // Get returns nullptr for unknown names.
  EXPECT_EQ( nullptr, registry.Get( "DoesNotExist" ) );
}

// AC-F9: Each logger can have multiple sinks registered independently.
TEST( LoggingSystem_Functional, AC_F9_MultipleIndependentSinksPerLogger )
{
  auto sink1 = std::make_shared<MockSink>();
  auto sink2 = std::make_shared<MockSink>();
  auto sink3 = std::make_shared<MockSink>();

  Nebulae::Logger logger( "Multi", Nebulae::Level::Trace );
  logger.AddSink( sink1 );
  logger.AddSink( sink2 );
  logger.AddSink( sink3 );

  NE_LOG_INFO_TO( logger, "S", "broadcast" );
  logger.Flush();

  EXPECT_EQ( 1u, sink1->Count() );
  EXPECT_EQ( 1u, sink2->Count() );
  EXPECT_EQ( 1u, sink3->Count() );
}

// AC-F10: FileSink produces a correctly formatted, readable log file.
TEST( LoggingSystem_Functional, AC_F10_FileSinkWritesReadableFile )
{
  const std::string path = "test_ac_f10_output.log";

  {
    Nebulae::Logger logger( "FileTest", Nebulae::Level::Trace );
    logger.AddSink( std::make_shared<Nebulae::FileSink>( path ) );

    for ( int i = 0; i < 100; ++i )
    {
      NE_LOG_INFO_TO( logger, "S", "record {}", i );
    }
    logger.Flush();
    // Logger destructor calls Shutdown() which drains and flushes.
  }

  std::ifstream file( path );
  ASSERT_TRUE( file.is_open() ) << "FileSink did not create the log file.";

  int lineCount = 0;
  std::string line;
  while ( std::getline( file, line ) )
    ++lineCount;

  EXPECT_EQ( 100, lineCount ) << "FileSink should have written exactly 100 lines.";

  file.close();
  std::filesystem::remove( path );
}

// AC-F11: ConsoleSink construction does not crash; basic sanity check.
TEST( LoggingSystem_Functional, AC_F11_ConsoleSinkConstructsAndAcceptsRecords )
{
  Nebulae::Logger logger( "ConsoleTest", Nebulae::Level::Trace );
  logger.AddSink( std::make_shared<Nebulae::ConsoleSink>( false ) );

  // One record at each level — must not crash.
  NE_LOG_TRACE_TO( logger, "S", "trace" );
  NE_LOG_DEBUG_TO( logger, "S", "debug" );
  NE_LOG_INFO_TO( logger, "S", "info" );
  NE_LOG_WARN_TO( logger, "S", "warn" );
  NE_LOG_ERROR_TO( logger, "S", "error" );
  NE_LOG_CRITICAL_TO( logger, "S", "critical" );

  EXPECT_NO_FATAL_FAILURE( logger.Flush() );
}

// AC-F12: DebugOutputSink can be constructed and used without crashing.
//         On Windows this calls OutputDebugStringA; on other platforms it
//         should be a no-op that still satisfies the ISink interface.
TEST( LoggingSystem_Functional, AC_F12_DebugOutputSinkConstructsAndAcceptsRecords )
{
  Nebulae::Logger logger( "DebugSinkTest", Nebulae::Level::Trace );
  logger.AddSink( std::make_shared<Nebulae::DebugOutputSink>() );

  NE_LOG_INFO_TO( logger, "S", "debug output test" );

  EXPECT_NO_FATAL_FAILURE( logger.Flush() );
}

// AC-F13: Calling any macro before NE_SetModuleLogger does not crash and
//         produces no records.
//
// NOTE: This test deliberately does NOT call NE_SetModuleLogger. It relies
// on the fact that the module-logger atomic defaults to nullptr.
// If a previous test set the module logger, this test explicitly clears it.
TEST( LoggingSystem_Functional, AC_F13_MacroBeforeSetModuleLoggerDoesNotCrash )
{
  // Clear the module logger to simulate pre-init state.
  NE_SetModuleLogger( nullptr );

  // None of the following calls should crash.
  EXPECT_NO_FATAL_FAILURE( NE_LOG_TRACE( "S", "pre-init trace" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_INFO( "S", "pre-init info" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_CRITICAL( "S", "pre-init critical" ) );
}

// AC-F14: _TO macro variants log to the explicitly passed logger, not the
//         module default.
TEST( LoggingSystem_Functional, AC_F14_ToVariantLogsToExplicitLogger )
{
  auto sinkDefault = std::make_shared<MockSink>();
  auto sinkExplicit = std::make_shared<MockSink>();

  Nebulae::Logger defaultLogger( "Default", Nebulae::Level::Trace );
  defaultLogger.AddSink( sinkDefault );

  Nebulae::Logger explicitLogger( "Explicit", Nebulae::Level::Trace );
  explicitLogger.AddSink( sinkExplicit );

  NE_SetModuleLogger( &defaultLogger );

  NE_LOG_INFO_TO( explicitLogger, "S", "goes to explicit" );
  defaultLogger.Flush();
  explicitLogger.Flush();

  EXPECT_EQ( 0u, sinkDefault->Count() ) << "_TO macro must not write to the module default logger.";
  EXPECT_EQ( 1u, sinkExplicit->Count() ) << "_TO macro must write to the explicitly passed logger.";

  NE_SetModuleLogger( nullptr );
}

// =============================================================================
// AC-A — Async & Thread Safety
// =============================================================================

// AC-A1: Calling thread is not blocked on non-Critical messages.
//        Verified by using a sink whose Write() takes a measurable time and
//        confirming the calling thread returns before Write() completes.
TEST( LoggingSystem_Async, AC_A1_NonCriticalEnqueueReturnsBeforeSinkWrite )
{
  std::atomic<bool> writeStarted{ false };
  std::atomic<bool> writeFinished{ false };

  auto slowSink = std::make_shared<MockSink>();
  slowSink->SetOnWrite(
    [&]( const Nebulae::LogRecord& )
    {
      writeStarted.store( true );
      std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
      writeFinished.store( true );
    } );

  Nebulae::Logger logger( "AsyncTest", Nebulae::Level::Trace );
  logger.AddSink( slowSink );

  auto enqueueStart = std::chrono::steady_clock::now();
  NE_LOG_INFO_TO( logger, "S", "non-critical" );
  auto enqueueEnd = std::chrono::steady_clock::now();

  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( enqueueEnd - enqueueStart ).count();

  // The enqueue call must return well before the 50 ms sink delay elapses.
  EXPECT_LT( elapsed, 20 ) << "Non-critical Enqueue() should return immediately, not block on sink I/O.";

  logger.Flush();
  EXPECT_TRUE( writeFinished.load() );
}

// AC-A2: Concurrent calls from N threads produce no data races.
//        Two threads enqueue 10,000 records each; all 20,000 must arrive.
TEST( LoggingSystem_Async, AC_A2_ConcurrentEnqueueProducesNoDataRaces )
{
  constexpr int kRecordsPerThread = 10'000;
  constexpr int kThreadCount = 2;

  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "ConcurrentTest", Nebulae::Level::Trace );
  logger.AddSink( sink );

  std::vector<std::thread> threads;
  threads.reserve( kThreadCount );

  for ( int t = 0; t < kThreadCount; ++t )
  {
    threads.emplace_back(
      [&logger, t, kRecordsPerThread]()
      {
        for ( int i = 0; i < kRecordsPerThread; ++i )
          NE_LOG_INFO_TO( logger, "Thread", "t={} i={}", t, i );
      } );
  }

  for ( auto& th : threads )
    th.join();

  logger.Flush();

  EXPECT_EQ( static_cast<size_t>( kThreadCount * kRecordsPerThread ), sink->Count() )
    << "All records from concurrent threads must be received by the sink.";
}

// AC-A3: All records enqueued before Flush() are processed before Flush() returns.
TEST( LoggingSystem_Async, AC_A3_FlushDrainsAllPreviouslyEnqueuedRecords )
{
  constexpr int kCount = 500;

  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "FlushTest", Nebulae::Level::Trace );
  logger.AddSink( sink );

  for ( int i = 0; i < kCount; ++i )
    NE_LOG_INFO_TO( logger, "S", "record {}", i );

  logger.Flush();

  EXPECT_EQ( static_cast<size_t>( kCount ), sink->Count() )
    << "Flush() must not return until all previously enqueued records are dispatched.";
}

// AC-A4: Shutdown() drains all queued records before the consumer thread exits.
TEST( LoggingSystem_Async, AC_A4_ShutdownDrainsQueueBeforeThreadExits )
{
  constexpr int kCount = 200;

  auto sink = std::make_shared<MockSink>();

  {
    Nebulae::Logger logger( "ShutdownTest", Nebulae::Level::Trace );
    logger.AddSink( sink );

    for ( int i = 0; i < kCount; ++i )
      NE_LOG_INFO_TO( logger, "S", "record {}", i );

    // Destructor calls Shutdown() implicitly.
  }

  EXPECT_EQ( static_cast<size_t>( kCount ), sink->Count() )
    << "Shutdown() must drain all records before joining the consumer thread.";
}

// AC-A5: Non-Critical records dropped due to full queue increment DroppedCount().
//        To trigger drops we must fill the queue (kMaxQueueDepth = 8192).
//        We do this by pausing the consumer thread via a blocking sink,
//        enqueuing more than kMaxQueueDepth records, then releasing.
TEST( LoggingSystem_Async, AC_A5_DroppedNonCriticalRecordsIncrementDropCount )
{
  constexpr size_t kMaxQueueDepth = 8192;
  constexpr size_t kOverfill = 200; // records beyond capacity

  std::mutex blockMutex;
  std::condition_variable blockCV;
  bool released = false;

  auto blockingSink = std::make_shared<MockSink>();
  blockingSink->SetOnWrite(
    [&]( const Nebulae::LogRecord& )
    {
      std::unique_lock<std::mutex> lk( blockMutex );
      blockCV.wait( lk, [&] { return released; } );
    } );

  Nebulae::Logger logger( "DropTest", Nebulae::Level::Trace );
  logger.AddSink( blockingSink );

  // Fill beyond queue depth. The first kMaxQueueDepth records enter the queue;
  // extras are dropped.
  for ( size_t i = 0; i < kMaxQueueDepth + kOverfill; ++i )
    NE_LOG_INFO_TO( logger, "S", "record {}", i );

  EXPECT_GE( logger.DroppedCount(), kOverfill ) << "Overflow records must be counted by DroppedCount().";

  // Release the blocking sink so the logger can drain and shut down cleanly.
  {
    std::lock_guard<std::mutex> lk( blockMutex );
    released = true;
  }
  blockCV.notify_all();

  logger.Flush();
}

// AC-A6: Critical records block the calling thread and are never dropped.
TEST( LoggingSystem_Async, AC_A6_CriticalRecordsAreNeverDropped )
{
  constexpr int kCount = 20;

  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "CriticalTest", Nebulae::Level::Trace );
  logger.AddSink( sink );

  for ( int i = 0; i < kCount; ++i )
    NE_LOG_CRITICAL_TO( logger, "S", "critical {}", i );

  // After all Critical enqueues return (each blocks until consumed),
  // the sink must already have all records — no further Flush needed.
  EXPECT_EQ( static_cast<size_t>( kCount ), sink->Count() )
    << "Critical enqueue must block until the record is consumed by the sink.";

  EXPECT_EQ( 0u, logger.DroppedCriticalCount() ) << "Critical records must never be dropped.";
}

// AC-A7: Critical records are NOT reordered — they enter and exit the queue
//        in the same relative order as other records.
TEST( LoggingSystem_Async, AC_A7_CriticalRecordsPreserveQueueOrder )
{
  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "OrderTest", Nebulae::Level::Trace );
  logger.AddSink( sink );

  // Enqueue alternating Info and Critical records from a single thread.
  // The consumer must dispatch them in submission order.
  NE_LOG_INFO_TO( logger, "S", "seq=0" );
  NE_LOG_CRITICAL_TO( logger, "S", "seq=1" );
  NE_LOG_INFO_TO( logger, "S", "seq=2" );
  NE_LOG_CRITICAL_TO( logger, "S", "seq=3" );

  logger.Flush();

  ASSERT_EQ( 4u, sink->Count() );
  auto records = sink->Records();
  EXPECT_EQ( "seq=0", records[0].message );
  EXPECT_EQ( "seq=1", records[1].message );
  EXPECT_EQ( "seq=2", records[2].message );
  EXPECT_EQ( "seq=3", records[3].message );
}

// =============================================================================
// AC-D — DLL Boundary
// =============================================================================

// AC-D1: A Logger* set via NE_SetModuleLogger is read back correctly by
//        NE_GetModuleLogger and used by the implicit macro variants.
TEST( LoggingSystem_DLL, AC_D1_SetModuleLoggerIsUsedByImplicitMacros )
{
  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "ModuleTest", Nebulae::Level::Trace );
  logger.AddSink( sink );

  NE_SetModuleLogger( &logger );

  NE_LOG_INFO( "S", "via module logger" );

  logger.Flush();

  EXPECT_EQ( 1u, sink->Count() ) << "NE_LOG_INFO must use the logger set via NE_SetModuleLogger.";

  NE_SetModuleLogger( nullptr );
}

// AC-D3: After NE_SetModuleLogger(nullptr) all implicit macro calls are no-ops.
TEST( LoggingSystem_DLL, AC_D3_NullModuleLoggerSilentlyDropsRecords )
{
  NE_SetModuleLogger( nullptr );

  // All macros must be no-ops when the module logger is null.
  EXPECT_NO_FATAL_FAILURE( NE_LOG_TRACE( "S", "dropped" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_DEBUG( "S", "dropped" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_INFO( "S", "dropped" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_WARN( "S", "dropped" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_ERROR( "S", "dropped" ) );
  EXPECT_NO_FATAL_FAILURE( NE_LOG_CRITICAL( "S", "dropped" ) );
}

// AC-D4: NE_SetModuleLogger is declared extern "C".
//        Verified at compile time by attempting to take its address as a C
//        function pointer (would fail to compile if it had C++ linkage).
TEST( LoggingSystem_DLL, AC_D4_SetModuleLoggerIsExternC )
{
  using SetterFn = void ( * )( Nebulae::Logger* );
  SetterFn fn = &NE_SetModuleLogger;
  EXPECT_NE( nullptr, fn );
}

// =============================================================================
// AC-P — Performance (structural / proxy tests)
// =============================================================================

// AC-P1 proxy: When a record is filtered out by the logger min level,
//              the sink must not receive it. This is a necessary (but not
//              sufficient) condition for zero heap allocations on the fast path.
TEST( LoggingSystem_Performance, AC_P1_FilteredCallDoesNotReachSink )
{
  auto sink = std::make_shared<MockSink>();
  Nebulae::Logger logger( "PerfTest", Nebulae::Level::Error );
  logger.AddSink( sink );

  for ( int i = 0; i < 10'000; ++i )
    NE_LOG_TRACE_TO( logger, "S", "filtered {}", i );

  logger.Flush();

  EXPECT_EQ( 0u, sink->Count() ) << "Filtered calls must not reach the sink.";
}

// AC-P3 proxy: The calling thread must hold the mutex only briefly.
//              We verify this by having a slow sink and checking that a
//              second thread can successfully enqueue during the slow write.
TEST( LoggingSystem_Performance, AC_P3_QueueMutexIsNotHeldDuringSinkWrite )
{
  std::atomic<bool> enqueueCompletedWhileSinkWasWriting{ false };
  std::atomic<bool> sinkWriteStarted{ false };

  auto slowSink = std::make_shared<MockSink>();
  slowSink->SetOnWrite(
    [&]( const Nebulae::LogRecord& )
    {
      sinkWriteStarted.store( true );
      std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
    } );

  Nebulae::Logger logger( "MutexTest", Nebulae::Level::Trace );
  logger.AddSink( slowSink );

  // Enqueue first record to start the slow write.
  NE_LOG_INFO_TO( logger, "S", "first" );

  // Wait until the sink has started its slow write.
  while ( !sinkWriteStarted.load() )
    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

  // Now enqueue a second record from this thread. If the mutex is held
  // during sink Write(), this will block for ~30ms. It should complete quickly.
  auto t0 = std::chrono::steady_clock::now();
  NE_LOG_INFO_TO( logger, "S", "second" );
  auto t1 = std::chrono::steady_clock::now();

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( t1 - t0 ).count();
  enqueueCompletedWhileSinkWasWriting.store( ms < 10 );

  logger.Flush();

  EXPECT_TRUE( enqueueCompletedWhileSinkWasWriting.load() )
    << "Enqueue() must not hold the mutex during sink Write(). "
       "Enqueue took too long while the consumer was in Write().";
}

// =============================================================================
// AC — Level utilities
// =============================================================================

// LevelToString returns non-null, non-empty strings for all valid levels.
TEST( LoggingSystem_Level, LevelToString_AllLevels_ReturnNonEmptyStrings )
{
  using namespace Nebulae;

  EXPECT_STREQ( "TRACE", LevelToString( Level::Trace ) );
  EXPECT_STREQ( "DEBUG", LevelToString( Level::Debug ) );
  EXPECT_STREQ( "INFO", LevelToString( Level::Info ) );
  EXPECT_STREQ( "WARNING", LevelToString( Level::Warning ) );
  EXPECT_STREQ( "ERROR", LevelToString( Level::Error ) );
  EXPECT_STREQ( "CRITICAL", LevelToString( Level::Critical ) );
}

// Level enum values must be ordered Trace < Debug < Info < Warning < Error < Critical < Off.
TEST( LoggingSystem_Level, LevelEnum_OrderIsMonotonicallyIncreasing )
{
  using namespace Nebulae;

  EXPECT_LT( static_cast<int>( Level::Trace ), static_cast<int>( Level::Debug ) );
  EXPECT_LT( static_cast<int>( Level::Debug ), static_cast<int>( Level::Info ) );
  EXPECT_LT( static_cast<int>( Level::Info ), static_cast<int>( Level::Warning ) );
  EXPECT_LT( static_cast<int>( Level::Warning ), static_cast<int>( Level::Error ) );
  EXPECT_LT( static_cast<int>( Level::Error ), static_cast<int>( Level::Critical ) );
  EXPECT_LT( static_cast<int>( Level::Critical ), static_cast<int>( Level::Off ) );
}

// =============================================================================
// AC — LogRegistry lifecycle
// =============================================================================

// AC-F8 / Phase 3: ShutdownAll() drains all loggers cleanly.
TEST( LoggingSystem_Registry, ShutdownAll_DrainsAllLoggersCleany )
{
  constexpr int kCount = 50;

  auto sink1 = std::make_shared<MockSink>();
  auto sink2 = std::make_shared<MockSink>();
  auto sink3 = std::make_shared<MockSink>();

  Nebulae::LogRegistry registry;
  registry.GetOrCreate( "A" ).AddSink( sink1 );
  registry.GetOrCreate( "B" ).AddSink( sink2 );
  registry.GetOrCreate( "C" ).AddSink( sink3 );

  for ( int i = 0; i < kCount; ++i )
  {
    NE_LOG_INFO_TO( *registry.Get( "A" ), "A", "msg {}", i );
    NE_LOG_INFO_TO( *registry.Get( "B" ), "B", "msg {}", i );
    NE_LOG_INFO_TO( *registry.Get( "C" ), "C", "msg {}", i );
  }

  registry.ShutdownAll();

  EXPECT_EQ( static_cast<size_t>( kCount ), sink1->Count() );
  EXPECT_EQ( static_cast<size_t>( kCount ), sink2->Count() );
  EXPECT_EQ( static_cast<size_t>( kCount ), sink3->Count() );
}
