#ifdef NEBULAE_INCLUDES_PNG

#include "PngCodec.h"

#include <png.h>


using namespace Nebulae;

namespace 
{
  void 
  user_read_data( png_structp png_ptr, png_bytep data, png_size_t length )
  {
    void* read_io_ptr = png_get_io_ptr( png_ptr );
    if( !read_io_ptr )
    {
      // That's unfortunate, we dont seem to have an io object
      return;
    }
     
    Nebulae::File* pFile = static_cast<Nebulae::File* >(  read_io_ptr );
    NE_ASSERT( pFile, "Unable to retrieve the File pointer while attempting to read texture file." )();
    const std::size_t bytesRead = pFile->Read( data, length );

    if( bytesRead != length)
    {
      return;
    }

    //The user_read_data() function is responsible for detecting and handling end-of-data errors.
  }


  Nebulae::PixelFormat 
  EnginePixelFormatFromPng( int colorType, int bitDepth )
  {
    if( colorType == PNG_COLOR_TYPE_GRAY )              // possible bit-depths; 1, 2, 4, 8, 16
    {
      if( bitDepth == 8 ) 
        return PF_L8;
      else if ( bitDepth == 16 )
        return PF_L16;
    }
    else if ( colorType == PNG_COLOR_TYPE_GRAY_ALPHA )  // possible bit-depths; 8, 16
    {
      if (bitDepth == 8)
        return PF_BYTE_LA;
      //else if (bitDepth == 16)
      //  return PF_L16A16; //??
    }
    else if ( colorType == PNG_COLOR_TYPE_RGB )         // possible bit-depths; 8, 16
    {
      if( bitDepth == 8 ) 
        return PF_BYTE_BGR;
      else if ( bitDepth == 16 )
        return PF_FLOAT16_RGB; //> This is actually probably incorrect, should be BGR?
    }
    else if ( colorType == PNG_COLOR_TYPE_RGB_ALPHA )   // possible bit-depths; 8, 16
    {
      if (bitDepth == 8)  
        return PF_B8G8R8A8;
      else if (bitDepth == 16) 
        return PF_FLOAT16_RGBA; //> This is actually probably incorrect, should be BGR?
    }
      
    return PF_UNKNOWN;
  }

  int 
  GetChannelsFromColorType( int colorType )
  {
    if ( colorType == PNG_COLOR_TYPE_GRAY )
    {
      return 1;
    }
    else if ( colorType == PNG_COLOR_TYPE_GRAY_ALPHA )
    {
      return 2;
    }
    else if ( colorType == PNG_COLOR_TYPE_RGB )
    {
      return 3;
    }
    else if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
    {
      return 4;
    }

    // Assume that we received PNG_COLOR_TYPE_PALETTE which shouldn't be possible as we switch this type to RGB.
    return 0;
  }
}

PngCodec::PngCodec()
///
/// Constructor
///
{}


PngCodec::~PngCodec()
///
/// Destructor.
///
{}


ImageCodecData*
PngCodec::Decode( File& is ) const
///
/// Attempts to interpret the contents of the filestream as an Image.
///
/// @param is
///   The datastream.
///
/// @return
///   The image data if it could be read.
///
{
  // Check if this is a png file.
  enum { kPngSignatureLength = 8 };
  png_byte pngSignature[kPngSignatureLength];
  is.Read( &pngSignature[0], kPngSignatureLength );
  if( png_sig_cmp( pngSignature, 0, 8 ) )
  {
    // file is not a png, bail
    return nullptr;
  }

  // Initialize internal processing structures used by libpng
  png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr ); // the nulls are error function callbacks, 
  if( !png_ptr )
  {
    return nullptr;
  }

  png_infop info_ptr = png_create_info_struct( png_ptr );
  if( !info_ptr ) 
  {
    png_destroy_read_struct( &png_ptr, nullptr, nullptr );
    return nullptr;
  }

  // Set the Read function to be the custom function supplied above.
  png_set_read_fn( png_ptr, &is, user_read_data );
  png_set_sig_bytes( png_ptr, 8 );

  png_read_info( png_ptr, info_ptr );

  png_uint_32 width, height;
  int bitDepth, colorType, interlace_type;
  png_get_IHDR( png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, 
    &interlace_type, NULL, NULL );

