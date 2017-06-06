#include "FreeTypeHelper.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace {

  struct FTLibraryWrapper
  {
    FTLibraryWrapper() : m_library(0)
    {
      if( !m_library && FT_Init_FreeType(&m_library) ) { // if no library exists and we can't create one...
        NE_ASSERT( false, "Unable to initialize FreeType font library object" )();
      }
    }

    ~FTLibraryWrapper() { FT_Done_FreeType(m_library); }

    FT_Library m_library;
  } g_library;


  char* get_file_contents( Nebulae::File& file, std::size_t *iLength )
  {
    char* szBuffer;

    file.SeekToEnd();
    (*iLength) = file.Tell(); 
    file.Seek( 0 );

    szBuffer = new char[ *iLength+1 ];
    file.Read( szBuffer, *iLength );
    szBuffer[ *iLength ] = '\0';

    return szBuffer;
  }

  const double ITALICS_SLANT_ANGLE = 12; // degrees
  const double ITALICS_FACTOR      = 1.0 / tan((90 - ITALICS_SLANT_ANGLE) * 3.1415926 / 180.0); // factor used to shear glyphs ITALICS_SLANT_ANGLE degrees CW from straight up

}

using namespace Nebulae;
using namespace FT;


FontFaceProxy::FontFaceProxy() :
    m_face(0)
{}

FontFaceProxy::~FontFaceProxy()
{ if (m_face) FT_Done_Face(m_face); }


FT_Error 
FontFaceProxy::GetFace( const char* filename )
{ 
  FT_Error error = FT_New_Face( g_library.m_library, filename, 0, &m_face ); 
  CheckFace( error );  
  return error;
}


FT_Error 
FontFaceProxy::GetFace( File& file )
{
  std::size_t length;
  char* buffer = get_file_contents( file, &length );
  if( buffer == NULL ) { 
    return -1; 
  }

  FT_Error error = FT_New_Memory_Face( g_library.m_library, (FT_Byte*)buffer, (int)length, 0, &m_face );
  CheckFace( error );  
  return error;
}


bool 
FontFaceProxy::SetCharSize( int size ) 
{
  // Set the character size
  if( FT_Set_Char_Size(m_face, 0, size * 64, 0, 0) ) { // if error is returned
    NE_ASSERT( false, "Could not set font size while attempting to create font" )();
    return false;
  }
  return true;
}


int 
FontFaceProxy::GetHeight() const
{ return GetAscent() - GetDecent() + 1; }


int 
FontFaceProxy::GetAscent() const
{ 
  // convert from fixed-point 26.6 format
  return static_cast<int>(m_face->size->metrics.ascender / 64.0);   
}


int 
FontFaceProxy::GetDecent() const
{ 
  // convert from fixed-point 26.6 format
  return static_cast<int>(m_face->size->metrics.descender / 64.0); 
}


int 
FontFaceProxy::GetLineSkip() const
{ return static_cast<int>(m_face->size->metrics.height / 64.0); }


double 
FontFaceProxy::GetUnderlineOffset() const
{ 
  return std::floor(FT_MulFix(m_face->underline_position, m_face->size->metrics.y_scale) / 64.0); 
}


double
FontFaceProxy::GetUnderlineHeight() const
{
  return std::max<int>( 1.0, std::ceil(FT_MulFix(m_face->underline_thickness, m_face->size->metrics.y_scale) / 64.0) );
}


double
FontFaceProxy::GetItalicOffset() const
{ return (ITALICS_FACTOR * GetHeight() / 2.0); }


bool 
FontFaceProxy::GenerateGlyph( boost::uint32_t ch )
{
  bool retval = true;

  // load the glyph
  NE_ASSERT( m_face != NULL, "Invalid font or font face" )();
  
  //using boost::lexical_cast;
  FT_UInt index = FT_Get_Char_Index(m_face, ch);
  if( index ) {
    if( FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT) ) {
      NE_ASSERT( false, "Freetype could not load the glyph for character '%i'", ch )();
    }

    FT_GlyphSlot glyph = m_face->glyph;

    // render the glyph
    if( FT_Render_Glyph(glyph, ft_render_mode_normal) ) {
      NE_ASSERT( false, "Freetype could not render the glyph for character '%i'", ch )();
    }
  } else {
    retval = false;
  }

  return retval;
}


int
FontFaceProxy::GetGlyphTop() const
///
/// Returns, for the currently bound glyph, the distance from the baseline to the top-most glyph scanline, upwards 
/// y coordinates being positive.
/// 
{ return m_face->glyph->bitmap_top; }


int 
FontFaceProxy::GetGlyphLeftBearing() const
{ return static_cast<int>((std::ceil(m_face->glyph->metrics.horiBearingX / 64.0))); }


int  
FontFaceProxy::GetGlyphVerticalBearing() const 
{ return static_cast<int>((std::ceil(m_face->glyph->metrics.horiBearingY / 64.0))); }


int 
FontFaceProxy::GetGlyphAdvance() const
{ return static_cast<int>((std::ceil(m_face->glyph->metrics.horiAdvance / 64.0))); }


int 
FontFaceProxy::GetGlyphWidth() const 
{ return static_cast<int>((std::ceil(m_face->glyph->metrics.width / 64.0))); }


int 
FontFaceProxy::GetGlyphHeight() const 
{ return static_cast<int>((std::ceil(m_face->glyph->metrics.height / 64.0))); }


uint8* 
FontFaceProxy::GetBitmapBuffer() const
{ return m_face->glyph->bitmap.buffer; }


int
FontFaceProxy::GetBitmapPitch() const
{ return m_face->glyph->bitmap.pitch; }


int 
FontFaceProxy::GetBitmapWidth() const
{ return m_face->glyph->bitmap.width; }


int
FontFaceProxy::GetBitmapHeight() const
{ return m_face->glyph->bitmap.rows; }


void
FontFaceProxy::CheckFace( FT_Error error )
{
  NE_ASSERT( error == 0 && m_face != NULL, "Face object created from %s was invalid" )();
  NE_ASSERT( FT_IS_SCALABLE(m_face), "Attempted to create font %s with unscalable font face" )();
}