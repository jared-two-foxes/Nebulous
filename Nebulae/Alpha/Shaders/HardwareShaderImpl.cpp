#include "HardwareShaderImpl.h"

using namespace Nebulae;

HardwareShaderImpl::HardwareShaderImpl( const std::string& strFileName, HardwareShaderType eType )
  : m_fileName( strFileName ),
    m_type( eType )
{
}


HardwareShaderImpl::~HardwareShaderImpl()
{
}


HardwareShaderType 
HardwareShaderImpl::GetType() const 
{ return m_type; }


const UniformDefinitionMap& 
HardwareShaderImpl::GetUniformDefinitions() const
{ return m_uniformDefinitions; }


bool 
HardwareShaderImpl::Load( File& is )
{ return false; }


bool 
HardwareShaderImpl::Unload()
{ return false; }