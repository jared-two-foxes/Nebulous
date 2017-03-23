
#include <Nebulae/Common/Common.h>

#include <Nebulae/Common/Window/Win32/Win32Includes.h>



using namespace Nebulae;


struct Clock::ClockImpl
{
  LARGE_INTEGER m_previous;
  uint64        m_currentTicks;
  double        m_frequency;
  uint64        m_elapsed;

  void Init()
  {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency( &frequency );
    QueryPerformanceCounter( &m_previous );
    
    m_frequency    = 1000000.0f / frequency.QuadPart; //< get frequency in microseconds.
    m_currentTicks = 0;
    m_elapsed      = 0;
  }

  void Mark()
  {
    LARGE_INTEGER current;
    QueryPerformanceCounter( &current );
    uint64 elapsed  = current.QuadPart - m_previous.QuadPart;
    m_elapsed       = uint64(elapsed * m_frequency);
    m_currentTicks += elapsed;
    m_previous      = current;
  }
    
  uint64 GetAccurateTime() const
  {
    LARGE_INTEGER current;
    QueryPerformanceCounter( &current );
    
    uint64 elapsed  = current.QuadPart - m_previous.QuadPart;
    
    return uint64( ( m_currentTicks + elapsed ) * m_frequency );
  }

  uint64 GetElapsedTime() const
  { return m_elapsed; }

  uint64 GetUpTime() const
  { return uint64( m_currentTicks * m_frequency ); }

};

Clock::Clock()
{
  m_impl = new ClockImpl;
  m_impl->Init();
}

Clock::~Clock()
{
  if( m_impl )
  {
    delete m_impl;
    m_impl = NULL;
  }
}

void 
Clock::Init()
{ m_impl->Init(); }

void  
Clock::MarkThisTick()
{ m_impl->Mark(); }

uint64  
Clock::GetAccurateTime() const
{ return m_impl->GetAccurateTime(); }

uint64  
Clock::GetElapsedTime() const
{ return m_impl->GetElapsedTime(); }

uint64  
Clock::GetUpTime() const
{ return m_impl->GetUpTime(); }