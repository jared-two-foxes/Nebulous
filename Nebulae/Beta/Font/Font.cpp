
#include "Font.h"
#include "FreeTypeHelper.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <utf8/checked.h>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <boost/assign/list_of.hpp>

#include <regex>

#include <cmath>
#include <cctype>
#include <numeric>
#include <sstream>

using namespace Nebulae;

namespace {

  void SetJustification(bool& last_line_of_curr_just, Font::LineData& line_data, Alignment orig_just, Alignment prev_just)
  {
    if (last_line_of_curr_just) {
      line_data.justification = orig_just;
      last_line_of_curr_just  = false;
    } else {
      line_data.justification = prev_just;
    }
  }

}



///////////////////////////////////////
// Constants
///////////////////////////////////////
const StrSize         Nebulae::S0(0);
const StrSize         Nebulae::S1(1);
const StrSize         Nebulae::INVALID_STR_SIZE(std::numeric_limits<std::size_t>::max());
const CPSize          Nebulae::CP0(0);
const CPSize          Nebulae::CP1(1);
const CPSize          Nebulae::INVALID_CP_SIZE(std::numeric_limits<std::size_t>::max());

const boost::uint32_t Nebulae::WIDE_SPACE       = ' ';
const boost::uint32_t Nebulae::WIDE_NEWLINE     = '\n';
const boost::uint32_t Nebulae::WIDE_CR          = '\r';
const boost::uint32_t Nebulae::WIDE_FF          = '\f';
const boost::uint32_t Nebulae::WIDE_TAB         = '\t';

const std::string     Nebulae::ALIGN_LEFT_TAG   = "left";
const std::string     Nebulae::ALIGN_CENTER_TAG = "center";
const std::string     Nebulae::ALIGN_RIGHT_TAG  = "right";
const std::string     Nebulae::PRE_TAG          = "pre";

const std::vector<std::pair<boost::uint32_t, boost::uint32_t> > Nebulae::PRINTABLE_ASCII_ALPHA_RANGES = boost::assign::list_of
    (std::pair<boost::uint32_t, boost::uint32_t>(0x41, 0x5B))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x61, 0x7B));

const std::vector<std::pair<boost::uint32_t, boost::uint32_t> > Nebulae::PRINTABLE_ASCII_NONALPHA_RANGES = boost::assign::list_of
    (std::pair<boost::uint32_t, boost::uint32_t>(0x09, 0x0D))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x20, 0x21))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x30, 0x3A))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x21, 0x30))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x3A, 0x41))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x5B, 0x61))
    (std::pair<boost::uint32_t, boost::uint32_t>(0x7B, 0x7F));


///////////////////////////////////////
// TextFormat
///////////////////////////////////////
const TextFormat Nebulae::FORMAT_NONE         (0);
const TextFormat Nebulae::FORMAT_VCENTER      (1 << 0);
const TextFormat Nebulae::FORMAT_TOP          (1 << 1);
const TextFormat Nebulae::FORMAT_BOTTOM       (1 << 2);
const TextFormat Nebulae::FORMAT_CENTER       (1 << 3);
const TextFormat Nebulae::FORMAT_LEFT         (1 << 4);
const TextFormat Nebulae::FORMAT_RIGHT        (1 << 5);
const TextFormat Nebulae::FORMAT_WORDBREAK    (1 << 6);
const TextFormat Nebulae::FORMAT_LINEWRAP     (1 << 7);
const TextFormat Nebulae::FORMAT_IGNORETAGS   (1 << 8);

NE_FLAGSPEC_IMPL(TextFormat);

