
#include "OpenAudioLibraryDriver.h"

#include <Nebulae/Audio/OpenAL/OpenAudioLibraryBuffer.h>
#include <Nebulae/Audio/OpenAL/OpenAudioLibraryEmitter.h>

using namespace Nebulae;

// the number of AudioBuffers to apply to each emitterQueue.
#define AUDIO_BUFFER_COUNT 4 

void
list_audio_devices( const ALCchar *devices )
{
  //Check for enumeration support.
  ALboolean enumeration = alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" );

	NE_ASSERT( enumeration != AL_FALSE, "enumeration extension not available." )( enumeration );

  if( enumeration )
  {
	  const ALCchar *device = devices, *next = devices + 1;
	  while( device && *device != '\0' && next && *next != '\0' ) 
    {
		  size_t len  = strlen(device);
		  device     += (len + 1);
		  next       += (len + 2);
	  }
  }
}

bool 
SetupListener()
{
  ALfloat listenerPos[]={0.0,0.0,1.0};
  ALfloat listenerVel[]={0.0,0.0,0.0};
  ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

  // Position ...
  alListenerfv( AL_POSITION, listenerPos );
  CheckForAudioErrors("listener position");

  // Velocity ...
  alListenerfv( AL_VELOCITY, listenerVel );
  CheckForAudioErrors("listener velocity");

  // Orientation ...
  alListenerfv( AL_ORIENTATION, listenerOri );
  CheckForAudioErrors("listener orientation");

  //Success.
  return true;
}


OpenAudioLibraryDriver::OpenAudioLibraryDriver()
  : m_device( NULL )
  , m_context( NULL )
{}


OpenAudioLibraryDriver::~OpenAudioLibraryDriver()
{
  Shutdown();
}


bool 
OpenAudioLibraryDriver::Init()
{
//
// Determine the Audio Device to use.
// 
	const ALCchar *defaultDeviceName = 0; //< attempt to pull this from some save setting?

  // If not specified grab the default device to use.
	if( !defaultDeviceName )
  {
		defaultDeviceName = alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
  }

//
// Create Audio device for use.
//
	m_device = alcOpenDevice( defaultDeviceName );
	NE_ASSERT( m_device, "Unable to open audio device %s", defaultDeviceName )();
	if( !m_device ) 
	{
		return false;
	}

//
// Create context to audio device.
//
  m_context = alcCreateContext( m_device, nullptr );
  if( !alcMakeContextCurrent( m_context ) )
  {
    //Failure
    NE_LOG( "failed to make default context\n" );
    return false;
	}

	CheckForAudioErrors("Set Context");

//
// Create the listener object.
//
  SetupListener();

  //Success
  return true;
}


void 
OpenAudioLibraryDriver::Shutdown()
{
  if( m_context )
    alcDestroyContext(m_context);
  m_context = 0;

  if( m_device )
    alcCloseDevice(m_device);
  m_device = 0;
}


AudioBufferInterface* 
OpenAudioLibraryDriver::CreateDriverBuffer()
{
  return new OpenAudioLibraryBuffer( 0 );
}


void 
OpenAudioLibraryDriver::DestroyDriverBuffer( AudioBufferInterface* driverBuffer )
{
  assert( 0 );
}


AudioEmitterInterface* 
OpenAudioLibraryDriver::CreateDriverEmitter()
{
  // Create a couple of AudioBuffers for emitter.
  AudioBufferInterface* buffers[AUDIO_BUFFER_COUNT];
  for( int i = 0; i < AUDIO_BUFFER_COUNT; ++i ) {
    buffers[i] = CreateDriverBuffer();
    buffers[i]->Initiate();
  }
   
  // Create emitter
  OpenAudioLibraryEmitter* emitter = new OpenAudioLibraryEmitter( buffers, AUDIO_BUFFER_COUNT );
  if( !emitter ) {
    return NULL;
  }

  // Initialize
  if( !emitter->Initiate() ) { //shit
    delete emitter;
    return NULL;
  }
    
  return emitter;
}


void 
OpenAudioLibraryDriver::DestroyDriverEmitter( AudioEmitterInterface* emitter ) 
{
  assert( 0 );
}


void 
OpenAudioLibraryDriver::SetMasterVolume( float volume )
{
  //@todo [jared.watt] Add in an assert to bound volume to MaxGain?
  alListenerf( AL_GAIN, volume );
}