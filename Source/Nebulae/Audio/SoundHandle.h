#ifndef __NEBULAE_SOUNDHANDLE_H__
#define __NEBULAE_SOUNDHANDLE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {
  
class AudioBackend;
class AudioSoundInterface;

/** Simple wrapper which provides an identifier for a "Sound".
 */ 
class SoundHandle
{
  friend class AudioBackend;
  
private:
  std::size_t          m_id;
  AudioBackend*        m_pBackend;
  AudioSoundInterface* m_pInternal;

  public:
    virtual ~SoundHandle() {}
    inline SoundHandle() : m_id(-1), m_pBackend(NULL), m_pInternal(NULL) {}
    inline SoundHandle( const SoundHandle& rhs ) : m_id(rhs.m_id), m_pBackend(rhs.m_pBackend), m_pInternal(rhs.m_pInternal) {}
    SoundHandle& operator=(const SoundHandle &rhs) {
      m_id        = rhs.m_id;
      m_pBackend  = rhs.m_pBackend;
      m_pInternal = rhs.m_pInternal;
      return (*this);
    }
    
  protected:
    SoundHandle( std::size_t id, AudioBackend* pBackend = 0, AudioSoundInterface* object = 0 ) {
      m_id        = id;
      m_pBackend  = pBackend;
      m_pInternal = object;
    }

}; //SoundHandle

}

#endif // __NEBULAE_SOUNDHANDLE_H__