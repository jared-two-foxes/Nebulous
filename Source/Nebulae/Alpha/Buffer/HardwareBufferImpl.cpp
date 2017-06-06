
#include "HardwareBufferImpl.h"


using namespace Nebulae;


HardwareBufferImpl::HardwareBufferImpl( const Flags<HardwareBufferUsage>& usage, std::size_t sizeInBytes, HardwareBufferBinding bindFlags, void* pSysMem ) 
  : m_Usage( usage ),
    m_SizeInBytes( sizeInBytes ),
    m_BindFlags( bindFlags ), 
    m_SysMem( pSysMem ) 
{}


HardwareBufferImpl::~HardwareBufferImpl() 
{}


bool 
HardwareBufferImpl::Load() 
{ return false; }


bool
HardwareBufferImpl::Unload() 
{ return false; }


void 
HardwareBufferImpl::WriteData( std::size_t offset, std::size_t length, const void* pSource, bool discardWholeBuffer )
{}


void* 
HardwareBufferImpl::Lock( std::size_t offset, size_t length, LockOptions options )
{ return nullptr; }


void 
HardwareBufferImpl::Unlock()
{}

