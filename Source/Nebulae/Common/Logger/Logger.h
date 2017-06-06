#ifndef __NEBULAE_COMMON_LOGGER_H__
#define __NEBULAE_COMMON_LOGGER_H__

namespace Nebulae {

class Logger : public Singleton<Logger >
///
/// A simple class that is responsible for maintaining log messages and pushing them to
/// registered output locations.  Valid output locations can include the console, a file, or any
/// other output buffer type.
///
{
private:
  struct Channel;
	typedef std::unique_ptr<Channel > ChannelPtr;

private:
  std::vector<std::string > m_messages;          ///< A list of all the currently untransmitted log messages.
  std::vector<ChannelPtr >  m_channels;          ///< A list of all the registered output channels.
  std::vector<ChannelPtr >  m_immediateChannels; ///< A list of the channels which are to receive log messages immediately.
  
  public:
  	/** Default Constructor. */
    Logger();

    /** Deconstructor. */
    ~Logger();

    /** Removes all of the currently queued messages and any registered output channels.  
    	@return 
    	  Nothing.
	    @note
	      It will not push any of the log messages before removing them thus if you wish to log any
	      remaining messages call Flush before Clear.
     */
    void Clear();

    /** Registers an output channel to which the logger will push log messages, when the message is
        pushed can be modified by setting the immediate flag to true which will push the message 
        immediately to the channel upon recieving it otherwise it will be pushed the next time flush
        is called.
        @param channel
          The output channel.
        @param immediate
    	  Whether the message should be pushed to the channel as soon as it is received.
	  	@return
  		  Nothing.
  	 */
    void Register( std::ostream& channel, bool immediate );

    /** Will remove a output channel from the registered list of output channels.  If the output
    	channel was not already registed it will be silently ignored.
    	@param
		  the channel to remove.
	    @return
    	  Nothing.	
	 */
    void Unregister( std::ostream& channel );

    /**	Add's a message into the logged message's queue.  The message will be pushed to the registered
    	output channels next time flush is called OR straight away if the channel is flagged to receive
    	messages immediately.
    	@param message
    	  the string to add to the messages queue.
	    @return
	      Nothing.
     */
    void Log( const char* message, ... );

    /** Will push all unpushed messages to any channel that has not been flagged to receive immediate
        log messages.
        @return
          Nothing.
     */
    void Flush();

    /** Const accessor for the list of currently unpushed messages.
    	@return
    	  the currently unpushed log messages.
    */
    const std::vector<std::string>& GetMessages() const;

};


#define NE_LOG(message, ... ) Logger::Get().Log( message, __VA_ARGS__ )

}

#endif // __NEBULAE_COMMON_LOGGING_H__