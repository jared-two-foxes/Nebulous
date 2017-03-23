#ifndef __NEBULAE_PIXELFORMAT_H__
#define __NEBULAE_PIXELFORMAT_H__


namespace Nebulae
{

  /** The pixel format used for images, textures, and render surfaces 
   */
  enum PixelFormat : uint16
  {
    /// Unknown pixel format.
    PF_UNKNOWN = 0,
    /// 8-bit pixel format, all bits luminance.
    PF_L8 = 1,
    PF_BYTE_L = PF_L8,
    /// 16-bit pixel format, all bits luminance.
    PF_L16 = 2,
    PF_SHORT_L = PF_L16,
    /// 8-bit pixel format, all bits alpha.
    PF_A8 = 3,
    PF_BYTE_A = PF_A8,
    /// 8-bit pixel format, 4 bits alpha, 4 bits luminance.
    PF_A4L4 = 4,
    /// 2 byte pixel format, 1 byte luminance, 1 byte alpha
    PF_BYTE_LA = 5,
    /// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
    PF_R5G6B5 = 6,
    /// 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
    PF_B5G6R5 = 7,
    /// 8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.
    PF_R3G3B2 = 31,
    /// 16-bit pixel format, 4 bits for alpha, red, green and blue.
    PF_A4R4G4B4 = 8,
    /// 16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.
    PF_A1R5G5B5 = 9,
    /// 24-bit pixel format, 8 bits for red, green and blue.
    PF_R8G8B8 = 10,
    /// 24-bit pixel format, 8 bits for blue, green and red.
    PF_B8G8R8 = 11,
    /// 32-bit pixel format, 8 bits for alpha, red, green and blue.
    PF_A8R8G8B8 = 12,
    /// 32-bit pixel format, 8 bits for blue, green, red and alpha.
    PF_A8B8G8R8 = 13,
    /// 32-bit pixel format, 8 bits for blue, green, red and alpha.
    PF_B8G8R8A8 = 14,
    /// 32-bit pixel format, 8 bits for red, green, blue and alpha.
    PF_R8G8B8A8 = 28,
    /// 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
    /// like PF_A8R8G8B8, but alpha will get discarded
    PF_X8R8G8B8 = 26,
    /// 32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red
    /// like PF_A8B8G8R8, but alpha will get discarded
    PF_X8B8G8R8 = 27,

#if NE_ENDIAN == NE_ENDIAN_BIG
    /// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
    PF_BYTE_RGB = PF_R8G8B8,
    /// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
    PF_BYTE_BGR = PF_B8G8R8,
    /// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
    PF_BYTE_BGRA = PF_B8G8R8A8,
    /// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
    PF_BYTE_RGBA = PF_R8G8B8A8,
#else
    /// 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
    PF_BYTE_RGB = PF_B8G8R8,
    /// 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
    PF_BYTE_BGR = PF_R8G8B8,
    /// 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
    PF_BYTE_BGRA = PF_A8R8G8B8,
    /// 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
    PF_BYTE_RGBA = PF_A8B8G8R8,
#endif

    /// 32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue.
    PF_A2R10G10B10 = 15,
    /// 32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha.
    PF_A2B10G10R10 = 16,
    /// DDS (DirectDraw Surface) DXT1 format
    PF_DXT1 = 17,
    /// DDS (DirectDraw Surface) DXT2 format
    PF_DXT2 = 18,
    /// DDS (DirectDraw Surface) DXT3 format
    PF_DXT3 = 19,
    /// DDS (DirectDraw Surface) DXT4 format
    PF_DXT4 = 20,
    /// DDS (DirectDraw Surface) DXT5 format
    PF_DXT5 = 21,
    // 16-bit pixel format, 16 bits (float) for red
    PF_FLOAT16_R = 32,
    // 48-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue
    PF_FLOAT16_RGB = 22,
    // 64-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue, 16 bits (float) for alpha
    PF_FLOAT16_RGBA = 23,
    // 32-bit pixel format, 32 bits (float) for red
    PF_FLOAT32_R = 33,
    // 96-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue
    PF_FLOAT32_RGB = 24,
    // 128-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue, 32 bits (float) for alpha
    PF_FLOAT32_RGBA = 25,
    // 32-bit, 2-channel s10e5 floating point pixel format, 16-bit green, 16-bit red
    PF_FLOAT16_GR = 35,
    // 64-bit, 2-channel floating point pixel format, 32-bit green, 32-bit red
    PF_FLOAT32_GR = 36,
    // Depth texture format
    PF_DEPTH = 29,
    // 64-bit pixel format, 16 bits for red, green, blue and alpha
    PF_SHORT_RGBA = 30,
    // 32-bit pixel format, 16-bit green, 16-bit red
    PF_SHORT_GR = 34,
    // 48-bit pixel format, 16 bits for red, green and blue
    PF_SHORT_RGB = 37,
    /// PVRTC (PowerVR) RGB 2 bpp
    PF_PVRTC_RGB2 = 38,
    /// PVRTC (PowerVR) RGBA 2 bpp
    PF_PVRTC_RGBA2 = 39,
    /// PVRTC (PowerVR) RGB 4 bpp
    PF_PVRTC_RGB4 = 40,
    /// PVRTC (PowerVR) RGBA 4 bpp
    PF_PVRTC_RGBA4 = 41,
    /// 8-bit pixel format, all bits red.
    PF_R8 = 42,
    /// 16-bit pixel format, 8 bits red, 8 bits green.
    PF_RG8 = 43,
    // Number of pixel formats currently defined
    PF_COUNT = 44
  };

