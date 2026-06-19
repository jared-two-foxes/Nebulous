
#include "TextureImpl.h"

using namespace Nebulae;


TextureImpl::TextureImpl( std::string strFileName ) : m_filename( strFileName ) {}


TextureImpl::~TextureImpl() {}


TextureType TextureImpl::GetTextureType() const { return m_type; }


PixelFormat TextureImpl::GetFormat() const { return m_pixelFormat; }


std::size_t TextureImpl::GetWidth() const { return m_width; }


std::size_t TextureImpl::GetHeight() const { return m_height; }


std::size_t TextureImpl::GetDepth() const { return m_depth; }


bool TextureImpl::Load( const Image& image ) { return false; }


bool TextureImpl::LoadFromMemory( unsigned char* szBuffer, uint8 srcDataType, uint8 channels, int iWidth, int iHeight )
{
  return false;
}


bool TextureImpl::Unload() { return false; }