namespace {
  bool RegisterTextFormats()
  {
    FlagSpec<TextFormat>& spec = FlagSpec<TextFormat>::instance();
    spec.insert(FORMAT_NONE, "FORMAT_NONE", true);
    spec.insert(FORMAT_VCENTER, "FORMAT_VCENTER", true);
    spec.insert(FORMAT_TOP, "FORMAT_TOP", true);
    spec.insert(FORMAT_BOTTOM, "FORMAT_BOTTOM", true);
    spec.insert(FORMAT_CENTER, "FORMAT_CENTER", true);
    spec.insert(FORMAT_LEFT, "FORMAT_LEFT", true);
    spec.insert(FORMAT_RIGHT, "FORMAT_RIGHT", true);
    spec.insert(FORMAT_WORDBREAK, "FORMAT_WORDBREAK", true);
    spec.insert(FORMAT_LINEWRAP, "FORMAT_LINEWRAP", true);
    spec.insert(FORMAT_IGNORETAGS, "FORMAT_IGNORETAGS", true);
    return true;
  }
  bool dummyTextFormatReturn = RegisterTextFormats();
}


///////////////////////////////////////
// Alignment
///////////////////////////////////////

const Alignment Nebulae::ALIGN_NONE         (0);
const Alignment Nebulae::ALIGN_VCENTER      (1 << 0);
const Alignment Nebulae::ALIGN_TOP          (1 << 1);
const Alignment Nebulae::ALIGN_BOTTOM       (1 << 2);
const Alignment Nebulae::ALIGN_CENTER       (1 << 3);
const Alignment Nebulae::ALIGN_LEFT         (1 << 4);
const Alignment Nebulae::ALIGN_RIGHT        (1 << 5);

NE_FLAGSPEC_IMPL(Alignment);

namespace {
  bool RegisterAlignments()
  {
    FlagSpec<Alignment>& spec = FlagSpec<Alignment>::instance();
    spec.insert(ALIGN_NONE, "ALIGN_NONE", true);
    spec.insert(ALIGN_VCENTER, "ALIGN_VCENTER", true);
    spec.insert(ALIGN_TOP, "ALIGN_TOP", true);
    spec.insert(ALIGN_BOTTOM, "ALIGN_BOTTOM", true);
    spec.insert(ALIGN_CENTER, "ALIGN_CENTER", true);
    spec.insert(ALIGN_LEFT, "ALIGN_LEFT", true);
    spec.insert(ALIGN_RIGHT, "ALIGN_RIGHT", true);
    return true;
  }
  bool dummyAlignmentReturn = RegisterAlignments();
}


///////////////////////////////////////
// Free Functions
///////////////////////////////////////

CPSize Nebulae::CodePointIndexOf(std::size_t line, CPSize index, const std::vector<Font::LineData>& line_data)
{
  CPSize retval(0);
  if( line_data.size() <= line ) {
    std::vector<Font::LineData>::const_reverse_iterator it     = line_data.rbegin();
    std::vector<Font::LineData>::const_reverse_iterator end_it = line_data.rend();
    while( it != end_it ) {
      if( !it->char_data.empty() ) {
        retval = it->char_data.back().code_point_index + 1;
        break;
      }
      ++it;
    }
  } else if( index < line_data[line].char_data.size() ) {
    retval = line_data[line].char_data[Value(index)].code_point_index;
  } else {
    std::vector<Font::LineData>::const_reverse_iterator it =
      line_data.rbegin() + (line_data.size() - 1 - line);
    std::vector<Font::LineData>::const_reverse_iterator end_it = line_data.rend();
    while( it != end_it ) {
      if( !it->char_data.empty() ) {
        retval = it->char_data.back().code_point_index + 1;
        break;
      }
      ++it;
    }
  }
  return retval;
}

StrSize Nebulae::StringIndexOf(std::size_t line, CPSize index, const std::vector<Font::LineData>& line_data)
{
  StrSize retval(0);
  if (line_data.size() <= line) {
    std::vector<Font::LineData>::const_reverse_iterator it = line_data.rbegin();
    std::vector<Font::LineData>::const_reverse_iterator end_it = line_data.rend();
    while (it != end_it) {
      if (!it->char_data.empty()) {
        retval = it->char_data.back().string_index + it->char_data.back().string_size;
        break;
      }
      ++it;
    }
  } else if (index < line_data[line].char_data.size()) {
    retval = line_data[line].char_data[Value(index)].string_index;
  } else {
    std::vector<Font::LineData>::const_reverse_iterator it =
      line_data.rbegin() + (line_data.size() - 1 - line);
    std::vector<Font::LineData>::const_reverse_iterator end_it = line_data.rend();
    while( it != end_it ) {
      if( !it->char_data.empty() ) {
        retval = it->char_data.back().string_index + it->char_data.back().string_size;
        break;
      }
      ++it;
    }
  }
  return retval;
}

