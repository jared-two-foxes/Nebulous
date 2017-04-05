#ifndef __NEBULAE_GLPIXELCONVERTIONUTIL_H__
#define __NEBULAE_GLPIXELCONVERTIONUTIL_H__

#include <Config.h>

namespace Nebulae
{
  enum PixelFormat : uint16;

  /**
   *  GLPixelConversionUtil. Class to do pixel format mapping between GL and Nebulae::PixelFormat
	 */
	class GLPixelConversionUtil
	{
	public:

		/** Takes the OGRE pixel format and returns the appropriate GL one
			  @return a GLenum describing the format, or 0 if there is no exactly matching 
			    one (and conversion is needed)
		*/
		static GLenum GetGLOriginFormat( PixelFormat pixelFormat );
	
		/** Takes the OGRE pixel format and returns type that must be provided
			  to GL as data type for reading it into the GPU
			  @return a GLenum describing the data type, or 0 if there is no exactly matching 
			    one (and conversion is needed)
		*/
		static GLenum GetGLOriginDataType( PixelFormat pixelFormat );
        
    /**	Takes the OGRE pixel format and returns the type that must be provided
			to GL as internal format. GL_NONE if no match exists.
		  @param mFormat The pixel format
		  @param hwGamma Whether a hardware gamma-corrected version is requested
		*/
		static GLenum GetGLInternalFormat( PixelFormat pixelFormat );
	
		/**	Takes the OGRE pixel format and returns the type that must be provided
			  to GL as internal format. If no match exists, returns the closest match.
		    @param mFormat The pixel format
		    @param hwGamma Whether a hardware gamma-corrected version is requested
		*/
		static GLenum GetClosestGLInternalFormat( PixelFormat pixelFormat );
		
		/**	Function to get the closest matching PixelFormat to an internal GL format. To be
			  precise, the format will be chosen that is most efficient to transfer to the card 
			  without losing precision.
			  @remarks It is valid for this function to always return PF_A8R8G8B8.
		*/
		static PixelFormat GetClosestPixelFormat( GLenum fmt );
	
		/** Returns the maximum number of Mipmaps that can be generated until we reach
			  the minimum format possible. This does not count the base level.
			  @param width
				  The width of the area
			  @param height
				  The height of the area
			  @param depth
				  The depth of the area
			  @param format
				  The format of the area
			  @remarks
				  In case that the format is non-compressed, this simply returns
				  how many times we can divide this texture in 2 until we reach 1x1.
				  For compressed formats, constraints apply on minimum size and alignment
				  so this might differ.
		*/
		static size_t GetMaxMipmaps( std::size_t width, std::size_t height, std::size_t depth, PixelFormat pixelFormat );
        
	};
} //namespace

#endif // __NEBULAE_GLPIXELCONVERTIONUTIL_H__