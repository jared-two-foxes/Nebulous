#include <Nebulae/Common/Common.h>

using namespace Nebulae;

/**
 *   A record that describes a pixel format in detail.
 */
struct PixelFormatDescription
{
  /// Name of the format, as in the enum
  const char* m_name;
  /// Number of bytes one element (colour value) takes.
  unsigned char m_elemBytes;
  /// Pixel format flags, see enum PixelFormatFlags for the bit field definitions
  unsigned int m_flags;
  /// Component type
  PixelComponentType m_componentType;
  /// Component count
  unsigned char m_componentCount;
  /// Number of bits for red(or luminance), green, blue, alpha
  unsigned char m_rbits, m_gbits, m_bbits, m_abits; /*, ibits, dbits, ... */
  /// Masks and shifts as used by packers/unpackers */
  unsigned int m_rmask, m_gmask, m_bmask, m_amask;
  unsigned char m_rshift, m_gshift, m_bshift, m_ashift;
};

/** Pixel format database.
 */
PixelFormatDescription _pixelFormats[PF_COUNT] = {
  //-----------------------------------------------------------------------
  { "PF_UNKNOWN",
    /* Bytes per element */
    0,
    /* Flags */
    0,
    /* Component type and count */
    PCT_BYTE, 0,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_L8",
    /* Bytes per element */
    1,
    /* Flags */
    PFF_LUMINANCE | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 1,
    /* rbits, gbits, bbits, abits */
    8, 0, 0, 0,
    /* Masks and shifts */
    0xFF, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_L16",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_LUMINANCE | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_SHORT, 1,
    /* rbits, gbits, bbits, abits */
    16, 0, 0, 0,
    /* Masks and shifts */
    0xFFFF, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_A8",
    /* Bytes per element */
    1,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 1,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 8,
    /* Masks and shifts */
    0, 0, 0, 0xFF, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_A4L4",
    /* Bytes per element */
    1,
    /* Flags */
    PFF_HASALPHA | PFF_LUMINANCE | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 2,
    /* rbits, gbits, bbits, abits */
    4, 0, 0, 4,
    /* Masks and shifts */
    0x0F, 0, 0, 0xF0, 0, 0, 0, 4 },
  //-----------------------------------------------------------------------
  { "PF_BYTE_LA",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_HASALPHA | PFF_LUMINANCE,
    /* Component type and count */
    PCT_BYTE, 2,
    /* rbits, gbits, bbits, abits */
    8, 0, 0, 8,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_R5G6B5",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    5, 6, 5, 0,
    /* Masks and shifts */
    0xF800, 0x07E0, 0x001F, 0, 11, 5, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_B5G6R5",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    5, 6, 5, 0,
    /* Masks and shifts */
    0x001F, 0x07E0, 0xF800, 0, 0, 5, 11, 0 },
  //-----------------------------------------------------------------------
  { "PF_A4R4G4B4",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    4, 4, 4, 4,
    /* Masks and shifts */
    0x0F00, 0x00F0, 0x000F, 0xF000, 8, 4, 0, 12 },
  //-----------------------------------------------------------------------
  {
    "PF_A1R5G5B5",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE,
    4,
    /* rbits, gbits, bbits, abits */
    5,
    5,
    5,
    1,
    /* Masks and shifts */
    0x7C00,
    0x03E0,
    0x001F,
    0x8000,
    10,
    5,
    0,
    15,
  },
  //-----------------------------------------------------------------------
  { "PF_R8G8B8",
    /* Bytes per element */
    3, // 24 bit integer -- special
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 0,
    /* Masks and shifts */
    0xFF0000, 0x00FF00, 0x0000FF, 0, 16, 8, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_B8G8R8",
    /* Bytes per element */
    3, // 24 bit integer -- special
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 0,
    /* Masks and shifts */
    0x0000FF, 0x00FF00, 0xFF0000, 0, 0, 8, 16, 0 },
  //-----------------------------------------------------------------------
  { "PF_A8R8G8B8",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 8,
    /* Masks and shifts */
    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000, 16, 8, 0, 24 },
  //-----------------------------------------------------------------------
  {
    "PF_A8B8G8R8",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE,
    4,
    /* rbits, gbits, bbits, abits */
    8,
    8,
    8,
    8,
    /* Masks and shifts */
    0x000000FF,
    0x0000FF00,
    0x00FF0000,
    0xFF000000,
    0,
    8,
    16,
    24,
  },
  //-----------------------------------------------------------------------
  { "PF_B8G8R8A8",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 8,
    /* Masks and shifts */
    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000, 16, 8, 0, 24 },
  //-----------------------------------------------------------------------
  { "PF_A2R10G10B10",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    10, 10, 10, 2,
    /* Masks and shifts */
    0x3FF00000, 0x000FFC00, 0x000003FF, 0xC0000000, 20, 10, 0, 30 },
  //-----------------------------------------------------------------------
  { "PF_A2B10G10R10",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    10, 10, 10, 2,
    /* Masks and shifts */
    0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000, 0, 10, 20, 30 },
  //-----------------------------------------------------------------------
  { "PF_DXT1",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 3, // No alpha
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_DXT2",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_DXT3",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_DXT4",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_DXT5",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT16_RGB",
    /* Bytes per element */
    6,
    /* Flags */
    PFF_FLOAT,
    /* Component type and count */
    PCT_FLOAT16, 3,
    /* rbits, gbits, bbits, abits */
    16, 16, 16, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT16_RGBA",
    /* Bytes per element */
    8,
    /* Flags */
    PFF_FLOAT | PFF_HASALPHA,
    /* Component type and count */
    PCT_FLOAT16, 4,
    /* rbits, gbits, bbits, abits */
    16, 16, 16, 16,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT32_RGB",
    /* Bytes per element */
    12,
    /* Flags */
    PFF_FLOAT,
    /* Component type and count */
    PCT_FLOAT32, 3,
    /* rbits, gbits, bbits, abits */
    32, 32, 32, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT32_RGBA",
    /* Bytes per element */
    16,
    /* Flags */
    PFF_FLOAT | PFF_HASALPHA,
    /* Component type and count */
    PCT_FLOAT32, 4,
    /* rbits, gbits, bbits, abits */
    32, 32, 32, 32,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_X8R8G8B8",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 0,
    /* Masks and shifts */
    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000, 16, 8, 0, 24 },
  //-----------------------------------------------------------------------
  { "PF_X8B8G8R8",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 0,
    /* Masks and shifts */
    0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 8, 16, 24 },
  //-----------------------------------------------------------------------
  { "PF_R8G8B8A8",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_HASALPHA | PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    8, 8, 8, 8,
    /* Masks and shifts */
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF, 24, 16, 8, 0 },
  //-----------------------------------------------------------------------
  { "PF_DEPTH",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_DEPTH,
    /* Component type and count */
    PCT_FLOAT32, 1, // ?
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_SHORT_RGBA",
    /* Bytes per element */
    8,
    /* Flags */
    PFF_HASALPHA,
    /* Component type and count */
    PCT_SHORT, 4,
    /* rbits, gbits, bbits, abits */
    16, 16, 16, 16,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_R3G3B2",
    /* Bytes per element */
    1,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    3, 3, 2, 0,
    /* Masks and shifts */
    0xE0, 0x1C, 0x03, 0, 5, 2, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT16_R",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_FLOAT,
    /* Component type and count */
    PCT_FLOAT16, 1,
    /* rbits, gbits, bbits, abits */
    16, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT32_R",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_FLOAT,
    /* Component type and count */
    PCT_FLOAT32, 1,
    /* rbits, gbits, bbits, abits */
    32, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_SHORT_GR",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_SHORT, 2,
    /* rbits, gbits, bbits, abits */
    16, 16, 0, 0,
    /* Masks and shifts */
    0x0000FFFF, 0xFFFF0000, 0, 0, 0, 16, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT16_GR",
    /* Bytes per element */
    4,
    /* Flags */
    PFF_FLOAT,
    /* Component type and count */
    PCT_FLOAT16, 2,
    /* rbits, gbits, bbits, abits */
    16, 16, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_FLOAT32_GR",
    /* Bytes per element */
    8,
    /* Flags */
    PFF_FLOAT,
    /* Component type and count */
    PCT_FLOAT32, 2,
    /* rbits, gbits, bbits, abits */
    32, 32, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_SHORT_RGB",
    /* Bytes per element */
    6,
    /* Flags */
    0,
    /* Component type and count */
    PCT_SHORT, 3,
    /* rbits, gbits, bbits, abits */
    16, 16, 16, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_PVRTC_RGB2",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_PVRTC_RGBA2",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_PVRTC_RGB4",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED,
    /* Component type and count */
    PCT_BYTE, 3,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_PVRTC_RGBA4",
    /* Bytes per element */
    0,
    /* Flags */
    PFF_COMPRESSED | PFF_HASALPHA,
    /* Component type and count */
    PCT_BYTE, 4,
    /* rbits, gbits, bbits, abits */
    0, 0, 0, 0,
    /* Masks and shifts */
    0, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_R8",
    /* Bytes per element */
    1,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 1,
    /* rbits, gbits, bbits, abits */
    8, 0, 0, 0,
    /* Masks and shifts */
    0xFF0000, 0, 0, 0, 0, 0, 0, 0 },
  //-----------------------------------------------------------------------
  { "PF_RG8",
    /* Bytes per element */
    2,
    /* Flags */
    PFF_NATIVEENDIAN,
    /* Component type and count */
    PCT_BYTE, 2,
    /* rbits, gbits, bbits, abits */
    8, 8, 0, 0,
    /* Masks and shifts */
    0xFF0000, 0x00FF00, 0, 0, 8, 0, 0, 0 },
};