std::pair<std::size_t, CPSize> Nebulae::LinePositionOf(CPSize index, const std::vector<Font::LineData>& line_data)
{
  std::pair<std::size_t, CPSize> retval(std::numeric_limits<std::size_t>::max(), INVALID_CP_SIZE);
  for( std::size_t i = 0; i < line_data.size(); ++i ) {
    if( line_data[i].char_data.front().code_point_index <= index &&
      index <= line_data[i].char_data.back().code_point_index ) {
      retval.first = i;
      retval.second = index - line_data[i].char_data.front().code_point_index;
      break;
    }
  }
  return retval;
}



class TextElementPool
{
public:
  std::vector<std::shared_ptr<Font::TextElement> > text_elements;

  void Allocate( int count = 512 ) {
    for ( int i = 0; i < count; ++i ) {
      std::shared_ptr<Font::TextElement> element = std::make_shared<Font::TextElement>(false, false);
      text_elements.push_back( std::move(element) );
    }
  }
 
  std::shared_ptr<Font::TextElement > Fetch() {
    if( text_elements.empty() ) {
      Allocate(); // ?
    }

    std::shared_ptr<Font::TextElement > element = text_elements.back();
    text_elements.pop_back();
    return element;
  }

};

TextElementPool element_pool;

void SplitStringIntoLines( const std::string& text, std::vector<std::shared_ptr<Font::TextElement> >& text_elements )
{
  std::size_t current, offset = 0;
  while( (current = text.find_first_of('\n', offset)) != std::string::npos )
  {
    std::shared_ptr<Font::TextElement> element = element_pool.Fetch();
    element->text = text.substr( offset, current - offset );
    offset = current + 1;
    text_elements.push_back( std::move(element) );
  }

  if( offset < text.length() )
  {
    std::shared_ptr<Font::TextElement> element = element_pool.Fetch();
    element->text = text.substr( offset, text.size() );
    text_elements.push_back( std::move(element) );
  }
}

///////////////////////////////////////
// class Nebulae::Font::TextElement
///////////////////////////////////////
Font::TextElement::TextElement() 
: whitespace(false),
  newline(false),
  cached_width(-1)
{}

Font::TextElement::TextElement(bool ws, bool nl) 
: whitespace(ws),
  newline(nl),
  cached_width(-1)
{}

Font::TextElement::TextElement(const Font::TextElement& other) // Copy constructor
: whitespace(other.whitespace),
  newline(other.newline),
  cached_width(other.cached_width)
{
  text   = other.text;
  widths = other.widths;
}

Font::TextElement::TextElement(Font::TextElement&& other) // Move constructor
: whitespace(other.whitespace),
  newline(other.newline),
  cached_width(other.cached_width)
{
  text = other.text;
  widths = other.widths; 
}

Font::TextElement::~TextElement()
{}

Font::TextElement& Font::TextElement::operator=(const TextElement&) 
{ 
  return *this; 
}  // Copy assignment operator

Font::TextElement& Font::TextElement::operator=(Font::TextElement&& other)  // Move assignment operator
{ 
  //whitespace = other.whitespace;
  //newline    = other.newline;
  return *this;
} 

int Font::TextElement::Width() const
{
  if( cached_width == -1 ) {
    cached_width = std::accumulate(widths.begin(), widths.end(), 0);
  }
  return cached_width;
}

StrSize Font::TextElement::StringSize() const
{ return StrSize(text.size()); }

CPSize Font::TextElement::CodePointSize() const
{ return CPSize(widths.size()); }


///////////////////////////////////////
// class Nebulae::Font::LineData
///////////////////////////////////////
Font::LineData::LineData() :
  justification(ALIGN_CENTER)
{}

int  Font::LineData::Width() const
{ return char_data.empty() ? 0 : char_data.back().extent; }

bool Font::LineData::Empty() const
{ return char_data.empty(); }


