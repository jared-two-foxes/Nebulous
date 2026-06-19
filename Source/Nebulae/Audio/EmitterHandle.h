#ifndef NEBULAE_AUDIO_EMITTERHANDLE_H_
#define NEBULAE_AUDIO_EMITTERHANDLE_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class AudioBackend;
class AudioEmitterInterface;

/** Simple wrapper which provides an identifier for an Emitter.
 */
class EmitterHandle
{
  friend class AudioBackend;

private:
  std::size_t m_id;
  AudioBackend* m_pBackend;
  AudioEmitterInterface* m_pInternal;

public:
  virtual ~EmitterHandle() {}

  EmitterHandle() : m_id( -1 ), m_pBackend( nullptr ), m_pInternal( nullptr ) {}

  EmitterHandle( const EmitterHandle& rhs )
    : m_id( rhs.m_id ), m_pBackend( rhs.m_pBackend ), m_pInternal( rhs.m_pInternal )
  {
  }

  EmitterHandle& operator=( const EmitterHandle& rhs )
  {
    m_id = rhs.m_id;
    m_pBackend = rhs.m_pBackend;
    m_pInternal = rhs.m_pInternal;
    return ( *this );
  }

  bool operator==( const EmitterHandle& rhs ) { return ( m_id == rhs.m_id ); }

protected:
  EmitterHandle( std::size_t id, AudioBackend* pBackend = nullptr, AudioEmitterInterface* object = nullptr )
  {
    m_id = id;
    m_pBackend = pBackend;
    m_pInternal = object;
  }
};

} // namespace Nebulae

#endif // NEBULAE_AUDIO_EMITTERHANDLE_H_