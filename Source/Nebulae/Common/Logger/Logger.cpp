
#include <Nebulae/Common/Common.h>

using namespace Nebulae;

struct Logger::Channel 
{
  std::ostream* m_os;

  Channel( std::ostream* os ) 
	: m_os(os) {}

  bool operator ==( const std::ostream& os )
  {
    return (m_os == &os);
  }

  Channel& operator <<( const std::string& message )
  {
    *m_os << message;
    return *this;
  }
};

Logger::Logger()
: Singleton<Logger>()
{}

Logger::~Logger()
{
  Clear();
}

void
Logger::Clear()
{
  m_messages.clear();
  m_channels.clear();
  m_immediateChannels.clear();
}

void 
Logger::Register( std::ostream& os, bool immediate )
{
  if( immediate )
  {
    m_immediateChannels.push_back( std::make_unique<Channel >( &os ) );
  }
  else
  {
    m_channels.push_back( std::make_unique<Channel >( &os ) );
  }
}

void 
Logger::Unregister( std::ostream& os )
{
  std::vector<ChannelPtr >::iterator it;
  
  //it = std::find( m_immediateChannels.begin(), m_immediateChannels.end(), Channel(os) );
  //if( it != m_immediateChannels.end() )
  //{
  //  m_immediateChannels.erase( it );
  //}

  //it = std::find( m_channels.begin(), m_channels.end(), Channel(os) );
  //if( it != m_channels.end() )
  //{
  //  m_channels.erase( it );
  //}
}

void
Logger::Log( const char* message, ... )
{ 
  if( message != nullptr && strlen(message) > 0 )
  {
    va_list arglist;
    va_start( arglist, message );

    // format string.
    char buffer[1024];
    vsprintf( buffer, message, arglist );

    m_messages.push_back( buffer );

    // Push the message to the immediate Channels.
   	for( ChannelPtr& channel : m_immediateChannels )
  	{
  	  *channel << buffer << "\n";
  	} 

    va_end( arglist );
  }
}

void
Logger::Flush()
{
  // Push the message to the secondary Channels.
  for( std::string message : m_messages )
  {
  	for( ChannelPtr& channel : m_channels )
  	{
      *channel << message << "\n";
  	}
  }

  // only clear the message queue if we have actually pushed this message somewhere.  Assumes the message was pushed
  // if there is at least one immediate or secondary Channel's.
  if( !m_channels.empty() || !m_immediateChannels.empty() )
  {
    m_messages.clear();
  }
}

const std::vector<std::string>& 
Logger::GetMessages() const
{ return m_messages; }