///////////////////////////////////////
// class Nebulae::Font::LineData::CharData
///////////////////////////////////////
Font::LineData::CharData::CharData() :
  extent(0),
  string_index(0)
{}

Font::LineData::CharData::CharData( int extent_, StrSize str_index, StrSize str_size, CPSize cp_index ) :
  extent(extent_),
  string_index(str_index),
  string_size(str_size),
  code_point_index(cp_index)
{}


///////////////////////////////////////
// struct Nebulae::Font::Glyph
///////////////////////////////////////
Font::Glyph::Glyph() :
  width(0),
  height(0),
  left_bearing(0),
  vertical_bearing(0),
  advance(0)
{}

Font::Glyph::Glyph( int w, int h, int lb, int vb, int adv ) :
  width(w),
  height(h),
  left_bearing(lb),
  vertical_bearing(vb),
  advance(adv)
{}


///////////////////////////////////////
// class Nebulae::Font
///////////////////////////////////////

Font::Font( const std::string& font_filename, uint32 pts ) 
: m_font_filename(font_filename),
  m_pt_sz(pts),
  m_ascent(0),
  m_descent(0),
  m_height(0),
  m_lineskip(0),
  m_space_width(0)
{}

Font::~Font()
{}

Font::const_iterator  Font::begin() const
{ return m_glyphs.begin(); }

Font::const_iterator  Font::end() const
{ return m_glyphs.end(); }

Font::iterator Font::find(const boost::uint32_t& pattern)
{ return m_glyphs.find(pattern); }

Font::const_iterator Font::find(const boost::uint32_t& pattern) const
{ return m_glyphs.find(pattern); }

const std::string& Font::FontName() const
{ return m_font_filename; }

unsigned int Font::PointSize() const
{ return m_pt_sz; }

const std::vector<UnicodeCharset>& Font::UnicodeCharsets() const
{ return m_charsets; }

int Font::Ascent() const
{ return m_ascent; }

int Font::Descent() const
{ return m_descent; }

int Font::Height() const
{ return m_height; }

int Font::Lineskip() const
{ return m_lineskip; }

int Font::SpaceWidth() const
{ return m_space_width; }


Nebulae::Point Font::DetermineLines( const std::string& text, Flags<TextFormat>& format, int box_width,
                            std::vector<LineData>& line_data ) const
{ return DetermineLinesImpl( text, format, box_width, line_data, 0 ); }

Nebulae::Point Font::DetermineLines( const std::string& text, Flags<TextFormat>& format, int box_width,
                            std::vector<LineData>& line_data,
                            std::vector<std::shared_ptr<TextElement> >& text_elements ) const
{
  NE_ASSERT(text_elements.empty(), "")();

  return DetermineLinesImpl( text, format, box_width, line_data, &text_elements );
}

Nebulae::Point Font::DetermineLines( const std::string& text, Flags<TextFormat>& format, int box_width,
                            const std::vector<std::shared_ptr<TextElement> >& text_elements,
                            std::vector<LineData>& line_data ) const
{
  NE_ASSERT(!text_elements.empty(), "")();
  return DetermineLinesImpl( text,
                             format,
                             box_width,
                             line_data,
                             const_cast<std::vector<std::shared_ptr<TextElement> >*>(&text_elements) );
}

Nebulae::Point Font::TextExtent( const std::string& text, Flags<TextFormat> format, int box_width ) const
{
  std::vector<LineData> lines;
  return DetermineLines( text, format, box_width ? box_width : int(1 << 15), lines );
}

Nebulae::Point Font::TextExtent( const std::string& text, const std::vector<LineData>& line_data ) const
{
  Nebulae::Point retval;
  for( std::size_t i = 0; i < line_data.size(); ++i ) {
    if( retval.x < line_data[i].Width() ) {
      retval.x = line_data[i].Width();
    }
  }
  retval.y = text.empty() ? 0 : (static_cast<int>(line_data.size()) - 1) * m_lineskip + m_height;
  return retval;
}

