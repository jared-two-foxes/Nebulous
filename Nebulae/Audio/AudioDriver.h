#ifndef __NEBULAE_AUDIODRIVER_H__
#define __NEBULAE_AUDIODRIVER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
  class AudioBufferInterface;
  class AudioEmitterInterface;
  class AudioSoundInterface;
  
  class AudioDriver
  {
  public:
    virtual ~AudioDriver(){};

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    
    //virtual void Suspend() = 0;
    //virtual void Resume() = 0;
    
    virtual void RampOut() {}
    virtual void RampIn() {}
    
    virtual void Update( const Real dt ){}

    //virtual void       Set3DParameter(s32 paramId, void* param)=0;
    //virtual void       SetDSPParameter(s32 paramId, void* param){};
    //virtual bool       SetOutputMode(VoxOutputMode mode){return false;}
    //virtual OutputMode GetOutputMode() const {return k_nOutputModeStereo;}

    virtual AudioBufferInterface* CreateDriverBuffer() = 0;
    virtual void DestroyDriverBuffer( AudioBufferInterface* sound ) = 0;

    virtual AudioEmitterInterface* CreateDriverEmitter() = 0;
    virtual void DestroyDriverEmitter( AudioEmitterInterface* emitter ) = 0;

    virtual void SetMasterVolume( float volume ) = 0;
    //virtual float GetMasterVolume() const;
  };

}

#endif // __NEBULAE_AUDIODRIVER_H__