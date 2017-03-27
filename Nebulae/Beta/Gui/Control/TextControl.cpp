
#include "TextControl.h"

#include <Nebulae/Common/Window/Window.h>
#include <Nebulae/Beta/Font/Font.h>

#include <utf8/checked.h>

using namespace Nebulae;


TextControl::TextControl( const WidgetFactory& factory, int x, int y, int w, int h, const std::string& str,
                          const std::shared_ptr<Font>& font, Colour colour, Flags<TextFormat> format, uint32 flags) 
: Control( factory, x, y, w, h, flags ),
  m_format( format ),
  m_text_color( colour ),
  m_clip_text( false ),
  m_set_min_size( false ),
  m_code_points( 0 ),
  m_font( font ),
  m_fit_to_text( false )
{
  ValidateFormat();
  SetText(str);
}

TextControl::TextControl( const WidgetFactory& factory, int x, int y, const std::string& str, 
                          const std::shared_ptr<Font>& font, Colour colour, Flags<TextFormat> format, uint32 flags )
: Control( factory, x, y, 0, 0, flags ),
  m_format( format ),
  m_text_color( colour ),
  m_clip_text( false ),
  m_set_min_size( false ),
  m_code_points( 0 ),
  m_font( font ),
  m_fit_to_text( true )
{
  ValidateFormat();
  SetText(str);
}


TextControl::~TextControl()
{

}

Nebulae::Point TextControl::GetMinUsableSize() const
{ return m_text_lr - m_text_ul; }

const std::string& TextControl::GetText() const
{ return m_text; }

Flags<TextFormat> TextControl::GetTextFormat() const
{ return m_format; }

Colour TextControl::TextColor() const
{ return m_text_color; }

bool TextControl::ClipText() const
{ return m_clip_text; }

bool TextControl::SetMinSize() const
{ return m_set_min_size; }

bool TextControl::Empty() const
{ return m_text.empty(); }

CPSize TextControl::Length() const
{ return m_code_points; }

Nebulae::Point TextControl::TextUpperLeft() const
{ return GetUpperLeft() + m_text_ul; }

Nebulae::Point TextControl::TextLowerRight() const
{ return GetUpperLeft() + m_text_lr; }

const std::vector<Font::LineData>& TextControl::GetLineData() const
{ return m_line_data; }

const std::shared_ptr<Font>& TextControl::GetFont() const
{ return m_font; }

void TextControl::SetText(const std::string& str)
{
  PROFILE;

  m_text = str;
  if( m_font ) {
    m_code_points = CPSize(utf8::distance(str.begin(), str.end()));
    m_text_elements.clear();
    Point text_sz = m_font->DetermineLines(m_text, m_format, GetClientSize().x, m_line_data, m_text_elements);
    m_text_ul     = Point();
    m_text_lr     = text_sz;
    AdjustMinimumSize();
    if( m_fit_to_text ) {
      Resize(text_sz);
    } else {
      RecomputeTextBounds();
    }
  }
}

void TextControl::SizeMove(const Nebulae::Point& ul, const Nebulae::Point& lr)
{
  Widget::SizeMove(ul, lr);
  bool redo_determine_lines = false;
  int client_width = GetClientSize().x;
  if( !m_fit_to_text && (m_format | FORMAT_WORDBREAK || m_format | FORMAT_LINEWRAP) ) {
    int text_width = m_text_lr.x - m_text_ul.x;
    redo_determine_lines =
      client_width < text_width ||
      (text_width < client_width && 1u < m_line_data.size());
  }
  if( redo_determine_lines ) {
    Point text_sz(0,0);
    if( m_text_elements.empty() ) {
      text_sz = m_font->DetermineLines(m_text, m_format, client_width, m_line_data, m_text_elements);
    } else {
      text_sz = m_font->DetermineLines(m_text, m_format, client_width, m_text_elements, m_line_data);
    }
    m_text_ul = Point(0,0);
    m_text_lr = text_sz;
    AdjustMinimumSize();
  }
  RecomputeTextBounds();
}

void TextControl::SetTextFormat(TextFormat format)
{
  m_format = format;
  ValidateFormat();
  if( m_format != format ) {
    SetText(m_text);
  }
}

void TextControl::SetTextColour(Colour colour)
{ m_text_color = colour; }