Nebulae::Point Font::DetermineLinesImpl( const std::string& text,
                                         Flags<TextFormat>& format,
                                         int box_width,
                                         std::vector<LineData>& line_data,
                                         std::vector<std::shared_ptr<TextElement> >* text_elements_ptr ) const
{
  std::vector<std::shared_ptr<TextElement> > local_text_elements;

  std::vector<std::shared_ptr<TextElement > >& text_elements =
      text_elements_ptr ? *text_elements_ptr : local_text_elements;

  if( text_elements.empty() ) 
  {
    ValidateFormat( format );

    SplitStringIntoLines( text, text_elements );

    // fill in the widths of code points in each TextElement
    const GlyphMap::const_iterator WIDE_SPACE_IT = m_glyphs.find( WIDE_SPACE );
    
    for ( std::size_t i = 0; i < text_elements.size(); ++i ) 
    {
      std::string::const_iterator it_     = text_elements[i]->text.begin();
      std::string::const_iterator end_it_ = text_elements[i]->text.end();
      
      while ( it_ != end_it_ ) 
      {
        text_elements[i]->widths.push_back( 0 );
        boost::uint32_t c = utf8::next( it_, end_it_ );
        if ( c != WIDE_NEWLINE ) 
        {
          GlyphMap::const_iterator glyph_itr = m_glyphs.find( c );
          // use a space when an unrendered glyph is requested (the
          // space chararacter is always renderable)
          if ( glyph_itr == m_glyphs.end() ) 
          {
            glyph_itr = WIDE_SPACE_IT;
          }
          
          text_elements[i]->widths.back() = glyph_itr->second.advance;
        }
      }
    }
  }

  int       tab_width              = 4;                         // default tab width
  int       tab_pixel_width        = tab_width * m_space_width; // get the length of a tab stop
  bool      expand_tabs            = format & FORMAT_LEFT;      // tab expansion only takes place when the lines are left-justified (otherwise, tabs are just spaces)
  bool      last_line_of_curr_just = false;                     // is this the last line of the current justification? (for instance when a </right> tag is encountered)
  Alignment orig_just              = ALIGN_NONE;
  if( format & FORMAT_LEFT )   orig_just = ALIGN_LEFT;
  if( format & FORMAT_CENTER ) orig_just = ALIGN_CENTER;
  if( format & FORMAT_RIGHT )  orig_just = ALIGN_RIGHT;  

  line_data.clear();
  line_data.push_back( LineData() );
  line_data.back().justification = orig_just;

  int x = 0;
  StrSize original_string_offset(0); // the position within the original string of the current TextElement
  CPSize code_point_offset(0);// the index of the first code point of the current TextElement

  const GlyphMap::const_iterator WIDE_SPACE_IT = m_glyphs.find( WIDE_SPACE );

  for  ( std::size_t i = 0, n = text_elements.size(); i < n; ++i ) 
  {
    std::shared_ptr<TextElement> elem   = text_elements[i];
    std::string::const_iterator  it     = elem->text.begin();
    std::string::const_iterator  end_it = elem->text.end();
    std::size_t idx = 0;

    while ( it != end_it ) 
    {
      StrSize char_index( std::distance(elem->text.cbegin(), it) );
      boost::uint32_t c = utf8::next( it, end_it );
      StrSize char_size = StrSize( std::distance(elem->text.cbegin(), it) - Value(char_index) );      
      GlyphMap::const_iterator glyph_itr = m_glyphs.find( c );
      
      // use a space when an unrendered glyph is requested (the space chararacter is always renderable)
      if ( glyph_itr == m_glyphs.end() ) 
      {
        glyph_itr = WIDE_SPACE_IT;
      }

      x += glyph_itr->second.advance;

      line_data.back().char_data.push_back(
        LineData::CharData( x,
                            original_string_offset + char_index, 
                            char_size,
                            code_point_offset) );

      line_data.back().char_data.back().code_point = c;
      line_data.back().char_data.back().glyph      = glyph_itr->second;

      ++idx;
      ++code_point_offset;
    }
    
    original_string_offset += elem->StringSize();

    if ( i < n )
    {
      line_data.push_back( LineData() );
      SetJustification( last_line_of_curr_just,
        line_data.back(),
        orig_just,
        line_data[line_data.size() - 2].justification );
      x = 0;
    }

    // Add new line character offset.
    original_string_offset += 1;
  }

  return TextExtent( text, line_data );
}