/**
 *   Directly get the description record for provided pixel format. For debug builds,
 *   this checks the bounds of fmt with an assertion.
 */
static inline const PixelFormatDescription& GetDescriptionFor( const PixelFormat FMT )
{
  const int ORD = static_cast<int>( FMT );
  NE_ASSERT( ord >= 0 && ord < PF_COUNT, "Invalid PixelFormat." );

  return _pixelFormats[ORD];
}


std::size_t PixelUtil::GetNumElemBytes( PixelFormat format ) { return GetDescriptionFor( format ).m_elemBytes; }


std::size_t PixelUtil::GetMemorySize( std::size_t width, std::size_t height, std::size_t depth, PixelFormat format )
{
  if ( IsCompressed( format ) )
  {
    switch ( format )
    {
    // DXT formats work by dividing the image into 4x4 blocks, then encoding each
    // 4x4 block with a certain number of bytes.
    case PF_DXT1:
      return ( ( width + 3 ) / 4 ) * ( ( height + 3 ) / 4 ) * 8 * depth;

    case PF_DXT2:
    case PF_DXT3:
    case PF_DXT4:
    case PF_DXT5:
      return ( ( width + 3 ) / 4 ) * ( ( height + 3 ) / 4 ) * 16 * depth;

    // Size calculations from the PVRTC OpenGL extension spec
    // http://www.khronos.org/registry/gles/extensions/IMG/IMG_texture_compression_pvrtc.txt
    // Basically, 32 bytes is the minimum texture size.  Smaller textures are padded up to 32 bytes
    case PF_PVRTC_RGB2:
    case PF_PVRTC_RGBA2:
      NE_ASSERT( depth == 1, "PVRTC expects single depth channel" );
      return ( ( std::max( static_cast<int>( width ), 16 ) * std::max( static_cast<int>( height ), 8 ) * 2 ) + 7 ) / 8;

    case PF_PVRTC_RGB4:
    case PF_PVRTC_RGBA4:
      NE_ASSERT( depth == 1, "PVRTC expects single depth channel" );
      return ( ( std::max( static_cast<int>( width ), 8 ) * std::max( static_cast<int>( height ), 8 ) * 4 ) + 7 ) / 8;

    default:
      break;
      NE_ASSERT( false, "Invalid compressed pixel format" );
    }
  }
  else
  {
    return width * height * depth * GetNumElemBytes( format );
  }

  return 0;
}


