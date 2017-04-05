
#include "TextureImpl_OGL.h"
#include "GLPixelConversionUtil.h"

#include <Nebulae/Alpha/Texture/Image.h>

#include <FreeImage.h>


using namespace Nebulae;

TextureImpl_OGL::TextureImpl_OGL( const std::string& strFileName )
	: TextureImpl( strFileName )
	, m_iHandle( 0 )
{}


TextureImpl_OGL::~TextureImpl_OGL()
{}


bool 
TextureImpl_OGL::Load( const Image& image )
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
  
  GLenum      glformat  = GLPixelConversionUtil::GetClosestGLInternalFormat( m_pixelFormat );
  bool        hasAlpha  = PixelUtil::HasAlpha( m_pixelFormat );

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
  GLenum      srcFormat = hasAlpha ? GL_RGBA : GL_RGB;
#else 
  GLenum      srcFormat = hasAlpha ? GL_BGRA : GL_BGR;
#endif

  // Generate the handle.
  glGenTextures( 1, &m_iHandle );
	CheckForGLError();
	
	// Make new handle current.
	glBindTexture( GL_TEXTURE_2D, m_iHandle );
	CheckForGLError();

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

  //@note Required for non-power of 2 textures.  Ideally all textures should be pow2, this is a performance issue.
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

  glTexImage2D( 
    GL_TEXTURE_2D, 
    0, 
    glformat,
		(GLsizei)m_width, 
		(GLsizei)m_height, 
    0, 
    srcFormat,
    srcDataType, 
    image.getData() );
	CheckForGLError();

  return true;
}


bool 
TextureImpl_OGL::LoadFromMemory( unsigned char* pImageData, uint8 srcDataType, uint8 channels, int iWidth, int iHeight )
///
/// @remarks
///   We Are Using GL_LUMINANCE_ALPHA To Indicate that We Are Using 2 Channel Data. Used when loading in font glyph data.
///
{
  m_pixelFormat = PF_BYTE_LA;
  m_type        = TextureType::TEX_TYPE_2D;
  m_width       = iWidth;
  m_height      = iHeight;
  m_depth       = 1;

	// Check if this texture already has a handle.
  if( m_iHandle == 0 ) {
	  glGenTextures( 1, &m_iHandle );
		CheckForGLError();
  }

	// Bind the handle to the 2D texture slot.
	glBindTexture( GL_TEXTURE_2D, m_iHandle );
	CheckForGLError();

	// Setup filtering
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  
  GLenum dataType = 0;
  switch( srcDataType ) {
  case 4: dataType = GL_UNSIGNED_BYTE;    break;
  case 8: dataType = GL_UNSIGNED_SHORT;   break;

  default:
    NE_ASSERT( false, "Src Data type not recognized." )( srcDataType );
    dataType = GL_UNSIGNED_BYTE;
    break;
  };

  GLenum srcStructure = 0;
  switch( channels ) {
  case 1: srcStructure = GL_LUMINANCE;        break;
  case 2: srcStructure = GL_LUMINANCE_ALPHA;  break;
  case 4: srcStructure = GL_BGRA;             break; 

  default: 
    NE_ASSERT( false, "Unsupported number of channels.  Unknown src structure." )();
    srcStructure = GL_RGB;
    break;
  }

  glTexImage2D( GL_TEXTURE_2D, 0, srcStructure, iWidth, iHeight, 0, srcStructure, dataType, pImageData );
	CheckForGLError();

  GLenum huboError = glGetError();
  if( huboError == GL_INVALID_ENUM ) {
    NE_BREAKPOINT;
    return false;
  } else if( huboError == GL_INVALID_VALUE ) {
    NE_BREAKPOINT;
    return false;
  } else if( huboError == GL_INVALID_OPERATION ) {
    NE_BREAKPOINT;
    return false;
  }

	return true;
}
