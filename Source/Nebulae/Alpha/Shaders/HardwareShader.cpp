
#include "HardwareShader.h"
#include "HardwareShaderImpl.h"

using Nebulae::HardwareShader;
using Nebulae::HardwareShaderImpl;

HardwareShader::HardwareShader( const std::string& strFileName, Nebulae::RenderSystem* renderDevice ) 
  : Resource( strFileName, renderDevice )
  , m_impl( nullptr )
{
}

HardwareShader::~HardwareShader()
{
  if( m_impl ) delete m_impl;
  m_impl = nullptr;
}

HardwareShaderImpl* 
HardwareShader::GetImpl() const 
{ 
  return m_impl; 
}

void  
HardwareShader::SetImpl( HardwareShaderImpl* impl )
{ 
  m_impl = impl; 
}

bool
HardwareShader::LoadImpl_( Nebulae::File* is )
{
  return m_impl ? m_impl->Load( *is ) : false;
}

bool
HardwareShader::UnloadImpl_()
{
  return m_impl ? m_impl->Unload() : false;
}
