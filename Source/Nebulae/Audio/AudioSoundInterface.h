#ifndef __NEBULAE_AUDIOSOUNDINTERFACE_H__
#define __NEBULAE_AUDIOSOUNDINTERFACE_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Audio/AudioTrackParams.h>

namespace Nebulae
{

//Forward Deceleration.
class AudioDecoder;
class AudioEmitterInterface;

///
/// Interface for manipulating a single sound file. 
///
class AudioSoundInterface
{
protected:
  File*                                m_fileHandle;
  AudioTrackParams                     m_trackParams; ///< Basic track information for this sound.
  AudioDecoder*                        m_decoder;     ///< Decoder to be used to stream in the sound data.
  std::vector< AudioEmitterInterface*> m_emitters;    ///< A list of Emitters that the Sound is currently bound to.

  public:
    AudioSoundInterface( File* filehandle );
    virtual ~AudioSoundInterface();

    const AudioTrackParams& GetTrackParams() const;
    
    void   Clear(); 
    void   SetDecoder( AudioDecoder* decoder );
    void   SetTrackParams( const AudioTrackParams& params );
    uint64 GetDataSize() const;
  
    double GetDuration() const;
    double Tell() const;
    void   Seek( float time );
    int32  Sample( uint32 start, uint32 size, char* buffer );

}; //AudioSoundInterface.

}

#endif // __NEBULAE_AUDIOSOUNDINTERFACE_H__