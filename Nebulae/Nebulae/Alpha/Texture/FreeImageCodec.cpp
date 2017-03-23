
#include "FreeImageCodec.h"

#include <FreeImage.h>

using namespace Nebulae;


namespace FreeImage {

bool g_freeImageInitialised = false;

unsigned DLL_CALLCONV StreamRead( void *buffer, unsigned size, unsigned count, fi_handle handle )
{
	return (unsigned int)file_read( buffer, (unsigned)size, (unsigned)count, handle );
}

unsigned DLL_CALLCONV StreamWrite( void *buffer, unsigned size, unsigned count, fi_handle handle )
{
	return (unsigned int)file_write( buffer, size, count, handle );
}

int DLL_CALLCONV StreamSeek( fi_handle handle, long offset, int origin )
{
	return (int)file_seek( handle, offset, origin );
}

long DLL_CALLCONV StreamTell( fi_handle handle )
{
	return file_tell( handle );
}


void 
ErrorHandler( FREE_IMAGE_FORMAT fif, const char *message )
///
/// FreeImage error handler.
///
/// @param fif 
///   Format / Plugin responsible for the error
///
/// @param message 
///   Error message
///
{
  printf( "\n*** " );
  if ( fif != FIF_UNKNOWN ) {
    printf( "%s Format\n", FreeImage_GetFormatFromFIF(fif) );
  }
  printf( "%s", message );
  printf( " ***\n" );
}

void
Initiate()
///
/// Initializes and sets up FreeImage.
///
/// @return
///   Nothing.
///
{
  if( !g_freeImageInitialised ) {
    FreeImage_Initialise();
    FreeImage_SetOutputMessage( FreeImage::ErrorHandler );
    g_freeImageInitialised = true;
  }
}

void
Shutdown()
///
/// Tears down FreeImage.
///
/// @return
///   Nothing.
///
{
  if( g_freeImageInitialised ) {
    FreeImage_DeInitialise();
    g_freeImageInitialised = false;
  }
}


FREE_IMAGE_TYPE 
ToImageType( PixelFormat formatType )
///
/// Provides a conversion from Nebulae::PixelFormat to FREE_IMAGE_TYPE
///
/// @param formatType
///   The Nebulae based PixelFormat
///
/// @return
///   The corresponding FreeImage image type if it could be found, else 
///   function will return FIT_UNKNOWN.
///
{
  FREE_IMAGE_TYPE type = FIT_UNKNOWN;

  switch( formatType )
  {
    case PF_R8:
    case PF_L8:
    case PF_A8:
    case PF_L16:
    case PF_R8G8B8:
    case PF_B8G8R8:
    case PF_A8R8G8B8:
    case PF_A8B8G8R8:
    case PF_B8G8R8A8:
    case PF_R8G8B8A8:
    case PF_X8R8G8B8:
    case PF_X8B8G8R8:
      type = FIT_BITMAP;
      break;

    case PF_FLOAT32_RGB:
      type = FIT_RGBF;
      break;
    case PF_FLOAT32_RGBA:
      type = FIT_RGBAF;
      break;
    
    case PF_SHORT_RGBA:
      type = FIT_RGBA16;
      break;
    
    case PF_SHORT_RGB:
      type = FIT_RGB16;
      break;
    
    default:
    case PF_DEPTH:
    case PF_A4L4:
    case PF_RG8:
    case PF_BYTE_LA:
    case PF_SHORT_GR:
    case PF_R5G6B5:
    case PF_B5G6R5:
    case PF_R3G3B2:
    case PF_A4R4G4B4:
    case PF_A1R5G5B5:
    case PF_A2R10G10B10:
    case PF_A2B10G10R10:
    case PF_FLOAT16_R:
    case PF_FLOAT16_RGB:
    case PF_FLOAT16_RGBA:
    case PF_FLOAT32_R:
    case PF_FLOAT16_GR:
    case PF_FLOAT32_GR:
    case PF_PVRTC_RGB2:
    case PF_PVRTC_RGBA2:
    case PF_PVRTC_RGB4:
    case PF_PVRTC_RGBA4:
    case PF_DXT1:
    case PF_DXT2:
    case PF_DXT3:
    case PF_DXT4:
    case PF_DXT5:
      NE_ASSERT( false, "Unsupport pixel format found!" ) ();
  }

  return type;
}

FIBITMAP* 
ConvertFromRawBitsEx( BYTE *bits, FREE_IMAGE_TYPE type, std::size_t width, std::size_t height, std::size_t pitch, unsigned bpp, unsigned red_mask, unsigned green_mask, unsigned blue_mask, BOOL topdown) 
///
/// An extension to the FreeImage_ConvertFromRawBits function which accepts an extra argument which describes the Image data format.  Creates 
/// a FIBITMAP object from the specified parameter information.  Currently only tested with standard image types.
///
/// @param bits
///   The image data.
///
/// @param type
///   The FREE_IMAGE_TYPE that best describes the image data format.
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
/// @param red_mask, green_mask, blue_mask
///   Colour masks.
///
/// @param topdown
///   Whether the image data is format such that the first scanline is the top or the bottom of the image.
///
/// @remarks
///   See FreeImage_ConvertFromRawBits
///
/// @return
///   A FIBITMAP FreeImage object.
///
{
  FIBITMAP *dib = FreeImage_AllocateT(type, (int)width, (int)height, bpp, red_mask, green_mask, blue_mask);

  if (dib != NULL) {
    if (topdown) {
      for (int i = (int)height - 1; i >= 0; --i) {
        memcpy(FreeImage_GetScanLine(dib, i), bits, FreeImage_GetLine(dib));
        bits += pitch;
      }
    } else {
      for (int i = 0; i < (int)height; ++i) {
        memcpy(FreeImage_GetScanLine(dib, i), bits, FreeImage_GetLine(dib));
        bits += pitch;
      }
    }
  }

  return dib;
}

Nebulae::ImageCodecData*
GenerateImageDataFromBitmap( FIBITMAP* bitmap ) 
///
/// For a given FIBITMAP object generates a Nebulae ImageCodec data object.
///
/// @param bitmap
///   A valid image object.
/// 
/// @return
///   The ImageCodecData
/// 
{
  FREE_IMAGE_COLOR_TYPE colourType  = FreeImage_GetColorType( bitmap );
  FREE_IMAGE_TYPE       imageType   = FreeImage_GetImageType( bitmap );
  uint32                bpp         = FreeImage_GetBPP( bitmap );
  int                   width       = FreeImage_GetWidth( bitmap );
  int                   height      = FreeImage_GetHeight( bitmap );
  int                   depth       = 1; // only 2D formats handled by this codec
  int                   numMipmaps  = 0; // no mipmaps in non-DDS
  int                   flags       = 0;
  PixelFormat           format      = PF_UNKNOWN;
  
  // Prep the image based on the image format.
  switch( imageType ) {
    case FIT_UNKNOWN:
    case FIT_COMPLEX:
    case FIT_UINT32:
    case FIT_INT32:
    case FIT_DOUBLE:
    default:
      NE_ASSERT( false, "Unknown or unsupported image format" )();
      break;
      
    case FIT_BITMAP: // Standard image type
      if( colourType == FIC_MINISWHITE || colourType == FIC_MINISBLACK ) {
        // Perform any colour conversions for greyscale
        FIBITMAP* newBitmap = FreeImage_ConvertToGreyscale( bitmap );
        // free old bitmap and replace
        FreeImage_Unload( bitmap );
        bitmap = newBitmap;
        // get new formats
        bpp = FreeImage_GetBPP( bitmap );
      } else if( bpp < 8 || colourType == FIC_PALETTE || colourType == FIC_CMYK ) {
        // Perform any colour conversions for RGB
        FIBITMAP* newBitmap =  NULL;
        if( FreeImage_IsTransparent( bitmap ) ) {
          // convert to 32 bit to preserve the transparency
          // (the alpha byte will be 0 if pixel is transparent)
          newBitmap = FreeImage_ConvertTo32Bits( bitmap );
        } else {
          // no transparency - only 3 bytes are needed
          newBitmap = FreeImage_ConvertTo24Bits( bitmap );
        }
        
        // free old bitmap and replace
        FreeImage_Unload( bitmap );
        bitmap = newBitmap;
        // get new formats
        bpp = FreeImage_GetBPP( bitmap );
      }
      
      // by this stage, 8-bit is greyscale, 16/24/32 bit are RGB[A]
      switch( bpp ) {
        case 8:
          format = PF_L8;
          break;
        case 16:
          // Determine 555 or 565 from green mask
          // cannot be 16-bit greyscale since that's FIT_UINT16
          if( FreeImage_GetGreenMask( bitmap ) == FI16_565_GREEN_MASK ) {
            format = PF_R5G6B5;
          } else { // FreeImage doesn't support 4444 format so must be 1555
            format = PF_A1R5G5B5;
          }
          break;
        case 24:
          format = PF_BYTE_RGB;
          break;
        case 32:
          format = PF_BYTE_RGBA;
          break;
      }
      break;
      
    case FIT_UINT16:
    case FIT_INT16: // 16-bit greyscale
      format = PF_L16;
      break;
      
    case FIT_FLOAT: // Single-component floating point data
      format = PF_FLOAT32_R;
      break;
      
    case FIT_RGB16:
      format = PF_SHORT_RGB;
      break;
      
    case FIT_RGBA16:
      format = PF_SHORT_RGBA;
      break;
      
    case FIT_RGBF:
      format = PF_FLOAT32_RGB;
      break;
      
    case FIT_RGBAF:
      format = PF_FLOAT32_RGBA;
      break;
  };
  
  // Final data - invert image and trim pitch at the same time
  std::size_t bytesPerPixel = PixelUtil::GetNumElemBytes( format );
  std::size_t pitch         = width * bytesPerPixel;
  std::size_t size          = pitch * height;
  BYTE*       bytes         = FreeImage_GetBits( bitmap );
  
  // Store out the image data.
  uint8* datastream = new uint8[ size ];
  memcpy( datastream, bytes, size );
  
  // create & full the output structure.
  ImageCodecData* codecData = new ImageCodecData();
  codecData->width      = width;
  codecData->height     = height;
  codecData->size       = size;
  codecData->format     = format;
  codecData->bpp        = bpp;
  codecData->datastream = datastream;
  
  // return image data.
  return codecData;
}
  
} //FreeImage


