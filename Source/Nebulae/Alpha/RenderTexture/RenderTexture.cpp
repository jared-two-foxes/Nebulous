
#include "RenderTexture.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/RenderTexture/RenderTextureImpl.h>
#include <Nebulae/Alpha/Texture/Texture.h>

using Nebulae::RenderTexture;

RenderTexture::RenderTexture( const std::string& name, Nebulae::RenderSystem* renderDevice ) 
: Resource( name, renderDevice ),
  m_impl( nullptr ),
  m_colour( nullptr ),
  m_depth( nullptr )
{}

RenderTexture::~RenderTexture()
{
  if( m_impl ) delete m_impl;
  m_impl = nullptr;
}

Nebulae::RenderTextureImpl* 
RenderTexture::GetImpl() const 
{ 
  return m_impl; 
}

std::size_t 
RenderTexture::GetWidth() const 
{ 
  NE_ASSERT( m_colour != nullptr, "Attempting to access an invalid RenderTextureImpl." )();
  return m_colour->GetWidth(); 
}

std::size_t 
RenderTexture::GetHeight() const  
{ 
  NE_ASSERT( m_colour != nullptr, "Attempting to access an invalid RenderTextureImpl." )();
  return m_colour->GetHeight(); 
}

void  
RenderTexture::SetImpl( Nebulae::RenderTextureImpl* impl ) 
{ 
  NE_ASSERT( impl != nullptr, "Attempting to set an invalid RenderTextureImpl." )();
  m_impl = impl; 
}

bool 
RenderTexture::BindColourBuffer( int32 index, int32 width, int32 height )
{
  //@todo handle multiple index binding!

  if( width == -1 )  width  = GetWidth();
  if( height == -1 ) height = GetHeight();

  if( m_impl != nullptr && m_impl->BindColourBuffer(index, width, height) )
  {
    m_colour = nullptr;
    return true;
  }

  return false;
}

bool
RenderTexture::BindColourTexture( int32 index, Nebulae::Texture* texture )
{
  //@todo handle multiple index binding!

  if( texture == m_colour ) return true;
  
  if( m_impl != nullptr && m_impl->BindColourTexture(index, texture) )
  {
    m_colour = texture;
    return true;
  }
 
  return false; 
}

bool 
RenderTexture::BindDepthBuffer( int32 width, int32 height )
{
  if( width == -1 )  width  = (int32)GetWidth();
  if( height == -1 ) height = (int32)GetHeight();

  if( m_impl != nullptr && m_impl->BindDepthBuffer(width, height) )
  {
    m_depth = nullptr;
    return true;
  }

  return false;
}

bool
RenderTexture::BindDepthTexture( Nebulae::Texture* texture )
{
  if( texture == m_depth ) return true;
  
  if( m_impl != nullptr && m_impl->BindDepthTexture(texture) )
  {
    m_depth = texture;
    return true;
  }

  return false;
}

bool 
RenderTexture::LoadImpl_( Nebulae::File* is ) 
{
  NE_ASSERT( m_impl != nullptr, "Attempting to load a RenderTexture without a valid RenderTextureImpl present" )();
  return (m_impl ? m_impl->Load() : false);
}

bool 
RenderTexture::UnloadImpl_() 
{
  NE_ASSERT( m_impl != nullptr, "Attempting to unload a RenderTexture without a valid RenderTextureImpl present" )();
  return (m_impl ? m_impl->Unload() : false);
}
