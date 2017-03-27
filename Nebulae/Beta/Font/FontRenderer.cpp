
#include "FontRenderer.h"
#include "FreeTypeHelper.h"

#include <Nebulae/Beta/SpriteBatch/SpriteBatch.h>

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>

#include <utf8/checked.h>

#include <ft2build.h>
#include FT_FREETYPE_H


using namespace Nebulae;


namespace {

  template <class T>
  T NextPowerOfTwo(T input)
  {
    T value(1);
    while( value < input ) {
      value *= 2;
    }
    return value;
  }

  /** This is used to collect data on the glyphs as they are recorded into buffers, 
      for use in creating Glyph objects at the end of Font's constructor.*/
  struct TempGlyphData
  {
    TempGlyphData() {}
      TempGlyphData(std::size_t i, const Nebulae::Point& ul_, const Nebulae::Point& lr_) :
      idx(i), ul(ul_), lr(lr_) {}

    std::size_t    idx;      ///< index into m_textures of texture that contains this glyph
    Nebulae::Point ul, lr;   ///< area of glyph subtexture within texture
  };

  const int FT_MAGIC_NUMBER = 4; // taken from the ftview FreeType demo (I have no idea....)
}


///////////////////////////////////////
// class Nebulae::FontRenderer
///////////////////////////////////////

FontRenderer::FontRenderer( RenderSystemPtr renderSystem, SpriteBatch* batcher, const std::shared_ptr<Font>& font ) 
  : OverlayRenderer( renderSystem, batcher ),
    m_font( font )
{}


const std::shared_ptr<Font > 
FontRenderer::GetFont() const 
{
  return m_font;
}


