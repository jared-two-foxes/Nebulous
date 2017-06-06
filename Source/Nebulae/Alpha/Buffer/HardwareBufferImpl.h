#ifndef NEBULAE_ALPHA_HARDWAREBUFFERIMPL_H__
#define NEBULAE_ALPHA_HARDWAREBUFFERIMPL_H__

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>


namespace Nebulae {


/** The interface required for the implementation of the HardwareBuffer.
 */
class HardwareBufferImpl {

  public:
    Flags<HardwareBufferUsage>   m_Usage;
    std::size_t                  m_SizeInBytes;
    HardwareBufferBinding        m_BindFlags;
    void*                        m_SysMem;

    public:
      HardwareBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* pSysMem );
      virtual ~HardwareBufferImpl();

      virtual bool  Load();
      virtual bool  Unload();
      virtual void  WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer );
      virtual void* Lock( std::size_t offset, size_t length, LockOptions options );
      virtual void  Unlock();

}; 

}

#endif // NEBULAE_ALPHA_HARDWAREBUFFERIMPL_H__