#ifndef __NEBULAE_OGG_AUDIODECODER_H__
#define __NEBULAE_OGG_AUDIODECODER_H__

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include <Nebulae/Common/Common.h>

#include <Nebulae/Audio/AudioDecoder.h>

namespace Nebulae
{

class OggAudioDecoder : public AudioDecoder
{
private:
  OggVorbis_File  vf;
  int             current_section;

  public:
    OggAudioDecoder();
    virtual ~OggAudioDecoder();

    virtual void   Clear() override;
    virtual bool   Parse( File* datastream ) override;
    virtual int32  Stream( uint32 start, uint32 size, char* data ) override;
    virtual bool   Seek( Real time ) override;
    virtual double Tell() const override;
    
    virtual double GetDuration() const override;
    
};
  
}

#endif //  __NEBULAE_OGG_AUDIODECODER_H__