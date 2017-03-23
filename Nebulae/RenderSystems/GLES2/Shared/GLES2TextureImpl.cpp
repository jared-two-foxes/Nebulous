
#include "GLES2TextureImpl.h"
#include "GLES2PixelConversionUtil.h"

#include <Nebulae/Alpha/Texture/Image.h>
#include <Nebulae/Alpha/Texture/Texture.h>

using namespace Nebulae;


GLES2TextureImpl::GLES2TextureImpl( const std::string& strFileName )
: TextureImpl( strFileName ),
  m_iHandle( 0 )
{
}


GLES2TextureImpl::~GLES2TextureImpl()
{}


bool 
GLES2TextureImpl::Load( const Image& image )
{
  m_pixelFormat = image.getFormat();
  m_type        = TextureType::TEX_TYPE_2D;
  m_width       = image.getWidth();
  m_height      = image.getHeight();
  m_depth       = 1;

  GLenum srcDataType = GL_UNSIGNED_BYTE;
  switch( m_pixelFormat ) {
    default:
    case PF_L8:
    case PF_R5G6B5:
    case PF_A1R5G5B5:
    case PF_BYTE_RGB:
    case PF_BYTE_RGBA:
      srcDataType = GL_UNSIGNED_BYTE;
      break;
    case PF_L16:
    case PF_SHORT_RGB:
    case PF_SHORT_RGBA:
      srcDataType = GL_UNSIGNED_SHORT;
      break;
    case PF_FLOAT32_R:
    case PF_FLOAT32_RGB:
    case PF_FLOAT32_RGBA:
      srcDataType = GL_FLOAT;
      break;
  };
  
  const uint8* srcData   = image.getData();
  Image*       imageCopy = NULL;

  //gles support only a very small subset of image data types.  Particularly it will not support anything thats not GL_UNSIGNED_BYTE so convert the image.
  if( srcDataType != GL_UNSIGNED_BYTE ) {
    imageCopy  = new Image( image );

    if(  PixelUtil::HasAlpha( m_pixelFormat ) ) {
      imageCopy->convert( PF_BYTE_RGBA );
    } else {
      imageCopy->convert( PF_BYTE_RGB );
    }

    m_pixelFormat = imageCopy->getFormat();
    srcDataType   = GL_UNSIGNED_BYTE;
    srcData       = imageCopy->getData();
  }

  GLenum glformat  = GLES2PixelConversionUtil::GetClosestGLInternalFormat( m_pixelFormat );
  bool   hasAlpha  = PixelUtil::HasAlpha( m_pixelFormat );

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
  GLenum      srcFormat = hasAlpha ? GL_RGBA : GL_RGB;
#else 
  GLenum      srcFormat = hasAlpha ? GL_BGRA : GL_BGR;
#endif

  // Generate the handle.
  glGenTextures( 1, &m_iHandle );
  CheckForGLESError();

  glBindTexture( GL_TEXTURE_2D, m_iHandle );
  CheckForGLESError();

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  CheckForGLESError();

  //@note Required for non-power of 2 textures.  Ideally all textures should be pow2, this is a performance issue.
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  CheckForGLESError();

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  CheckForGLESError();

  glTexImage2D( 
    GL_TEXTURE_2D, 
    0, 
    srcFormat, //glformat,
    (GLsizei)m_width,
		(GLsizei)m_height,
    0, 
    srcFormat,
    srcDataType,
    srcData );

  CheckForGLESError();

  if( imageCopy != NULL )
  {
    delete imageCopy;
  }

  return true;
}


bool
GLES2TextureImpl::LoadFromMemory( unsigned char* szBuffer, uint8 srcDataType, uint8 channels, int iWidth, int iHeight )
{
  m_width  = iWidth;
  m_height = iHeight;

  // Check if this texture already has a handle.
  if( m_iHandle == 0 ) {
    glGenTextures( 1, &m_iHandle );
    CheckForGLESError();
  }

  // Bind the handle to the 2D texture slot.
  glBindTexture( GL_TEXTURE_2D, m_iHandle );
  CheckForGLESError();

  // Setup filtering
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  CheckForGLESError();

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  CheckForGLESError();

  GLenum dataType = 0;
  switch( srcDataType ) {
  case 4: dataType = GL_UNSIGNED_BYTE;    break;
  case 8: dataType = GL_UNSIGNED_SHORT;   break;

  default:
    NE_ASSERT( false, "Src Data type not recognized." )();
    dataType = GL_UNSIGNED_BYTE;
    break;
  };

  GLenum srcStructure = 0;
  switch( channels ) {
  case 2: srcStructure = GL_LUMINANCE_ALPHA;  break;
  case 4: srcStructure = GL_RGBA;             break;

  default: 
    NE_ASSERT( false, "Unsupported number of channels.  Unknown src structure." )();
    srcStructure = GL_RGB;
    break;
  }

  glTexImage2D( GL_TEXTURE_2D, 0, srcStructure, iWidth, iHeight, 0, srcStructure, dataType, szBuffer );
  CheckForGLESError();

  return true;
}