bool
FontRenderer::Init( File* is ) 
{ 
  // Create the FontFace interface object.
  FT::FontFaceProxy face;
  if( is != NULL ) 
  {
    face.GetFace( *is );
  } 
  else 
  { 
    face.GetFace( m_font->FontName().c_str() );
  }
  
  face.SetCharSize( m_font->PointSize() );

  // we always need these whitespace, number, and punctuation characters
  std::vector<std::pair<boost::uint32_t, boost::uint32_t> > range_vec(
    PRINTABLE_ASCII_NONALPHA_RANGES.begin(),
    PRINTABLE_ASCII_NONALPHA_RANGES.end());

  // add ASCII letter characters or user-specified scripts to them, if the user specifies a specific set of
  // characters
  if( m_font->UnicodeCharsets().empty() ) {
    range_vec.insert( range_vec.end(),
                      PRINTABLE_ASCII_ALPHA_RANGES.begin(),
                      PRINTABLE_ASCII_ALPHA_RANGES.end() );
  } else {
    typedef std::pair<boost::uint32_t, boost::uint32_t> Pair;
    for( std::size_t i = 0; i < m_font->UnicodeCharsets().size(); ++i ) {
      range_vec.push_back(Pair(m_font->UnicodeCharsets()[i].m_first_char, m_font->UnicodeCharsets()[i].m_last_char));
    }
  }

  // define default image buffer size
  const int BUF_WIDTH(256);
  const int BUF_HEIGHT(256);
  const std::size_t BUF_SZ = Value(BUF_WIDTH) * Value(BUF_HEIGHT);

  // declare std::vector of image buffers into which we will copy glyph images and create first buffer
  std::vector<boost::uint16_t*>            buffer_vec; // 16 bpp: we are creating a luminance + alpha image
  std::vector<Point>                       buffer_sizes;
  std::map<boost::uint32_t, TempGlyphData> temp_glyph_data;
  boost::uint16_t* temp_buf = new boost::uint16_t[BUF_SZ]; // 16 bpp: we are creating a luminance + alpha image
  std::memset(temp_buf, 0, BUF_SZ);
  buffer_vec.push_back(temp_buf);
  buffer_sizes.push_back(Point(BUF_WIDTH, BUF_HEIGHT));

  int x     = 0;
  int y     = 0;
  int max_x = 0;
  for( std::size_t i = 0; i < range_vec.size(); ++i ) {
    boost::uint32_t low  = range_vec[i].first;
    boost::uint32_t high = range_vec[i].second;

    // copy glyph images
    for( boost::uint32_t c = low; c < high; ++c ) {
      if( temp_glyph_data.find(c) == temp_glyph_data.end() && face.GenerateGlyph(c) ) {
        if( x + face.GetBitmapWidth() >= BUF_WIDTH ) { // start a new row of glyph images
          if( x > max_x ) max_x = x;
          x = 0;
          y += m_font->Height();
          if( y + m_font->Height() >= BUF_HEIGHT ) { // if there's not enough room for another row, create a new buffer
            // cut off bottom portion of buffer just written, if it is possible to do so and maintain power-of-two height
            int pow_of_2_x( NextPowerOfTwo(max_x) );
            int pow_of_2_y( NextPowerOfTwo(y + m_font->Height()) );
            if( pow_of_2_y < buffer_sizes.back().y ) {
              buffer_sizes.back().y = pow_of_2_y;
            }
            if( pow_of_2_x < buffer_sizes.back().x ) {
              buffer_sizes.back().x = pow_of_2_x;
            }
            x = 0;
            y = 0;
            temp_buf = new boost::uint16_t[BUF_SZ];
            std::memset(temp_buf, 0, BUF_SZ);
            buffer_vec.push_back(temp_buf);
            buffer_sizes.push_back(Point(BUF_WIDTH, BUF_HEIGHT));
          }
        }

        boost::uint8_t*  src_start = face.GetBitmapBuffer();
        boost::uint16_t* dst_start = buffer_vec.back() + Value(y) * Value(BUF_WIDTH) + Value(x);

        int dst_row = 0;
        for( int src_row = face.GetBitmapHeight()-1; src_row >= 0; --src_row ) {
          boost::uint8_t*  src = src_start + src_row * face.GetBitmapPitch();
          boost::uint16_t* dst = dst_start + dst_row * Value(BUF_WIDTH);
          for( int col = 0; col < face.GetBitmapWidth(); ++col ) {
#ifdef __BIG_ENDIAN__
            *dst++ = *src++ | (255 << 8); // big-endian uses different byte ordering
#else
            *dst++ = (*src++ << 8) | 255; // alpha is the value from glyph_bitmap; luminance is always 100% white
#endif
          }
          ++dst_row;
        }

        // record info on how to find and use this glyph later
        temp_glyph_data[c] =
          TempGlyphData( static_cast<int>(buffer_vec.size()) - 1,
                         Point(x, y), 
                         Point(x + face.GetBitmapWidth(), y + face.GetBitmapHeight()) );
          
        // advance buffer write-position
        x += face.GetBitmapWidth();
      }
    }
  }

  // cut off bottom portion of last buffer, if it is possible to do so and maintain power-of-two height
  if (x > max_x) max_x = x;
  int pow_of_2_x( NextPowerOfTwo(max_x) );
  int pow_of_2_y( NextPowerOfTwo(y + m_font->Height()) );
  if( pow_of_2_y < buffer_sizes.back().y ) {
    buffer_sizes.back().y = pow_of_2_y;
  }
  if( pow_of_2_x < buffer_sizes.back().x ) {
    buffer_sizes.back().x = pow_of_2_x;
  }

  // Create texture from buffer(s) and release buffer(s)
  for( std::size_t i = 0; i < buffer_vec.size(); ++i ) {
    std::string name( m_font->FontName() );
    Texture* texture = m_renderDevice->CreateTexture( name, false );
    texture->LoadFromMemory( (unsigned char*)(buffer_vec[i]), 4/*GL_UNSIGNED_BYTE*/, 2/*GL_LUMINANCE_ALPHA*/, buffer_sizes[i].x, buffer_sizes[i].y );
    m_textures.push_back( texture );
    delete [] buffer_vec[i];
  }

  // Create Glyph objects from temp glyph data
  for( std::map<boost::uint32_t, TempGlyphData>::iterator it = temp_glyph_data.begin(); it != temp_glyph_data.end(); ++it ) {
    Texture*    texture = m_textures[it->second.idx];
    std::size_t width   = texture->GetWidth();
    std::size_t height  = texture->GetHeight();

    m_glyphTextures[it->first] = SubTexture( texture, float(it->second.ul.x)/width, float(it->second.ul.y)/height, float(it->second.lr.x)/width, float(it->second.lr.y)/height );
  }

  return OverlayRenderer::Init();
}