// ---------------------------------------------------------------------------

FreeImageCodec::FreeImageCodec()
///
/// Constructor
///
{}

FreeImageCodec::~FreeImageCodec()
///
/// Destructor.
///
{}

ImageCodecData* 
FreeImageCodec::Decode( File& is ) const
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
  // Ensure that freeimage is initiated.
  FreeImage::Initiate();

  FreeImageIO io;
  io.read_proc  = FreeImage::StreamRead;
  io.write_proc = FreeImage::StreamWrite;
  io.seek_proc  = FreeImage::StreamSeek;
  io.tell_proc  = FreeImage::StreamTell;

  // check the file signature and deduce its format
  FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromHandle( &io, &is );
  NE_ASSERT( fif != FIF_UNKNOWN, "Unknown or unsupported image format" )( fif );
  if( fif == FIF_UNKNOWN ) {  
    return NULL;
  }

  // check that the plugin has reading capabilities ...
  if( (fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif) ) {
    // ok, let's load the file
    FIBITMAP* bitmap = FreeImage_LoadFromHandle( fif, &io, &is );
    if( bitmap ) {
      ImageCodecData* data = FreeImage::GenerateImageDataFromBitmap( bitmap );
      FreeImage_Unload( bitmap );

      return data;
    }
  }
    
  return NULL;
}

ImageCodecData*
FreeImageCodec::ConvertFromRawBits( uint8* bits, PixelFormat sourceFormat, std::size_t width, std::size_t height, std::size_t pitch, uint8 bpp, PixelFormat returnFormat ) const 
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
  FREE_IMAGE_TYPE sourceType    = FreeImage::ToImageType( sourceFormat );
  FREE_IMAGE_TYPE convertedType = FreeImage::ToImageType( returnFormat );
  
  if( sourceType == FIT_UNKNOWN ) return NULL;

  FIBITMAP* bitmap = FreeImage::ConvertFromRawBitsEx( bits, sourceType, width, height, pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false );
  if( bitmap != NULL ) {
    FIBITMAP* newBitmap = FreeImage_ConvertToType( bitmap, convertedType );   
    FreeImage_Unload(bitmap);

    ImageCodecData* data = FreeImage::GenerateImageDataFromBitmap( newBitmap );
    FreeImage_Unload( newBitmap );

    return data;
  }

  return NULL;
}