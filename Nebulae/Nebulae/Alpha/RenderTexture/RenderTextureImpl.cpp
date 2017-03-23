
#include "RenderTextureImpl.h"

using namespace Nebulae;

RenderTextureImpl::RenderTextureImpl( int32 width, int32 height )
: m_width(width), 
  m_height(height), 
  m_colour(nullptr),
  m_depth(nullptr)
{}

RenderTextureImpl::~RenderTextureImpl()
{}

void 
RenderTextureImpl::BindRenderTexture() 
{}

bool 
RenderTextureImpl::BindColourBuffer( int32 index, int32 width, int32 height )
{
  m_colour = nullptr;
  return false;
}    

bool 
RenderTextureImpl::BindColourTexture( int32 index, Texture* texture ) 
{
  m_colour = texture;
  return false;
}

bool 
RenderTextureImpl::BindDepthBuffer( int32 index, int32 width, int32 height )
{
  m_depth = nullptr;
  return false;
}

bool 
RenderTextureImpl::BindDepthTexture( Texture* texture ) 
{
  m_depth = texture;
  return false;
}

bool 
RenderTextureImpl::Load() 
{ return false; }

bool 
RenderTextureImpl::Unload() 
{ return false; }