int 
FontRenderer::RenderGlyph( const Point& pt, char c, RenderState* render_state ) const
{
  if( !detail::ValidUTFChar<char>()(c) ) {
    throw utf8::invalid_utf8(c);
  }

  return RenderGlyph( pt, CharToUint32_t(c), render_state );
}


int
FontRenderer::RenderGlyph( const Point& pt, boost::uint32_t c, RenderState* render_state ) const
{
  Font::const_iterator glyph_it = m_font->find( c );
  if( glyph_it == m_font->end() )
  {
    glyph_it = m_font->find( WIDE_SPACE );
  } 

  GlyphTextureMap::const_iterator texture_it = m_glyphTextures.find( c );
  if( texture_it == m_glyphTextures.end() ) {
    texture_it = m_glyphTextures.find( WIDE_SPACE );
  }

  return RenderGlyph( pt, glyph_it->second, texture_it->second, render_state );
}


int
FontRenderer::RenderText( const Point& pt_, const std::string& text, const Colour& colour, RenderState* render_state  ) const
{
  RenderState state;
  if( !render_state ) {
    render_state = &state;
  }

  render_state->colours.push( colour );

  Point                       pt     = pt_;
  int                         orig_x = pt.x;
  std::string::const_iterator it     = text.begin();
  std::string::const_iterator end_it = text.end();
  while( it != end_it ) {
    pt.x += RenderGlyph( pt, utf8::next(it, end_it), render_state );
  }

  return pt.x - orig_x;
}


void 
FontRenderer::RenderText( const Point& ul, const Point& lr, const std::string& text, Flags<TextFormat>& format,
                          const Colour& colour, const std::vector<Font::LineData>* line_data, 
                          RenderState* render_state ) const
{
  RenderState state;
  if( !render_state ) {
    render_state = &state;
  }

  render_state->colours.push( colour );

  // get breakdown of how text is divided into lines
  std::vector<Font::LineData > lines;
  if (!line_data) {
    assert(0);
    m_font->DetermineLines( text, format, lr.x - ul.x, lines );
    line_data = &lines;
  }

  RenderText( ul, lr, text, format, *line_data, *render_state,
              0, CP0, line_data->size(), CPSize(line_data->back().char_data.size()) );
}




