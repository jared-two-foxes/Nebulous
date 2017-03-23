
#include "Image.h"

#include <Nebulae/Alpha/Texture/FreeImageCodec.h>


using namespace Nebulae;


Image::Image() 
  : m_buffer(0) {}


Image::~Image() {
  freeMemory();
}


Image::Image( const Image& img ) {
	// call assignment operator
	*this = img;
}


void 
Image::freeMemory() {
	if( m_buffer ) {
    delete [] m_buffer;
  }
  m_buffer = NULL;
}


Image&
Image::operator =( const Image &img ) {
	m_width        = img.m_width;
	m_height       = img.m_height;
	//m_depth      = img.m_depth;
	m_format       = img.m_format;
	m_bufSize      = img.m_bufSize;
	//m_flags      = img.m_flags;
	m_pixelSize    = img.m_pixelSize;
	//m_numMipmaps = img.m_numMipmaps;

	m_buffer = new uint8[m_bufSize];
	memcpy( m_buffer, img.m_buffer, m_bufSize );
	
	return *this;
}


Image& 
Image::flipAroundY() {
  if( !m_buffer ) {
    NE_ASSERT( false, "Can not flip an unitialized texture" )();
  }
  
  //mNumMipmaps = 0; // Image operations lose precomputed mipmaps
  
  uint8  *pTempBuffer1 = NULL;
  uint16 *pTempBuffer2 = NULL;
  uint8  *pTempBuffer3 = NULL;
  uint32 *pTempBuffer4 = NULL;

  uint8  *src1 = m_buffer, *dst1 = NULL;
  uint16 *src2 = (uint16 *)m_buffer, *dst2 = NULL;
  uint8  *src3 = m_buffer, *dst3 = NULL;
  uint32 *src4 = (uint32 *)m_buffer, *dst4 = NULL;

  uint16 y;
  switch (m_pixelSize)
  {
  case 1:
    pTempBuffer1 = new uint8[m_width * m_height];
    for( y = 0; y < m_height; y++ ) {
      dst1 = (pTempBuffer1 + ((y * m_width) + m_width - 1));
      for (uint16 x = 0; x < m_width; x++) {
        memcpy(dst1--, src1++, sizeof(uint8));
      }
    }

    memcpy(m_buffer, pTempBuffer1, m_width * m_height * sizeof(uint8));
    delete [] pTempBuffer1;
    break;

  case 2:
    pTempBuffer2 = new uint16[m_width * m_height];
    for (y = 0; y < m_height; y++) {
      dst2 = (pTempBuffer2 + ((y * m_width) + m_width - 1));
      for( uint16 x = 0; x < m_width; x++ ) {
        memcpy( dst2--, src2++, sizeof(uint16) );
      }
    }

    memcpy(m_buffer, pTempBuffer2, m_width * m_height * sizeof(uint16));
    delete [] pTempBuffer2;
    break;

  case 3:
    pTempBuffer3 = new uint8[m_width * m_height * 3];
    for( y = 0; y < m_height; y++ ) {
      std::size_t offset = ((y * m_width) + (m_width - 1)) * 3;
      dst3 = pTempBuffer3;
      dst3 += offset;
      for( std::size_t x = 0; x < m_width; x++ ) {
        memcpy(dst3, src3, sizeof(uint8) * 3);
        dst3 -= 3; src3 += 3;
      }
    }

    memcpy( m_buffer, pTempBuffer3, m_width * m_height * sizeof(uint8) * 3 );
    delete [] pTempBuffer3;
    break;

  case 4:
    pTempBuffer4 = new uint32[m_width * m_height];
    for( y = 0; y < m_height; y++ ) {
      dst4 = (pTempBuffer4 + ((y * m_width) + m_width - 1));
      for( uint16 x = 0; x < m_width; x++ ) {
        memcpy(dst4--, src4++, sizeof(uint32) );
      }
    }

    memcpy(m_buffer, pTempBuffer4, m_width * m_height * sizeof(uint32));
    delete [] pTempBuffer4;
    break;

  default:
    NE_ASSERT( false, "Unknown pixel depth" )();
    break;
  }

  return *this;

}