bool Font::Load( File* is )
{
  NE_ASSERT( m_pt_sz != 0, "Attempted to create font %s with 0 point size", m_font_filename.c_str() )();

  if( !m_pt_sz ) {  
    return false;
  }

  // Create the FontFace interface object.
  FT::FontFaceProxy face;
  if( is != NULL ) {
    face.GetFace( *is );
  } else {
    face.GetFace( m_font_filename.c_str() );
  }
  face.SetCharSize( m_pt_sz );
  
  // Get the scalable font metrics for this font
  m_ascent   = face.GetAscent();
  m_descent  = face.GetDecent();
  m_height   = face.GetHeight();
  m_lineskip = face.GetLineSkip();
  
  // we always need these whitespace, number, and punctuation characters
  std::vector<std::pair<boost::uint32_t, boost::uint32_t> > range_vec(
    PRINTABLE_ASCII_NONALPHA_RANGES.begin(),
    PRINTABLE_ASCII_NONALPHA_RANGES.end());

  // add ASCII letter characters or user-specified scripts to them, if the
  // user specifies a specific set of characters.
  if( m_charsets.empty() ) {
    range_vec.insert( range_vec.end(),
                      PRINTABLE_ASCII_ALPHA_RANGES.begin(),
                      PRINTABLE_ASCII_ALPHA_RANGES.end() );
  } else {
    typedef std::pair<boost::uint32_t, boost::uint32_t> Pair;
    for( std::size_t i = 0; i < m_charsets.size(); ++i ) {
      range_vec.push_back(Pair(m_charsets[i].m_first_char, m_charsets[i].m_last_char));
    }
  }

  for( std::size_t i = 0; i < range_vec.size(); ++i ) {
    boost::uint32_t low  = range_vec[i].first;
    boost::uint32_t high = range_vec[i].second;

    // record info on how to find and use this glyph later.
    for( boost::uint32_t c = low; c < high; ++c ) {
      if( m_glyphs.find(c) == m_glyphs.end() && face.GenerateGlyph(c) ) {
        m_glyphs[c] = Glyph( face.GetGlyphWidth(), 
                             face.GetGlyphHeight(),
                             face.GetGlyphLeftBearing(),
                             face.GetGlyphVerticalBearing(),
                             face.GetGlyphAdvance() );
      }
    }
  }

  // record the width of the space character
  GlyphMap::const_iterator glyph_it = m_glyphs.find(WIDE_SPACE);
  NE_ASSERT(glyph_it != m_glyphs.end(), "Unable to find the WIDE_SPACE gliph for font %s", m_font_filename.c_str() )();
  m_space_width = glyph_it->second.advance;

  return true;
}

void Font::ValidateFormat(Flags<TextFormat>& format) const
{
  // correct any disagreements in the format flags
  int dup_ct = 0;   // duplication count
  if( format & FORMAT_LEFT )   ++dup_ct;
  if( format & FORMAT_RIGHT )  ++dup_ct;
  if( format & FORMAT_CENTER ) ++dup_ct;
  if( dup_ct != 1 ) {   // exactly one must be picked; when none or multiples are picked, use FORMAT_LEFT by default
    format &= ~(FORMAT_RIGHT | FORMAT_CENTER);
    format |= FORMAT_LEFT;
  }
  dup_ct = 0;
  if( format & FORMAT_TOP )     ++dup_ct;
  if( format & FORMAT_BOTTOM )  ++dup_ct;
  if( format & FORMAT_VCENTER ) ++dup_ct;
  if( dup_ct != 1 ) {   // exactly one must be picked; when none or multiples are picked, use FORMAT_TOP by default
    format &= ~(FORMAT_BOTTOM | FORMAT_VCENTER);
    format |= FORMAT_TOP;
  }
  if( (format & FORMAT_WORDBREAK) && (format & FORMAT_LINEWRAP) ) {  // only one of these can be picked; FORMAT_WORDBREAK overrides FORMAT_LINEWRAP
    format &= ~FORMAT_LINEWRAP;
  }
}