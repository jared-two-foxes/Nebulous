#ifndef __NEBULAE_OPENAUDIOLIBRARYDRIVER_H__
#define __NEBULAE_OPENAUDIOLIBRARYDRIVER_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Audio/AudioDriver.h>
#include <Nebulae/Audio/OpenAL/CommonHeaders.h>

namespace Nebulae {
  
class OpenAudioLibraryDriver : public AudioDriver
{
private:
  ALCdevice*  m_device;
  ALCcontext* m_context;

  public:
    OpenAudioLibraryDriver();
    virtual ~OpenAudioLibraryDriver();

    virtual bool Init() override;
    virtual void Shutdown() override;
    
    //virtual void Suspend()=0;
    //virtual void Resume()=0;
    //virtual void Update( const Real dt ){}
    //virtual void Set3DParameter(s32 paramId, void* param)=0;
    //virtual void SetDSPParameter(s32 paramId, void* param){};
    //virtual bool SetOutputMode(VoxOutputMode mode){return false;}
    //virtual VoxOutputMode GetOutputMode(){return k_nOutputModeStereo;}

    virtual AudioBufferInterface* CreateDriverBuffer() override;
    virtual void DestroyDriverBuffer( AudioBufferInterface* driverSource ) override;

    virtual AudioEmitterInterface* CreateDriverEmitter() override;
    virtual void DestroyDriverEmitter( AudioEmitterInterface* emitter ) override;

    virtual void SetMasterVolume( float volume ) override;

};

}

#endif // __NEBULAE_OPENAUDIOLIBRARYDRIVER_H__