std::size_t PixelUtil::GetNumElemBits( PixelFormat format ) { return GetDescriptionFor( format ).m_elemBytes * 8; }


unsigned int PixelUtil::GetFlags( PixelFormat format ) { return GetDescriptionFor( format ).m_flags; }


bool PixelUtil::HasAlpha( PixelFormat format ) { return ( PixelUtil::GetFlags( format ) & PFF_HASALPHA ) > 0; }


bool PixelUtil::IsFloatingPoint( PixelFormat format ) { return ( PixelUtil::GetFlags( format ) & PFF_FLOAT ) > 0; }


bool PixelUtil::IsCompressed( PixelFormat format ) { return ( PixelUtil::GetFlags( format ) & PFF_COMPRESSED ) > 0; }


bool PixelUtil::IsDepth( PixelFormat format ) { return ( PixelUtil::GetFlags( format ) & PFF_DEPTH ) > 0; }


bool PixelUtil::IsNativeEndian( PixelFormat format )
{
  return ( PixelUtil::GetFlags( format ) & PFF_NATIVEENDIAN ) > 0;
}


bool PixelUtil::IsLuminance( PixelFormat format ) { return ( PixelUtil::GetFlags( format ) & PFF_LUMINANCE ) > 0; }


void PixelUtil::UnpackColour( Colour* colour, PixelFormat pf, const void* src )
{
  UnpackColour( &colour->r, &colour->g, &colour->b, &colour->a, pf, src );
}


