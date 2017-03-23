#ifndef __NEBULAE_HARDWAREBUFFER_H__
#define __NEBULAE_HARDWAREBUFFER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Resource/Resource.h>

namespace Nebulae {

class HardwareBufferImpl;


NE_FLAG_TYPE(HardwareBufferUsage);

/// Static buffer which the application rarely modifies once created.  
/// Modifying the contents of this buffer will involve a performance hit.
extern const HardwareBufferUsage HBU_STATIC; 

/// Indicates the application would like to modify this buffer with the CPU fairly often.  
/// Buffers created with this flag will typically end up in AGP memory rather than video memory.
extern const HardwareBufferUsage HBU_DYNAMIC; 

/// Indicates the application will never read the contents of the buffer back, it will only ever write data.
/// Locking a buffer with this flag will ALWAYS return a pointer to new, blank memory rather than the memory
/// associated with the contents of the buffer; this avoids DMA stalls because you can write to a new memory
/// area while the previous one is being used.
extern const HardwareBufferUsage HBU_WRITE_ONLY;
  
/// Indicates that the application will be refilling the contents of the buffer regularly 
/// (not just updating, but generating the contents from scratch), and therefore does not mind
/// if the contents of the buffer are lost somehow and need to be recreated.
/// This allows an additional level of optimisation on the buffer.  This option only really makes
/// sense when combined with HBU_DYNAMIC_WRITE_ONLY.
extern const HardwareBufferUsage HBU_DISCARDABLE;

/// Combination of HBU_STATIC and HBU_WRITE_ONLY.
extern const Flags<HardwareBufferUsage> HBU_STATIC_WRITE_ONLY;

/// Combination of HBU_DYNAMIC & HBU_WRITE_ONLY.
/// If you use this, strongly consider using HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE instead if you
/// update the entire contents of the buffer very regularly.
extern const Flags<HardwareBufferUsage> HBU_DYNAMIC_WRITE_ONLY;

/// Combination of HBU_DYNAMIC, HBU_WRITE_ONLY and HBU_DISCARDABLE.
extern const Flags<HardwareBufferUsage> HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE;


NE_FLAG_TYPE(LockOptions); 

extern const LockOptions HBL_NORMAL;       ///< Normal mode, ie allows read/write and contents are preserved
extern const LockOptions HBL_DISCARD;      ///< Discards the entire buffer while locking;  This allows optimisation to be performed because synchronisation issues are relaxed.  Only allowed on buffers created with the HBU_DYNAMIC flag.
extern const LockOptions HBL_READ_ONLY;    ///< Lock the buffer for reading only.  Not allowed in buffers which are created with HBU_WRITE_ONLY.  Mandatory on static buffers, ie those created without the HBU_DYNAMIC flag.
extern const LockOptions HBL_NO_OVERWRITE; ///< As HBL_DISCARD, except the application guarantees not to overwrite any region of the buffer which has already been used in this frame, can allow some optimisations on some API's.
extern const LockOptions HBL_WRITE_ONLY;   ///< Lock the buffer for writing only.


enum HardwareBufferBinding
{
  HBB_VERTEX = 0,
  HBB_INDEX,
  HBB_CONSTANT,
  HBB_COUNT
};


class HardwareBuffer : public Resource
///
/// An example of a Facade class; The HardwareBuffer class provides an interface for interacting with 
/// graphical memory in a extendable manner which separates the interface and the implementation for any
/// platform &/or methodology by providing only the interface and a mechanism to attach any
/// implementation.
///
{
private:
  HardwareBufferImpl* m_pImpl;
  bool                m_isLocked;
  std::size_t         m_lockStart;
  std::size_t         m_lockSize;

  public:
    HardwareBuffer( const std::string& name, RenderSystem* renderer );
    virtual ~HardwareBuffer();

    HardwareBufferImpl* GetImpl() const;
    bool                IsLocked() const;

    void  SetImpl( HardwareBufferImpl* impl );
    
    /** Writes data to the buffer from an area of system memory; note that you must
        ensure that your buffer is big enough.
		    @param offset 
          The byte offset from the start of the buffer to start writing
		    @param length 
          The size of the data to write to, in bytes
        @param pSource 
          The source of the data to be written
	      @param discardWholeBuffer 
          If true, this allows the driver to discard the entire buffer when writing,
		      such that DMA stalls can be avoided; use if you can.
     */
    void  WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer = false );

    void* Lock( LockOptions options );
    void* Lock( std::size_t offset, size_t length, LockOptions options );
    void  Unlock();

  private:
    virtual bool LoadImpl_( File* is ) override;
    virtual bool UnloadImpl_() override;

};

} //Nebulae

#endif // __NEBULAE_HARDWAREBUFFER_H__