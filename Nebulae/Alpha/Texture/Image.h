#ifndef __NEBULAE_IMAGE_H__
#define __NEBULAE_IMAGE_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Image {
    
  private:
    std::size_t m_width;      ///< The width of the image in pixels
    std::size_t m_height;     ///< The height of the image in pixels
    std::size_t m_bufSize;    ///< The size of the image buffer
    PixelFormat m_format;     ///< The pixel format of the image
    uint8       m_pixelSize;  ///< The number of bytes per pixel
    uint8*      m_buffer;     ///< The internal image buffer.

    public:
      /** Standard constructor. */
      Image();

      /** Copy-constructor - copies all the data from the target image. */
      Image( const Image& image );

      /** Standard destructor. */
      ~Image();

      /** Assignment operator - copies all the data from the target image. */
      Image& operator =( const Image &img );

      /** Delete all the memory held by this image, if owned by this image (not dynamic). */
      void freeMemory();

      /** Flips (mirrors) the image around the Y-axis. 
          @remarks
              An example of an original and flipped image:
              <pre>                
              originalimg
              00000000000
              00000000000
              00000000000
              00000000000
              00000000000
              ------------> flip axis
              00000000000
              00000000000
              00000000000
              00000000000
              00000000000
              originalimg
              </pre>
      */
      Image& flipAroundY();

      /** Flips (mirrors) the image around the X-axis.
          @remarks
              An example of an original and flipped image:
              <pre>
                     flip axis
                         |
              originalimg|gmilanigiro
              00000000000|00000000000
              00000000000|00000000000
              00000000000|00000000000
              00000000000|00000000000
              00000000000|00000000000
              </pre>
      */                 
      Image& flipAroundX();

      /** Loads an image file from a stream.
          @remarks
            This method works in the same way as the filename-based load 
            method except it loads the image from a DataStream object. 
            This DataStream is expected to contain the 
            encoded data as it would be held in a file. 
            Any format for which an associated ImageCodec is registered 
            can be loaded. 
            This can include complex formats like DDS with embedded custom 
            mipmaps, cube faces and volume textures.
            The type can be determined by calling getFormat().             
          @param
            stream The source data.
          @param
            type The type of the image. Used to decide what decompression
            codec to use. Can be left blank if the stream data includes
            a header to identify the data.
        */
      Image& load( File& is );

      /** Stores a pointer to raw data in memory. The pixel format has to be specified.
          @remarks
            This method loads an image into memory held in the object. The 
            pixel format will be either greyscale or RGB with an optional
            Alpha component.
            The type can be determined by calling getFormat().             
          @note
            Whilst typically your image is likely to be a simple 2D image,
            you can define complex images including cube maps, volume maps,
            and images including custom mip levels. The layout of the 
            internal memory should be:
            <ul><li>face 0, mip 0 (top), width x height (x depth)</li>
            <li>face 0, mip 1, width/2 x height/2 (x depth/2)</li>
            <li>face 0, mip 2, width/4 x height/4 (x depth/4)</li>
            <li>.. remaining mips for face 0 .. </li>
            <li>face 1, mip 0 (top), width x height (x depth)</li
            <li>.. and so on. </li>
            </ul>
            Of course, you will never have multiple faces (cube map) and
            depth too.
          @param
            The data pointer
          @param
            Width of image
          @param
            Height of image
          @param
            Image Depth (in 3d images, numbers of layers, otherwise 1)
          @param
            Pixel Format
          @param
            if memory associated with this buffer is to be destroyed
            with the Image object.
          @param
            the number of faces the image data has inside (6 for cubemaps, 1 otherwise)
          @param
            the number of mipmaps the image data has inside
          @note
            The memory associated with this buffer is NOT destroyed with the
            Image object, unless autoDelete is set to true.
          @remarks 
            The size of the buffer must be numFaces*PixelUtil::getMemorySize(width, height, depth, format)
        */
      Image& loadDynamicImage( uint8* data, std::size_t width, std::size_t height, 
                std::size_t depth, PixelFormat format, bool autoDelete = false, 
                std::size_t numFaces = 1, std::size_t numMipMaps = 0);

      /** Will attempt to convert this image to use the requested format.
        */
      Image& convert( PixelFormat convertedFormat );
  
      /// Save the image as a file.
      void save( const std::string& filename );

      /// Returns a pointer to the internal image buffer.
      uint8* getData();

      /// Returns a const pointer to the internal image buffer. 
      const uint8* getData() const;

      /// Returns the size of the data buffer.
      std::size_t getSize() const;

      /// Gets the width of the image in pixels.
      std::size_t getWidth() const;

      /// Gets the height of the image in pixels.
      std::size_t getHeight() const;

      /// Gets the physical width in bytes of each row of pixels.
      std::size_t getRowSpan() const;

      /// Returns the image format.
      PixelFormat getFormat() const;

      /// Returns the number of bits per pixel.
      uint8 getBPP() const;

      /// Returns true if the image has an alpha component.
      bool hasAlpha() const;

      /// Get colour value from a certain location in the image. 
      Colour getColourAt( std::size_t x, std::size_t y ) const;

  
      /// Static function to calculate size in bytes from the number of mipmaps, faces and the dimensions
      static std::size_t calculateSize( std::size_t mipmaps, std::size_t faces, std::size_t width, std::size_t height, std::size_t depth, PixelFormat format );


}; //Image

} //Nebulae

#endif //__NEBULAE_IMAGE_H__