  /** Flags defining some on/off properties of pixel formats
    */
  enum PixelFormatFlags 
  {
    // This format has an alpha channel
    PFF_HASALPHA        = 0x00000001,      
    // This format is compressed. This invalidates the values in elemBytes,
    // elemBits and the bit counts as these might not be fixed in a compressed format.
    PFF_COMPRESSED    = 0x00000002,
    // This is a floating point format
    PFF_FLOAT           = 0x00000004,         
    // This is a depth format (for depth textures)
    PFF_DEPTH           = 0x00000008,
    // Format is in native endian. Generally true for the 16, 24 and 32 bits
    // formats which can be represented as machine integers.
    PFF_NATIVEENDIAN    = 0x00000010,
    // This is an intensity format instead of a RGB one. The luminance
    // replaces R,G and B. (but not A)
    PFF_LUMINANCE       = 0x00000020
  };

  /** Pixel component format 
   */
  enum PixelComponentType
  {
    PCT_BYTE    = 0, ///< Byte per component (8 bit fixed 0.0..1.0)
    PCT_SHORT   = 1, ///< Short per component (16 bit fixed 0.0..1.0))
    PCT_FLOAT16 = 2, ///< 16 bit float per component
    PCT_FLOAT32 = 3, ///< 32 bit float per component
    PCT_COUNT   = 4  ///< Number of pixel types
  };

  /**
   *  Some utility functions for packing and unpacking pixel data
   */
  class PixelUtil 
  {
  public:

    /** Returns the size in bytes of an element of the given pixel format.
        @return
            The size in bytes of an element. See Remarks.
        @remarks
            Passing PF_UNKNOWN will result in returning a size of 0 bytes.
    */
    static std::size_t GetNumElemBytes( PixelFormat format );

    /** Returns the size in bits of an element of the given pixel format.
        @return
            The size in bits of an element. See Remarks.
        @remarks
            Passing PF_UNKNOWN will result in returning a size of 0 bits.
    */
    static std::size_t GetNumElemBits( PixelFormat format );

    /** Returns the size in memory of a region with the given extents and pixel
        format with consecutive memory layout.
        @param width
          The width of the area
        @param height
          The height of the area
        @param depth
          The depth of the area
        @param format
          The format of the area
        @return
      	  The size in bytes
        @remarks
          In case that the format is non-compressed, this simply returns
          width*height*depth*PixelUtil::getNumElemBytes(format). In the compressed
          case, this does serious magic.
    */
    static std::size_t GetMemorySize( std::size_t width, std::size_t height, std::size_t depth, PixelFormat format );
		
    /** Returns the property flags for this pixel format
        @return
          A bitfield combination of PFF_HASALPHA, PFF_ISCOMPRESSED,
          PFF_FLOAT, PFF_DEPTH, PFF_NATIVEENDIAN, PFF_LUMINANCE
        @remarks
          This replaces the separate functions for formatHasAlpha, formatIsFloat, ...
    */
    static unsigned int GetFlags( PixelFormat format );

    /** Shortcut method to determine if the format has an alpha component
     */
    static bool HasAlpha( PixelFormat format );

    /** Shortcut method to determine if the format is floating point
     */
    static bool IsFloatingPoint( PixelFormat format );

    /** Shortcut method to determine if the format is compressed
     */
    static bool IsCompressed( PixelFormat format );

    /** Shortcut method to determine if the format is a depth format.
     */
    static bool IsDepth( PixelFormat format );

    /** Shortcut method to determine if the format is in native endian format.
     */
    static bool IsNativeEndian( PixelFormat format );

    /** Shortcut method to determine if the format is a luminance format.
     */
    static bool IsLuminance( PixelFormat format );
		
    /** Return whether a certain image extent is valid for this image format.
      @param width
    	The width of the area
      @param height
    	The height of the area
      @param depth
    	The depth of the area
      @param format
    	The format of the area
      @remarks 
        For non-compressed formats, this is always true. For DXT formats,
        only sizes with a width and height multiple of 4 and depth 1 are allowed.
    */
    static bool IsValidExtent( std::size_t width, std::size_t height, std::size_t depth, PixelFormat format );

    /** Gives the number of bits (RGBA) for a format. See remarks.          
        @remarks      
          For non-colour formats (dxt, depth) this returns [0,0,0,0].
    */
    static void GetBitDepths( PixelFormat format, int rgba[4] );

    /** Gives the masks for the R, G, B and A component
        @note		
          Only valid for native endian formats
    */
    static void GetBitMasks( PixelFormat format, unsigned int rgba[4] );

    /** Gives the bit shifts for R, G, B and A component
        @note		
          Only valid for native endian formats
    */
    static void GetBitShifts( PixelFormat format, unsigned char rgba[4] );

    /** Unpack a colour value from memory
        @param colour	The colour is returned here
        @param pf		Pixelformat in which to read the colour
        @param src		Source memory location
    */
    static void UnpackColour( Colour *colour, PixelFormat pf,  const void* src );
    
    /** Unpack a colour value from memory
        @param r,g,b,a	The colour is returned here (as byte)
        @param pf		Pixelformat in which to read the colour
        @param src		Source memory location
        @remarks 	This function returns the colour components in 8 bit precision,
          this will lose precision when coming from PF_A2R10G10B10 or floating
          point formats.  
    */
    static void UnpackColour( uint8 *r, uint8 *g, uint8 *b, uint8 *a, PixelFormat pf,  const void* src );
    
    /** Unpack a colour value from memory
        @param r,g,b,a	The colour is returned here (as float)
        @param pf		Pixelformat in which to read the colour
        @param src		Source memory location
    */
    static void UnpackColour( float *r, float *g, float *b, float *a, PixelFormat pf,  const void* src ); 

  }; //PixelUtil

} //namespace

#endif // __NEBULAE_PIXELFORMAT_H__