//  // Tell libpng to strip 16 bit/color files down to 8 bits/color.
//  // Use accurate scaling if it's available, otherwise just chop off the
//  // low byte.
//#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
//  png_set_scale_16( png_ptr );
//#else
//  png_set_strip_16( png_ptr );
//#endif

  // Expand paletted colors into true RGB triplets
  if ( colorType == PNG_COLOR_TYPE_PALETTE )
  {
    png_set_palette_to_rgb( png_ptr );
  }
  
  // Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel
  if ( colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8 )
  {
    png_set_expand_gray_1_2_4_to_8( png_ptr );
  }

  // Expand paletted or RGB images with transparency to full alpha channels
  // so the data will be available as RGBA quartets.
  if ( png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS ) )
  {
    png_set_tRNS_to_alpha(png_ptr);
  }
  
  // If you want to shift the pixel values from the range [0,255] or
  // [0,65535] to the original [0,7] or [0,31], or whatever range the
  // colors were originally in:
  if ( png_get_valid( png_ptr, info_ptr, PNG_INFO_sBIT ) )
  {
    png_color_8p sig_bit_p;
    png_get_sBIT(png_ptr, info_ptr, &sig_bit_p);
    png_set_shift(png_ptr, sig_bit_p);
  }
  
  // Flip the RGB pixels to BGR (or RGBA to BGRA)
  if ( colorType & PNG_COLOR_MASK_COLOR )
  {
    png_set_bgr( png_ptr );
  }

  // Swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR)
  //png_set_swap_alpha( png_ptr );
  
  // Swap bytes of 16 bit files to least significant byte first
  png_set_swap( png_ptr );
  
  // Add filler (or alpha) byte (before/after each RGB triplet)
  //png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

  int number_passes;
#ifdef PNG_READ_INTERLACING_SUPPORTED
  /* Turn on interlace handling.  REQUIRED if you are not using
  * png_read_image().  To see how to handle interlacing passes,
  * see the png_read_row() method below:
  */
  number_passes = png_set_interlace_handling(png_ptr);
#else
  number_passes = 1;
#endif /* PNG_READ_INTERLACING_SUPPORTED */

  // Optional call to gamma correct and add the background to the palette
  // and update info structure.  REQUIRED if you are expecting libpng to
  // update the palette for you (ie you selected such a transform above).
  png_read_update_info( png_ptr, info_ptr );

  // Allocate the memory to hold the image using the fields of info_ptr.
  std::size_t rowSize   = png_get_rowbytes( png_ptr, info_ptr );
  std::size_t totalSize = height * rowSize;
  char* buffer = new char[totalSize];
  char* ptr = ( buffer + totalSize );

  // Read in the image into out data buffer.
  for ( int pass = 0; pass < number_passes; ++pass )
  {
    for ( int row = 0; row < height; ++row )
    {
      ptr -= rowSize;
      png_read_row( png_ptr, (png_bytep)ptr, NULL );
    }

    NE_ASSERT( ptr == buffer )();
  }
    
  // Read rest of file, and get additional chunks in info_ptr - REQUIRED 
  png_read_end( png_ptr, info_ptr );

  png_destroy_read_struct( &png_ptr, &info_ptr, nullptr );

  int channels      = GetChannelsFromColorType( colorType );
  int bytesPerPixel = channels * ( bitDepth / 8 ); // Convert bits to bytes and multiply with number of channels.
  
  // create & full the output structure.
  ImageCodecData* codecData = new ImageCodecData();
  codecData->width      = width;
  codecData->height     = height;
  codecData->size       = width * height * bytesPerPixel;
  codecData->format     = EnginePixelFormatFromPng( colorType, bitDepth );
  codecData->bpp        = bytesPerPixel;
  codecData->datastream = (uint8*)buffer;

  // return image data.
  return codecData;
}


ImageCodecData*
PngCodec::ConvertFromRawBits( uint8* bits, PixelFormat sourceFormat, std::size_t width, std::size_t height, std::size_t pitch, uint8 bpp, PixelFormat returnFormat ) const
///
/// Convert an image from one format to another.  Attempts to interpret the raw image data using the specified parameters
/// and create a FIBITMAP object which is then converted to the specified "out" image format using the FreeImage_ConvertToType
/// function.
///
/// @param bits
///   The image data stream.
///
/// @param sourceFormat
///   The PixelFormat that best describes the source image data.
///
/// @param width
///   the pixel width of the image data.
///
/// @param height
///   The pixel height of the image.
///
/// @param pitch
///   The data size of a single scanline.
///
/// @param bpp
///   The size of each pixel in bits.
///
/// @param returnFormat
///   The PixelFormat that we wish to return
///
/// @return
///   A created ImageCodecData object that contains the image data for the converted image.
///
{
  return nullptr;
}

#endif 