void PixelUtil::UnpackColour( uint8* r, uint8* g, uint8* b, uint8* a, PixelFormat pf, const void* src )
{
  const PixelFormatDescription& des = GetDescriptionFor( pf );
  if ( ( des.m_flags & PFF_NATIVEENDIAN ) != 0U )
  {
    // Shortcut for integer formats unpacking
    const unsigned int VALUE = Bitwise::IntRead( src, des.m_elemBytes );
    if ( ( des.m_flags & PFF_LUMINANCE ) != 0U )
    {
      // Luminance format -- only rbits used
      *r = *g = *b =
        static_cast<uint8>( Bitwise::FixedToFixed( ( VALUE & des.m_rmask ) >> des.m_rshift, des.m_rbits, 8 ) );
    }
    else
    {
      *r = static_cast<uint8>( Bitwise::FixedToFixed( ( VALUE & des.m_rmask ) >> des.m_rshift, des.m_rbits, 8 ) );
      *g = static_cast<uint8>( Bitwise::FixedToFixed( ( VALUE & des.m_gmask ) >> des.m_gshift, des.m_gbits, 8 ) );
      *b = static_cast<uint8>( Bitwise::FixedToFixed( ( VALUE & des.m_bmask ) >> des.m_bshift, des.m_bbits, 8 ) );
    }
    if ( ( des.m_flags & PFF_HASALPHA ) != 0U )
    {
      *a = static_cast<uint8>( Bitwise::FixedToFixed( ( VALUE & des.m_amask ) >> des.m_ashift, des.m_abits, 8 ) );
    }
    else
    {
      *a = 255; // No alpha, default a component to full
    }
  }
  else
  {
    // Do the operation with the more generic floating point
    float rr = 0;
    float gg = 0;
    float bb = 0;
    float aa = 0;
    UnpackColour( &rr, &gg, &bb, &aa, pf, src );
    *r = static_cast<uint8>( Bitwise::FloatToFixed( rr, 8 ) );
    *g = static_cast<uint8>( Bitwise::FloatToFixed( gg, 8 ) );
    *b = static_cast<uint8>( Bitwise::FloatToFixed( bb, 8 ) );
    *a = static_cast<uint8>( Bitwise::FloatToFixed( aa, 8 ) );
  }
}


