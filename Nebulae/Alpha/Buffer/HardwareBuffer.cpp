#include "HardwareBuffer.h"
#include "HardwareBufferImpl.h"

using namespace Nebulae;

///////////////////////////////////////
// HardwareBufferUsage
///////////////////////////////////////
const HardwareBufferUsage Nebulae::HBU_STATIC      (0x0001);
const HardwareBufferUsage Nebulae::HBU_DYNAMIC     (0x0002);
const HardwareBufferUsage Nebulae::HBU_WRITE_ONLY  (0x0004);
const HardwareBufferUsage Nebulae::HBU_DISCARDABLE (0x0008);

namespace {
  bool RegisterHardwareBufferUsage()
  {
    FlagSpec<HardwareBufferUsage>& spec = FlagSpec<HardwareBufferUsage>::instance();
    spec.insert(Nebulae::HBU_STATIC,      "HBU_STATIC",      true);
    spec.insert(Nebulae::HBU_DYNAMIC,     "HBU_DYNAMIC",     true);
    spec.insert(Nebulae::HBU_WRITE_ONLY,  "HBU_WRITE_ONLY",  true);
    spec.insert(Nebulae::HBU_DISCARDABLE, "HBU_DISCARDABLE", true);
    return true;
  }
  bool dummy1 = RegisterHardwareBufferUsage();
}

const Flags<HardwareBufferUsage> Nebulae::HBU_STATIC_WRITE_ONLY             ((Nebulae::HBU_STATIC | Nebulae::HBU_WRITE_ONLY));
const Flags<HardwareBufferUsage> Nebulae::HBU_DYNAMIC_WRITE_ONLY            ((Nebulae::HBU_DYNAMIC | Nebulae::HBU_WRITE_ONLY));
const Flags<HardwareBufferUsage> Nebulae::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE((Nebulae::HBU_DYNAMIC | Nebulae::HBU_WRITE_ONLY | Nebulae::HBU_DISCARDABLE));

NE_FLAGSPEC_IMPL(HardwareBufferUsage);


///////////////////////////////////////
// LockOptions
///////////////////////////////////////
const LockOptions Nebulae::HBL_NORMAL      (0x0001);
const LockOptions Nebulae::HBL_DISCARD     (0x0002);
const LockOptions Nebulae::HBL_READ_ONLY   (0x0004);
const LockOptions Nebulae::HBL_NO_OVERWRITE(0x0008);
const LockOptions Nebulae::HBL_WRITE_ONLY  (0x0010); 

namespace {
  bool RegisterLockOptions()
  {
    FlagSpec<LockOptions>& spec = FlagSpec<LockOptions>::instance();
    spec.insert(Nebulae::HBL_NORMAL,       "HBL_NORMAL",       true);
    spec.insert(Nebulae::HBL_DISCARD,      "HBL_DISCARD",      true);
    spec.insert(Nebulae::HBL_READ_ONLY,    "HBL_READ_ONLY",    true);
    spec.insert(Nebulae::HBL_NO_OVERWRITE, "HBL_NO_OVERWRITE", true);
    spec.insert(Nebulae::HBL_WRITE_ONLY,   "HBL_WRITE_ONLY",   true);  
    return true;
  }
  bool dummy2 = RegisterLockOptions();
}

NE_FLAGSPEC_IMPL(LockOptions);


///////////////////////////////////////
// HardwareBuffer
///////////////////////////////////////

HardwareBuffer::HardwareBuffer( const std::string& name, RenderSystem* renderDevice )
: Resource(name, renderDevice),
  m_pImpl(NULL),
  m_isLocked(false),
  m_lockStart(0),
  m_lockSize(0)
{
}

HardwareBuffer::~HardwareBuffer()
{
  if( m_pImpl ) {
    delete m_pImpl;
  }
  m_pImpl = NULL;
}

HardwareBufferImpl*   
HardwareBuffer::GetImpl() const 
{ return m_pImpl; }

void              
HardwareBuffer::SetImpl( HardwareBufferImpl* impl )
{ m_pImpl = impl; }

bool 
HardwareBuffer::LoadImpl_( File* is )
{
  return (m_pImpl ? m_pImpl->Load() : false);
}

bool
HardwareBuffer::UnloadImpl_()
{
  if( !m_pImpl )
  {
    return  false;
  }

  return m_pImpl->Unload();
}

//LoadData
//@todo: Handle the case where the incoming data is larger/smaller than the current buffer? 
void
HardwareBuffer::WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer )
{
  NE_ASSERT( m_pImpl != nullptr, "Attempting to load data to a buffer which has no impl pointer." )();

  if( m_pImpl != nullptr ) 
  {
    m_pImpl->WriteData( offset, length, pSource, discardWholeBuffer );
  }
}

bool 
HardwareBuffer::IsLocked() const
{ return m_isLocked; }

void*
HardwareBuffer::Lock( std::size_t offset, size_t length, LockOptions options )
{
  NE_ASSERT( m_pImpl != nullptr, "Attempting to lock a buffer which has no impl pointer." )();
  NE_ASSERT( !IsLocked(), "Cannot lock this buffer, it is already locked!" )();
  //NE_ASSERT( (length + offset) < mSizeInBytes, "Lock request out of bounds." )();
	
  void* ret = NULL;
	
	// Lock the real buffer if there is no shadow buffer 
  if( m_pImpl ) 
  {
    ret = m_pImpl->Lock( offset, length, options );
  }

  m_isLocked  = true;
  m_lockStart = offset;
	m_lockSize  = length;
  
  return ret;
}

void* 
HardwareBuffer::Lock( LockOptions options )
{
  NE_ASSERT( m_pImpl != nullptr, "Attempting to lock a buffer which has no impl pointer." )();

  if( m_pImpl ) 
  {
    return m_pImpl->Lock( 0, m_pImpl->m_SizeInBytes, options );
  }

  return nullptr;
}

void 
HardwareBuffer::Unlock()
{
  NE_ASSERT( m_pImpl != NULL, "Attempting to unlock a buffer which has no impl pointer." )();
  NE_ASSERT( IsLocked(), "Cannot unlock this buffer, it is not locked!" )();

  if( m_pImpl ) 
  {
    m_pImpl->Unlock();
  }

  m_isLocked  = false;
  m_lockStart = 0;
  m_lockSize  = 0;
}