void 
FontRenderer::RenderText( const Point& ul, const Point& lr, const std::string& text, Flags<TextFormat>& format,
                          const std::vector<Font::LineData>& line_data, RenderState& render_state,
                          std::size_t begin_line, CPSize begin_char,
                          std::size_t end_line, CPSize end_char ) const
{
  int y_origin = ul.y; // default value for FORMAT_TOP
  if ( format & FORMAT_BOTTOM ) 
  {
    y_origin = lr.y - (static_cast<int>(end_line - begin_line - 1) * m_font->Lineskip() + m_font->Height());
  } 
  else if ( format & FORMAT_VCENTER ) 
  {
    y_origin = ul.y + ((lr.y - ul.y) - (static_cast<int>(end_line - begin_line - 1) * m_font->Lineskip() + m_font->Height())) / 2.0f;
  }


  std::string::const_iterator     str_begin_it  = text.begin();
  std::string::const_iterator     str_end_it    = text.end();
  int x_origin, ascent = m_font->Ascent();
  Point pt;
  GlyphTextureMap::const_iterator end_itr       = m_glyphTextures.end();
  GlyphTextureMap::const_iterator wide_space    = m_glyphTextures.find( WIDE_SPACE );
  GlyphTextureMap::const_iterator it;

  std::vector<GlyphState >      glyphs( 1024 );
  std::vector<boost::uint32_t > code_points( 1024 ); 

  // sizeof( Font::LineData ) == 20, ~3 pe cache line
  for ( std::size_t i = begin_line; i < end_line; ++i ) 
  {
    const Font::LineData& line = line_data[i];


  //
  // Determine the glyph positioning
  //

    x_origin = ul.x; // default value for FORMAT_LEFT

    if ( line.justification == ALIGN_RIGHT )
    {
      x_origin = lr.x - line.Width();
    }
    else if ( line.justification == ALIGN_CENTER )
    {
      x_origin = ul.x + ((lr.x - ul.x) - line.Width()) / 2.0f;
    }

    pt.x = x_origin;
    pt.y = y_origin + static_cast<int>(i - begin_line) * m_font->Lineskip();

    CPSize j   = ((i == begin_line) ? begin_char : CP0), 
           end = ((i == end_line - 1) ? end_char : CPSize(line.char_data.size()));

    assert( Value( end - j ) < 1024 );

    std::size_t idx = 0;

    memset( glyphs.data(), 0, 1024*sizeof(GlyphState) );
    memset( code_points.data(), 0, 1024*sizeof(boost::uint32_t) );

    // sizeof( Font::LineData::CharData ) == 24, ~2.666667 per cache line
    for( ; j < end; ++j ) 
    {        
      auto char_data = line.char_data[ Value(j) ];

      code_points[idx] = char_data.code_point;

      const Font::Glyph& glyph = char_data.glyph;

      /// sizeof(GlyphState) == 32; Thus 2 per cache line.
      GlyphState& glyph_state = glyphs[idx];

      glyph_state.upperLeft.x   = pt.x + glyph.left_bearing;
      glyph_state.lowerRight.x  = pt.x + glyph.left_bearing + glyph.width;
      glyph_state.lowerRight.y  = pt.y + ascent + ( glyph.height - glyph.vertical_bearing );
      glyph_state.upperLeft.y   = pt.y + ascent - glyph.vertical_bearing;
      glyph_state.colour        = render_state.colours.empty() ? CLR_WHITE : render_state.colours.top();

      pt.x += glyph.advance;

      idx++;
    }


  //
  // Find the glyphs
  //
    assert( code_points.size() == glyphs.size() );

    std::size_t n = idx;
    idx = 0;
    for ( ; idx < n; ++idx )
    {
      /// This will always require a cache fetch! ~150 clocks. 
      it = m_glyphTextures.find( code_points[idx] );
      if ( it == end_itr ) 
      {
        it = wide_space;
      }

      glyphs[idx].sub_texture = &it->second;
    }

  //
  // Push the glyphs to the batcher.
  //
    if ( !glyphs.empty() )
    {
      m_batcher->AddQuad( &glyphs[0], n );
    }
  }
}



int 
FontRenderer::RenderGlyph( const Point& pt, const Font::Glyph& glyph, const SubTexture& subtexture, const RenderState* render_state ) const 
{
  int    x2     = pt.x + glyph.left_bearing + glyph.width;
  int    y2     = pt.y + m_font->Ascent() + ( glyph.height - glyph.vertical_bearing );
  int    x1     = pt.x + glyph.left_bearing;
  int    y1     = pt.y + m_font->Ascent() - glyph.vertical_bearing;
  Colour colour = (!render_state || render_state->colours.empty()) ? CLR_WHITE : render_state->colours.top();

  DrawQuad( m_renderDevice, Point(x1,y1), Point(x2,y2), colour, &subtexture );

  return glyph.advance;
}