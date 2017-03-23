
#include "GLES2PixelConversionUtil.h"


using namespace Nebulae;


GLenum GLES2PixelConversionUtil::GetGLOriginFormat(PixelFormat mFormat)
{
  switch(mFormat)
  {
	case PF_A8:
		return GL_ALPHA;
  case PF_L8:
    return GL_LUMINANCE;
  case PF_L16:
    return GL_LUMINANCE;
	case PF_BYTE_LA:
		return GL_LUMINANCE_ALPHA;
	case PF_R3G3B2:
		return GL_RGB;
	//case PF_A1R5G5B5:
	//	return GL_BGRA;
	case PF_R5G6B5:
		return GL_RGB;
	//case PF_B5G6R5:
	//	return GL_BGR;
	//case PF_A4R4G4B4:
	//	return GL_BGRA;
#if NE_ENDIAN == NE_ENDIAN_BIG
  // Formats are in native endian, so R8G8B8 on little endian is
  // BGR, on big endian it is RGB.
  case PF_R8G8B8:
    return GL_RGB;
//  case PF_B8G8R8:
//    return GL_BGR;
#else
//  case PF_R8G8B8:
//    return GL_BGR;
  case PF_B8G8R8:
    return GL_RGB;
#endif
//  case PF_X8R8G8B8:
//  case PF_A8R8G8B8:
//    return GL_BGRA;
  case PF_X8B8G8R8:
  case PF_A8B8G8R8:
    return GL_RGBA;
//  case PF_B8G8R8A8:
//    return GL_BGRA;
  case PF_R8G8B8A8:
    return GL_RGBA;
  //case PF_A2R10G10B10:
  //  return GL_BGRA;
  case PF_A2B10G10R10:
    return GL_RGBA;
  case PF_FLOAT16_R:
    return GL_LUMINANCE;
  case PF_FLOAT16_GR:
    return GL_LUMINANCE_ALPHA;
  case PF_FLOAT16_RGB:
    return GL_RGB;
  case PF_FLOAT16_RGBA:
    return GL_RGBA;
  case PF_FLOAT32_R:
    return GL_LUMINANCE;
  case PF_FLOAT32_GR:
    return GL_LUMINANCE_ALPHA;
  case PF_FLOAT32_RGB:
    return GL_RGB;
  case PF_FLOAT32_RGBA:
    return GL_RGBA;
  case PF_SHORT_RGBA:
    return GL_RGBA;
  case PF_SHORT_RGB:
    return GL_RGB;
  case PF_SHORT_GR:
    return GL_LUMINANCE_ALPHA;
  //case PF_DXT1:
  //  return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
  //case PF_DXT3:
  //  return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
  //case PF_DXT5:
  //  return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
  default:
    return 0;
  }
}

GLenum GLES2PixelConversionUtil::GetGLOriginDataType( PixelFormat mFormat )
{
  switch(mFormat)
  {
  case PF_A8:
  case PF_L8:
  case PF_R8G8B8:
  case PF_B8G8R8:
  case PF_BYTE_LA:
    return GL_UNSIGNED_BYTE;
  //case PF_R3G3B2:
  //  return GL_UNSIGNED_BYTE_3_3_2;
  //case PF_A1R5G5B5:
  //  return GL_UNSIGNED_SHORT_1_5_5_5_REV;
  case PF_R5G6B5:
  case PF_B5G6R5:
	  return GL_UNSIGNED_SHORT_5_6_5;
  //case PF_A4R4G4B4:
  //	return GL_UNSIGNED_SHORT_4_4_4_4_REV;
  case PF_L16:
    return GL_UNSIGNED_SHORT;
#if NE_ENDIAN == NE_ENDIAN_BIG
  case PF_X8B8G8R8:
  //case PF_A8B8G8R8:
  //  return GL_UNSIGNED_INT_8_8_8_8_REV;
  //case PF_X8R8G8B8:
  //case PF_A8R8G8B8:
  //  return GL_UNSIGNED_INT_8_8_8_8_REV;
  case PF_B8G8R8A8:
    return GL_UNSIGNED_BYTE;
  case PF_R8G8B8A8:
	  return GL_UNSIGNED_BYTE;
#else
  case PF_X8B8G8R8:
  case PF_A8B8G8R8:
    return GL_UNSIGNED_BYTE;
  case PF_X8R8G8B8:
  case PF_A8R8G8B8:
	  return GL_UNSIGNED_BYTE;
  //case PF_B8G8R8A8:
  //  return GL_UNSIGNED_INT_8_8_8_8;
  //case PF_R8G8B8A8:
  //  return GL_UNSIGNED_INT_8_8_8_8;
#endif
  //case PF_A2R10G10B10:
  //  return GL_UNSIGNED_INT_2_10_10_10_REV;
  //case PF_A2B10G10R10:
  //  return GL_UNSIGNED_INT_2_10_10_10_REV;
  //case PF_FLOAT16_R:
  //case PF_FLOAT16_GR:
  //case PF_FLOAT16_RGB:
  //case PF_FLOAT16_RGBA:
  //  return GL_HALF_FLOAT_ARB;
  case PF_FLOAT32_R:
  case PF_FLOAT32_GR:
  case PF_FLOAT32_RGB:
  case PF_FLOAT32_RGBA:
    return GL_FLOAT;
  case PF_SHORT_RGBA:
  case PF_SHORT_RGB:
  case PF_SHORT_GR:
	  return GL_UNSIGNED_SHORT;
  default:
    return 0;
  }
}
    
GLenum GLES2PixelConversionUtil::GetGLInternalFormat( PixelFormat format, bool hwGamma )
{
  return GL_NONE;
}

GLenum GLES2PixelConversionUtil::GetClosestGLInternalFormat( PixelFormat mFormat, bool hwGamma )
{
  GLenum format = GetGLInternalFormat( mFormat, hwGamma );
  if( format== GL_NONE )
  {
    //if ( hwGamma )
    //{
    //  return GL_SRGB8;
    //}
    //else
    //{
    //  return GL_RGBA8;
    //}
	  return GL_RGBA;
  }
  
  return format;
}
	
PixelFormat GLES2PixelConversionUtil::GetClosestPixelFormat( GLenum fmt )
{
	switch( fmt )
	{
//	case GL_LUMINANCE8:
//		return PF_L8;
//	case GL_LUMINANCE16:
//		return PF_L16;
//	case GL_ALPHA8:
//		return PF_A8;
//	case GL_LUMINANCE4_ALPHA4:
//		// Unsupported by GL as input format, use the byte packed format
//		return PF_BYTE_LA;
//	case GL_LUMINANCE8_ALPHA8:
//		return PF_BYTE_LA;
//	case GL_R3_G3_B2:
//		return PF_R3G3B2;
	case GL_RGB5_A1:
		return PF_A1R5G5B5;
	default:
		return PF_A8R8G8B8;
	};
} 

size_t GLES2PixelConversionUtil::GetMaxMipmaps(size_t width, size_t height, size_t depth, PixelFormat format)
{
  std::size_t count = 0;
  if( (width > 0) && (height > 0) && (depth > 0) )
  {
    do 
    {
      if( width > 1)  width  /= 2;
      if( height > 1)	height /= 2;
      if( depth > 1)  depth  /= 2;
              
      count ++;
    } while ( !(width == 1 && height == 1 && depth == 1) );
  }		
	return count;
}