void TextControl::SetColour(Colour c)
{
  Control::SetColour(c);
  m_text_color = c;
}

void TextControl::ClipText(bool b)
{ m_clip_text = b; }

void TextControl::SetMinSize(bool b)
{
  m_set_min_size = b;
  AdjustMinimumSize();
}

void TextControl::Clear()
{ SetText(""); }

void TextControl::Insert(CPSize pos, char c)
{
  std::size_t line;
  std::tie(line, pos) = LinePositionOf(pos, m_line_data);
  Insert(line, pos, c);
}

void TextControl::Insert(CPSize pos, const std::string& s)
{
  std::size_t line;
  std::tie(line, pos) = LinePositionOf(pos, m_line_data);
  Insert(line, pos, s);
}

void TextControl::Erase(CPSize pos, CPSize num/* = CP1*/)
{
  std::size_t line;
  std::tie(line, pos) = LinePositionOf(pos, m_line_data);
  Erase(line, pos, num);
}

void TextControl::Insert(std::size_t line, CPSize pos, char c)
{
  if( !detail::ValidUTFChar<char>()(c) ) {
    throw utf8::invalid_utf8(c);
  }
  m_text.insert( Value(StringIndexOf(line, pos, m_line_data)), 1, c );
  SetText( m_text );
}

void TextControl::Insert( std::size_t line, CPSize pos, const std::string& s )
{
  m_text.insert( Value(StringIndexOf(line, pos, m_line_data)), s );
  SetText( m_text );
}

void TextControl::Erase(std::size_t line, CPSize pos, CPSize num/* = CP1*/)
{
  std::string::iterator it = m_text.begin() + Value(StringIndexOf(line, pos, m_line_data));
  std::string::iterator end_it = m_text.begin() + Value(StringIndexOf(line, pos + num, m_line_data));
  m_text.erase(it, end_it);
  SetText(m_text);
}

bool TextControl::FitToText() const
{ return m_fit_to_text; }

void TextControl::ValidateFormat()
{
  int dup_ct = 0;   // duplication count
  if( m_format & FORMAT_LEFT ) ++dup_ct;
  if( m_format & FORMAT_RIGHT ) ++dup_ct;
  if( m_format & FORMAT_CENTER ) ++dup_ct;
  if( dup_ct != 1 ) {   // exactly one must be picked; when none or multiples are picked, use FORMAT_LEFT by default
    m_format &= ~(FORMAT_RIGHT | FORMAT_CENTER);
    m_format |= FORMAT_LEFT;
  }
  dup_ct = 0;
  if( m_format & FORMAT_TOP ) ++dup_ct;
  if( m_format & FORMAT_BOTTOM ) ++dup_ct;
  if( m_format & FORMAT_VCENTER ) ++dup_ct;
  if( dup_ct != 1 ) {   // exactly one must be picked; when none or multiples are picked, use FORMAT_VCENTER by default
    m_format &= ~(FORMAT_TOP | FORMAT_BOTTOM);
    m_format |= FORMAT_VCENTER;
  }
  if( (m_format & FORMAT_WORDBREAK) && (m_format & FORMAT_LINEWRAP) ) {  // only one of these can be picked; FORMAT_WORDBREAK overrides FORMAT_LINEWRAP
    m_format &= ~FORMAT_LINEWRAP;
  }
}

void TextControl::AdjustMinimumSize()
{
  if (m_set_min_size) {
    Widget::SetMinSize(m_text_lr - m_text_ul);
  }
}

void TextControl::RecomputeTextBounds()
{
  Nebulae::Point text_sz = TextLowerRight() - TextUpperLeft();
  m_text_ul.y = 0; // default value for FORMAT_TOP
  if( m_format & FORMAT_BOTTOM ) {
    m_text_ul.y = GetSize().y - text_sz.y;
  } else if( m_format & FORMAT_VCENTER ) {
    m_text_ul.y = (GetSize().y - text_sz.y) / 2.0;
  }
  m_text_ul.x = 0; // default for FORMAT_LEFT
  if( m_format & FORMAT_RIGHT ) {
    m_text_ul.x = GetSize().x - text_sz.x;
  } else if( m_format & FORMAT_CENTER ) {
    m_text_ul.x = (GetSize().x - text_sz.x) / 2.0;
  }
  m_text_lr = m_text_ul + text_sz;
}
