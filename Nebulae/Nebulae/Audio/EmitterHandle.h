#ifndef __NEBULAE_EMITTERHANDLE_H__
#define __NEBULAE_EMITTERHANDLE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class AudioBackend;
class AudioEmitterInterface;

/** Simple wrapper which provides an identifier for an Emitter.
 */ 
class EmitterHandle
{
  friend class AudioBackend;

private:
	std::size_t            m_id;
  AudioBackend*          m_pBackend;
  AudioEmitterInterface* m_pInternal;

  public:
    virtual ~EmitterHandle() {}
    
    EmitterHandle() : m_id(-1), m_pBackend(NULL), m_pInternal(NULL) {}

    EmitterHandle( const EmitterHandle& rhs ) : m_id(rhs.m_id), m_pBackend(rhs.m_pBackend), m_pInternal(rhs.m_pInternal) {}
    
    EmitterHandle& operator=( const EmitterHandle &rhs ) {
      m_id        = rhs.m_id;
      m_pBackend  = rhs.m_pBackend;
      m_pInternal = rhs.m_pInternal;
      return (*this);
    }
  
  bool operator ==( const EmitterHandle& rhs ) {
    return (m_id == rhs.m_id);
  }
    
  protected:
    EmitterHandle( std::size_t id, AudioBackend* pBackend = 0, AudioEmitterInterface* object = 0 ) {
      m_id        = id;
      m_pBackend  = pBackend;
      m_pInternal = object;
    }

}; 

}

#endif // __NEBULAE_EMITTERHANDLE_H__