void PixelUtil::UnpackColour( float* r, float* g, float* b, float* a, PixelFormat pf, const void* src )
{
  const PixelFormatDescription& des = GetDescriptionFor( pf );
  if ( ( des.m_flags & PFF_NATIVEENDIAN ) != 0U )
  {
    // Shortcut for integer formats unpacking
    const unsigned int VALUE = Bitwise::IntRead( src, des.m_elemBytes );
    if ( ( des.m_flags & PFF_LUMINANCE ) != 0U )
    {
      // Luminance format -- only rbits used
      *r = *g = *b = Bitwise::FixedToFloat( ( VALUE & des.m_rmask ) >> des.m_rshift, des.m_rbits );
    }
    else
    {
      *r = Bitwise::FixedToFloat( ( VALUE & des.m_rmask ) >> des.m_rshift, des.m_rbits );
      *g = Bitwise::FixedToFloat( ( VALUE & des.m_gmask ) >> des.m_gshift, des.m_gbits );
      *b = Bitwise::FixedToFloat( ( VALUE & des.m_bmask ) >> des.m_bshift, des.m_bbits );
    }

    if ( ( des.m_flags & PFF_HASALPHA ) != 0U )
    {
      *a = Bitwise::FixedToFloat( ( VALUE & des.m_amask ) >> des.m_ashift, des.m_abits );
    }
    else
    {
      *a = 1.0F; // No alpha, default a component to full
    }
  }
  else
  {
    switch ( pf )
    {
    case PF_FLOAT32_R:
      *r = *g = *b = ( (float*)src )[0];
      *a = 1.0F;
      break;
    case PF_FLOAT32_GR:
      *g = ( (float*)src )[0];
      *r = *b = ( (float*)src )[1];
      *a = 1.0F;
      break;
    case PF_FLOAT32_RGB:
      *r = ( (float*)src )[0];
      *g = ( (float*)src )[1];
      *b = ( (float*)src )[2];
      *a = 1.0F;
      break;
    case PF_FLOAT32_RGBA:
      *r = ( (float*)src )[0];
      *g = ( (float*)src )[1];
      *b = ( (float*)src )[2];
      *a = ( (float*)src )[3];
      break;
    case PF_FLOAT16_R:
      *r = *g = *b = Bitwise::HalfToFloat( ( (uint16*)src )[0] );
      *a = 1.0F;
      break;
    case PF_FLOAT16_GR:
      *g = Bitwise::HalfToFloat( ( (uint16*)src )[0] );
      *r = *b = Bitwise::HalfToFloat( ( (uint16*)src )[1] );
      *a = 1.0F;
      break;
    case PF_FLOAT16_RGB:
      *r = Bitwise::HalfToFloat( ( (uint16*)src )[0] );
      *g = Bitwise::HalfToFloat( ( (uint16*)src )[1] );
      *b = Bitwise::HalfToFloat( ( (uint16*)src )[2] );
      *a = 1.0F;
      break;
    case PF_FLOAT16_RGBA:
      *r = Bitwise::HalfToFloat( ( (uint16*)src )[0] );
      *g = Bitwise::HalfToFloat( ( (uint16*)src )[1] );
      *b = Bitwise::HalfToFloat( ( (uint16*)src )[2] );
      *a = Bitwise::HalfToFloat( ( (uint16*)src )[3] );
      break;
    case PF_SHORT_RGB:
      *r = Bitwise::FixedToFloat( ( (uint16*)src )[0], 16 );
      *g = Bitwise::FixedToFloat( ( (uint16*)src )[1], 16 );
      *b = Bitwise::FixedToFloat( ( (uint16*)src )[2], 16 );
      *a = 1.0F;
      break;
    case PF_SHORT_RGBA:
      *r = Bitwise::FixedToFloat( ( (uint16*)src )[0], 16 );
      *g = Bitwise::FixedToFloat( ( (uint16*)src )[1], 16 );
      *b = Bitwise::FixedToFloat( ( (uint16*)src )[2], 16 );
      *a = Bitwise::FixedToFloat( ( (uint16*)src )[3], 16 );
      break;
    case PF_BYTE_LA:
      *r = *g = *b = Bitwise::FixedToFloat( ( (uint8*)src )[0], 8 );
      *a = Bitwise::FixedToFloat( ( (uint8*)src )[1], 8 );
      break;
    default:
      // Not yet supported
      assert( 0 );
      // OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
      //     "unpack from "+getFormatName(pf)+" not implemented",
      //     "PixelUtil::unpackColour");
      break;
    }
  }
}