Image& 
Image::flipAroundX()
{
  if( !m_buffer )
  {
    NE_ASSERT( false, "Can not flip an unitialized texture" )();
  }
      
  //mNumMipmaps = 0; // Image operations lose precomputed mipmaps

  std::size_t rowSpan = m_width * m_pixelSize;

  uint8 *pTempBuffer = new uint8[rowSpan * m_height];
  uint8 *ptr1 = m_buffer, *ptr2 = pTempBuffer + ( ( m_height - 1 ) * rowSpan );

  for( uint16 i = 0; i < m_height; i++ ) {
    memcpy( ptr2, ptr1, rowSpan );
    ptr1 += rowSpan; ptr2 -= rowSpan;
  }

  memcpy( m_buffer, pTempBuffer, rowSpan * m_height );

  delete [] pTempBuffer;

  return *this;
}


Image&
Image::load( File& is ) {
  ImageCodec*     codec = new FreeImageCodec();
  ImageCodecData* data  = codec->Decode( is );
  if( data ) {
    m_width      = data->width;
    m_height     = data->height;
    m_bufSize    = data->size;
    m_format     = data->format;
    m_pixelSize  = static_cast<uint8>( PixelUtil::GetNumElemBytes(m_format) );
  
    // Store the image data.
    m_buffer = new uint8[ m_bufSize ];
    memcpy( m_buffer, data->datastream, m_bufSize );

    // delete the data being pointed to by the ImageCodeData;
    delete [] data->datastream;
    delete data;
  }

  delete codec;
  
  return *this;
}


Image& 
Image::loadDynamicImage( uint8* data, std::size_t width, std::size_t height, 
std::size_t depth, PixelFormat format, bool autoDelete, std::size_t numFaces, 
std::size_t numMipMaps) {
  m_width      = width;
  m_height     = height;
  //m_depth      = depth;
  m_format     = format;
  m_pixelSize  = static_cast<uint8>( PixelUtil::GetNumElemBytes(m_format) );
  //m_numMipmaps = numMipMaps;
  m_bufSize    = calculateSize( numMipMaps, numFaces, width, height, depth, format );
  m_buffer     = data;

  return *this;
}


Image&
Image::convert( PixelFormat conversionFormat ) {
  ImageCodec*     codec = new FreeImageCodec();
  std::size_t     pitch = m_width * PixelUtil::GetNumElemBytes(m_format);
  ImageCodecData* data  = codec->ConvertFromRawBits( m_buffer, m_format, m_width, m_height, pitch, m_pixelSize * 8, conversionFormat );
  if( data ) {
    m_width      = data->width;
    m_height     = data->height;
    m_bufSize    = data->size;
    m_format     = data->format;
    m_pixelSize  = static_cast<uint8>( PixelUtil::GetNumElemBytes(m_format) );
  
    // Store the image data.
    m_buffer = new uint8[ m_bufSize ];
    memcpy( m_buffer, data->datastream, m_bufSize );
  
    // delete the data being pointed to by the ImageCodeData;
    delete [] data->datastream;
    delete data;
  }

  delete codec;
  
  return *this;
}


uint8* 
Image::getData() { return 
  m_buffer; 
}


const uint8* Image::getData() const { 
  return m_buffer; 
}


std::size_t Image::getSize() const { 
  return m_bufSize;
}


std::size_t Image::getWidth() const { 
  return m_width; 
}


std::size_t Image::getHeight() const { 
  return m_height; 
}


std::size_t Image::getRowSpan() const { 
  return 0; 
}


PixelFormat Image::getFormat() const { 
  return m_format; 
}


uint8 Image::getBPP() const { 
  return m_pixelSize; 
}


bool Image::hasAlpha() const { 
  return false; 
}


Colour 
Image::getColourAt( std::size_t x, std::size_t y ) const {
  Colour rval(CLR_ZERO);
  if( x < m_width && y < m_height ) {
    PixelUtil::UnpackColour( &rval, m_format, &m_buffer[m_pixelSize * (m_width * y + x)] );
  }
  return rval;
}


std::size_t
Image::calculateSize( std::size_t mipmaps, std::size_t faces, std::size_t width, std::size_t height, std::size_t depth, PixelFormat format ) {
  std::size_t size = 0;
  for( std::size_t mip = 0; mip <= mipmaps; ++mip ) {
    size += PixelUtil::GetMemorySize( width, height, depth, format ) * faces; 
    if( width != 1 ) {
      width /= 2;
    }
    if( height != 1 ) {
      height /= 2;
    }
    if( depth != 1 ) {
      depth /= 2;
    }
  }
  return size;
}