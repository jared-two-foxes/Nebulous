/** \file Box.h \brief Contains the utility class Box, which represents a simple
    box in Nebulae. */

#ifndef __NEBULAE_COMMON_PIXELBOX_H__
#define __NEBULAE_COMMON_PIXELBOX_H__

namespace Nebulae {


/** A primitive describing a volume (3D), image (2D) or line (1D) of pixels in memory.
    In case of a rectangle, depth must be 1. 
    Pixels are stored as a succession of "depth" slices, each containing "height" rows of 
    "width" pixels.
*/
class PixelBox : public Box {

  public:
    /// The data pointer 
    void *data;

    /// The pixel format 
    PixelFormat format;

    /** Number of elements between the leftmost pixel of one row and the left
        pixel of the next. This value must always be equal to getWidth() (consecutive) 
        for compressed formats.
    */
    std::size_t rowPitch;

    /** Number of elements between the top left pixel of one (depth) slice and 
        the top left pixel of the next. This can be a negative value. Must be a multiple of
        rowPitch. This value must always be equal to getWidth()*getHeight() (consecutive) 
        for compressed formats.
    */
    std::size_t slicePitch;

    public:
      /// Parameter constructor for setting the members manually
      PixelBox() {}
      ~PixelBox() {}

      /** Constructor providing extents in the form of a Box object. This constructor
          assumes the pixel data is laid out consecutively in memory. (this
          means row after row, slice after slice, with no space in between)
          @param extents      Extents of the region defined by data
          @param pixelFormat  Format of this buffer
          @param pixelData  Pointer to the actual data
        */
      PixelBox( const Box &extents, PixelFormat pixelFormat, void *pixelData = 0 );

      /** Constructor providing width, height and depth. This constructor
          assumes the pixel data is laid out consecutively in memory. (this
          means row after row, slice after slice, with no space in between)
          @param width      Width of the region
          @param height     Height of the region
          @param depth      Depth of the region
          @param pixelFormat  Format of this buffer
          @param pixelData    Pointer to the actual data
      */
      PixelBox( std::size_t width, std::size_t height, std::size_t depth, PixelFormat pixelFormat, void *pixelData = 0 );
      
      
      /** Set the rowPitch and slicePitch so that the buffer is laid out consecutive 
          in memory.
      */        
      void setConsecutive();

      /** Get the number of elements between one past the rightmost pixel of 
          one row and the leftmost pixel of the next row. (IE this is zero if rows
          are consecutive).
      */
      std::size_t getRowSkip() const;

      /** Get the number of elements between one past the right bottom pixel of
          one slice and the left top pixel of the next slice. (IE this is zero if slices
          are consecutive).
      */
      std::size_t getSliceSkip() const;

      /** Return whether this buffer is laid out consecutive in memory (ie the pitches
          are equal to the dimensions)
      */        
      bool isConsecutive() const;

      /** Return the size (in bytes) this image would take if it was
          laid out consecutive in memory
      */
      std::size_t getConsecutiveSize() const;

      /** Return a subvolume of this PixelBox.
          @param def  Defines the bounds of the subregion to return
          @return A pixel box describing the region and the data in it
          @remarks  This function does not copy any data, it just returns
            a PixelBox object with a data pointer pointing somewhere inside 
            the data of object.
          @throws Exception(ERR_INVALIDPARAMS) if def is not fully contained
      */
      PixelBox getSubVolume( const Box &def ) const;
      
      /** Get colour value from a certain location in the PixelBox. The z coordinate
          is only valid for cubemaps and volume textures. This uses the first (largest)
          mipmap.
       */
      Colour getColourAt( std::size_t x, std::size_t y, std::size_t z );

      /** Set colour value at a certain location in the PixelBox. The z coordinate
          is only valid for cubemaps and volume textures. This uses the first (largest)
          mipmap.
       */
      void setColourAt( Colour const &cv, std::size_t x, std::size_t y, std::size_t z );

};

}

#endif // __NEBULAE_COMMON_PIXELBOX_H__