
#include "Texture.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Texture/Image.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>

using namespace Nebulae;


Texture::Texture( const std::string& name, RenderSystem* renderDevice ) 
  : Resource( name, renderDevice ),
    m_impl( NULL )
{
}
  

Texture::~Texture() {
  if( m_impl ) {
    delete m_impl;
  }
  m_impl = NULL;
}


TextureImpl* 
Texture::GetImpl() const { 
  return m_impl; 
}


TextureType 
Texture::GetTextureType() const { 
  return m_impl->GetTextureType(); 
}


PixelFormat
Texture::GetFormat() const {
  return m_impl->GetFormat();
}


std::size_t 
Texture::GetWidth() const { 
  return m_impl->GetWidth(); 
}


std::size_t 
Texture::GetHeight() const  { 
  return m_impl->GetHeight(); 
}


std::size_t 
Texture::GetDepth() const { 
  return m_impl->GetDepth(); 
}


void    
Texture::ConvertToImage( Image& destImage, bool includeMipMaps ) const {
  ////@todo implementation needed.
  //std::size_t numMips  = 1; //includeMipMaps? getNumMipmaps() + 1 : 1;
  //std::size_t dataSize = Image::calculateSize( numMips, 1/*GetNumFaces()*/, GetWidth(), GetHeight(), GetDepth(), GetFormat() );

  //void* pixData = new char[dataSize];

  //// if there are multiple faces and mipmaps we must pack them into the data
  //// faces, then mips
  //void* currentPixData = pixData;
  //for( std::size_t face = 0; face < 1/*getNumFaces()*/; ++face ) {
  //  for( std::size_t mip = 0; mip < numMips; ++mip ) {
  //    std::size_t mipDataSize = PixelUtil::getMemorySize( GetWidth(), GetHeight(), GetDepth(), GetFormat() );

  //    PixelBox pixBox( GetWidth(), GetHeight(), GetDepth(), GetFormat(), currentPixData );
  //    //GetBuffer(face, mip)->BlitToMemory(pixBox);

  //    currentPixData = (void*)((char*)currentPixData + mipDataSize);
  //  }
  //}


  //// load, and tell Image to delete the memory when it's done.
  //destImage.loadDynamicImage( (uint8*)pixData, GetWidth(), GetHeight(), GetDepth(), GetFormat(), true, 
  //  1/*GetNumFaces()*/, numMips - 1 );
    
  destImage = m_image;
}


void  
Texture::SetImpl( TextureImpl* impl ) { 
  m_impl = impl; 
}


bool 
Texture::LoadImpl_( File* is ) {
  if( !m_impl || !is ) {
    return false;
  }

  m_image.load( *is );
  
  return m_impl->Load( m_image );
}


bool 
Texture::UnloadImpl_() {
  return (m_impl ? m_impl->Unload() : false);
}


bool 
Texture::LoadFromMemory( unsigned char* pData, uint8 srcDataType, uint8 channels, int iWidth, int iHeight ) {
  // If load has already been called successfully then return that success.
  //if( m_LoadingStatus == LoadingStatus::Loaded ) {
  //	return true;
  //}

  // Flag the resource as attempting to load
  m_loadingStatus = LoadingStatus::Loading;

  bool bRet = m_impl->LoadFromMemory( pData, srcDataType, channels, iWidth, iHeight ); 
  NE_ASSERT( bRet, "Failed to load texture." )(); 
  if( !bRet ) {  
    return false;
  }

  // Flag the asset as loaded
  m_loadingStatus = LoadingStatus::Loaded;		
    
  // Return success
  return true;
}