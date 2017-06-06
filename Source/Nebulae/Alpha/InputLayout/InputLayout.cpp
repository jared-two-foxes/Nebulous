
#include "InputLayout.h"
#include "InputLayoutImpl.h"
#include "VertexDeceleration.h"

using Nebulae::InputLayout;

InputLayout::InputLayout( const std::string& name, Nebulae::RenderSystem* renderDevice )
: Resource( name, renderDevice ),
  m_impl( nullptr )
{}

InputLayout::~InputLayout()
{
	if( m_impl ) delete m_impl;
	m_impl = nullptr;
}

void  
InputLayout::SetImpl( Nebulae::InputLayoutImpl* impl )
{ 
  m_impl = impl; 
}

Nebulae::InputLayoutImpl* 
InputLayout::GetImpl() const 
{ 
  return m_impl; 
}

bool 
InputLayout::LoadImpl_( Nebulae::File* is )
{
  return m_impl ? m_impl->Load() : false;
}

bool
InputLayout::UnloadImpl_()
{
  return m_impl ? m_impl->Unload() : false;
}