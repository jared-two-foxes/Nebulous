#ifndef NEBULAE_AUDIO_AUDIODECODER_H_
#define NEBULAE_AUDIO_AUDIODECODER_H_

#include <Nebulae/Common/Common.h>

#include <Nebulae/Audio/AudioTrackParams.h>

namespace Nebulae
{


class AudioDecoder
///
/// Defines the interface used for reading audio data.  Each AudioDecoder will parse a
/// single file of a specified type.
///
{
protected:
  File* m_stream;                 ///< file stream object.
  AudioTrackParams m_trackParams; ///< Basic track information.

public:
  AudioDecoder() : m_stream( NULL ) {}
  virtual ~AudioDecoder() {}

  /** Mutators */
  virtual void Clear() {}
  virtual bool Parse( File* datastream ) = 0;
  virtual int32 Stream( uint32 start, uint32 size, char* data ) = 0;
  virtual double Tell() const { return 0.0; }
  virtual bool Seek( Real time ) { return false; }

  /** Accessors */
  const AudioTrackParams& GetTrackParams() const { return m_trackParams; }
  virtual double GetDuration() const { return 0.0; }
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_AUDIODECODER_H_