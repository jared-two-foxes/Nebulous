
#include <Nebulae/Beta/Gui/Control/EditControl.h>

#include <utf8/checked.h>

using Nebulae::Colour;
using Nebulae::CPSize;
using Nebulae::EditControl;
using Nebulae::Flags;
using Nebulae::Font;
using Nebulae::KeyCode;
using Nebulae::ModKey;
using Nebulae::Point;
using Nebulae::WidgetFactory;

namespace 
{
  //struct InRange
  //{
  //  InRange( CPSize value ) : m_value(value) {}
  //  bool operator()( const std::pair< CPSize, CPSize >& p ) const
  //  { return p.first < m_value && m_value < p.second; }
  //  const CPSize m_value;
  //};

  int HeightFromFont( const std::shared_ptr<Font >& font, unsigned int pixel_margin )
  {  
    return font->Height() + 2 * static_cast<int>(pixel_margin);
  }
}

////////////////////////////////////////////////
// GG::Edit
////////////////////////////////////////////////

const int EditControl::PIXEL_MARGIN = 5;


EditControl::EditControl( const WidgetFactory& factory, int x, int y, int w, const std::string& str, const std::shared_ptr<Font>& font, Colour color,
                          Colour text_color, Colour interior, uint32 flags ) 
: TextControl(factory, x, y, w, HeightFromFont(font, PIXEL_MARGIN), str, font, text_color, FORMAT_LEFT | FORMAT_IGNORETAGS, flags),
  m_cursor_pos(CP0, CP0),
  m_first_char_shown(CP0),
  m_int_color(interior),
  m_hilite_color(CLR_SHADOW),
  m_sel_text_color(CLR_WHITE),
  m_recently_edited(false)
{
  SetColour(color);
}

Point 
EditControl::MinUsableSize() const
{ return Point(4 * PIXEL_MARGIN, HeightFromFont(GetFont(), PIXEL_MARGIN)); }

Point 
EditControl::ClientUpperLeft() const
{ return GetUpperLeft() + Point(PIXEL_MARGIN, PIXEL_MARGIN); }

Point 
EditControl::ClientLowerRight() const
{ return GetLowerRight() - Point(PIXEL_MARGIN, PIXEL_MARGIN); }

const std::pair<CPSize, CPSize>& 
EditControl::CursorPosn() const
{ return m_cursor_pos; }

Colour 
EditControl::InteriorColour() const
{ return m_int_color; }

Colour 
EditControl::HiliteColour() const
{ return m_hilite_color; }

Colour 
EditControl::SelectedTextColour() const
{ return m_sel_text_color; }

void 
EditControl::SetColour(Colour c)
{ Control::SetColour(c); }

void 
EditControl::SetInteriorColour(Colour c)
{ m_int_color = c; }

void
EditControl::SetHiliteColour(Colour c)
{ m_hilite_color = c; }

void 
EditControl::SetSelectedTextColour(Colour c)
{ m_sel_text_color = c; }

void
EditControl::SelectAll()
{
  m_cursor_pos.first = Length();
  m_cursor_pos.second = CP0;
  AdjustView();
}

void 
EditControl::SelectRange(CPSize from, CPSize to)
{
  if (from < to)
  {
    m_cursor_pos.first = std::max(CP0, from);
    m_cursor_pos.second = std::min(to, Length());
  } 
  else 
  {
    m_cursor_pos.first = std::min(from, Length());
    m_cursor_pos.second = std::max(CP0, to);
  }
  AdjustView();
}

void
EditControl::SetText(const std::string& str)
{
  TextControl::SetText(str);
  m_cursor_pos.second = m_cursor_pos.first; // eliminate any hiliting

  // make sure the change in text did not make the cursor or view position invalid
  if (str.empty() || GetLineData().empty() || GetLineData()[0].char_data.size() < m_cursor_pos.first) 
  {
    m_first_char_shown = CP0;
    m_cursor_pos = std::make_pair(CP0, CP0);
  }

  m_recently_edited = true;
}

bool 
EditControl::MultiSelected() const
{ return m_cursor_pos.first != m_cursor_pos.second; }

CPSize 
EditControl::FirstCharShown() const
{ return m_first_char_shown; }

bool 
EditControl::RecentlyEdited() const
{ return m_recently_edited; }

CPSize 
EditControl::CharIndexOf(int x) const
{
  CPSize retval;
  int first_char_offset = FirstCharOffset();
  for (retval = CP0; retval < Length(); ++retval) 
  {
    int curr_extent;
    if (x + first_char_offset <= (curr_extent = GetLineData()[0].char_data[Value(retval)].extent)) // the point falls within the character at index retval
    { 
      int prev_extent = retval ? GetLineData()[0].char_data[Value(retval - 1)].extent : 0;
      int half_way = (prev_extent + curr_extent) / 2;
      if (half_way <= x + first_char_offset) // if the point is more than halfway across the character, put the cursor *after* the character
      {
        ++retval;
      }
      break;
    }
  }
  return retval;
}

int 
EditControl::FirstCharOffset() const
{ return (m_first_char_shown ? GetLineData()[0].char_data[Value(m_first_char_shown - 1)].extent : 0); }

int 
EditControl::ScreenPosOfChar(CPSize idx) const
{
    int first_char_offset = FirstCharOffset();
    return GetUpperLeft().x + PIXEL_MARGIN + ((idx ? GetLineData()[0].char_data[Value(idx - 1)].extent : 0) - first_char_offset);
}

CPSize 
EditControl::LastVisibleChar() const
{
  int first_char_offset = FirstCharOffset();
  CPSize retval = m_first_char_shown;
  for ( ; retval < Length(); ++retval) {
    if (GetSize().x - 2 * PIXEL_MARGIN <= (retval ? GetLineData()[0].char_data[Value(retval - 1)].extent : 0) - first_char_offset)
    {
      break;
    }
  }
  return retval;
}

void 
EditControl::TouchDown(const Point& pt, Flags<ModKey> mod_keys)
{
  if( !IsDisabled() ) {
    int    click_xpos  = ScreenToWindow(pt).x - PIXEL_MARGIN; // x coord of click within text space
    CPSize idx         = CharIndexOf(click_xpos);
    m_cursor_pos.first = m_cursor_pos.second = idx;
  }
}

void EditControl::TouchDrag(const Point& pt, const Point& move, Flags<ModKey> mod_keys)
{
  if (!IsDisabled()) {
    int xpos = ScreenToWindow(pt).x - PIXEL_MARGIN; // x coord for mouse position within text space
    CPSize idx = CharIndexOf(xpos);
    // when a single-click drag occurs, move m_cursor_pos.second to where the mouse is, which selects a range of characters
    m_cursor_pos.second = idx;
    if (xpos < 0 || GetSize().x - 2 * PIXEL_MARGIN < xpos) // if we're dragging past the currently visible text
      AdjustView();
  }
}

void EditControl::KeyPress(KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys)
{
  if (!IsDisabled()) {
    bool shift_down = mod_keys & (MOD_KEY_LSHIFT | MOD_KEY_RSHIFT);
    bool emit_signal = false;

    switch (key) 
    {
    case VKC_HOME:
      m_first_char_shown = CP0;
      if (shift_down)
          m_cursor_pos.second = CP0;
      else
          m_cursor_pos.second = m_cursor_pos.first = CP0;
        break;
    case VKC_LEFT:
        if (MultiSelected() && !shift_down) {
            m_cursor_pos.second = m_cursor_pos.first = std::min(m_cursor_pos.first, m_cursor_pos.second);
        } else if (0 < m_cursor_pos.second) {
            --m_cursor_pos.second;
            int32 extent = GetLineData()[0].char_data[Value(m_cursor_pos.second)].extent;
            while (0 < m_cursor_pos.second && extent == GetLineData()[0].char_data[Value(m_cursor_pos.second - 1)].extent)
                --m_cursor_pos.second;
            if (!shift_down)
                m_cursor_pos.first = m_cursor_pos.second;
        }
        AdjustView();
        break;
    case VKC_RIGHT:
        if (MultiSelected() && !shift_down) {
            m_cursor_pos.second = m_cursor_pos.first = std::max(m_cursor_pos.first, m_cursor_pos.second);
        } else if (m_cursor_pos.second < Length()) {
            int32 extent = GetLineData()[0].char_data[Value(m_cursor_pos.second)].extent;
            while (m_cursor_pos.second < Length() && extent == GetLineData()[0].char_data[Value(m_cursor_pos.second)].extent)
                ++m_cursor_pos.second;
            if (!shift_down)
                m_cursor_pos.first = m_cursor_pos.second;
        }
        AdjustView();
        break;
    case VKC_END:
        if (shift_down)
            m_cursor_pos.second = Length();
        else
            m_cursor_pos.second = m_cursor_pos.first = Length();
        AdjustView();
        break;
    case VKC_BACKSPACE:
        if (MultiSelected()) {
            ClearSelected();
            emit_signal = true;
        } else if (0 < m_cursor_pos.first) {
            m_cursor_pos.second = --m_cursor_pos.first;
            Erase(0, m_cursor_pos.first);
            emit_signal = true;
        }
        AdjustView();
        break;
    case VKC_DELETE:
        if (MultiSelected()) {
            ClearSelected();
            emit_signal = true;
        } else if (m_cursor_pos.first < Length()) {
            Erase(m_cursor_pos.first);
            emit_signal = true;
        }
        AdjustView();
        break;
    case VKC_RETURN:
    case VKC_KP_ENTER:
        FocusUpdateSignal(GetText());
        TextControl::KeyPress(key, key_code_point, mod_keys);
        m_recently_edited = false;
        break;
    default:
        std::string translated_code_point;
        GetTranslatedCodePoint( key, key_code_point, mod_keys, translated_code_point );
        if( !translated_code_point.empty() && !(mod_keys & (MOD_KEY_CTRL | MOD_KEY_ALT | MOD_KEY_META)) ) 
        {
          if( MultiSelected() )
          {
            ClearSelected();
          }
          Insert( 0, m_cursor_pos.first, translated_code_point );
          m_cursor_pos.second = ++m_cursor_pos.first;
          emit_signal = true;
          if( LastVisibleChar() <= m_cursor_pos.first )
          {
            AdjustView();
          }
        } 
        else 
        {
          TextControl::KeyPress( key, key_code_point, mod_keys );
        }
        break;
    }
    
    if( emit_signal )
    {
      EditedSignal( GetText() );
    }
  } 
  else 
  {
    TextControl::KeyPress(key, key_code_point, mod_keys);
  }
}

void EditControl::GainingFocus()
{ m_recently_edited = false; }

void EditControl::LosingFocus()
{
  if (m_recently_edited)
    FocusUpdateSignal(GetText());
}

void EditControl::ClearSelected()
{
    CPSize low = std::min(m_cursor_pos.first, m_cursor_pos.second);
    CPSize high = std::max(m_cursor_pos.first, m_cursor_pos.second);
    if (m_cursor_pos.first < m_cursor_pos.second)
        m_cursor_pos.second = m_cursor_pos.first;
    else
        m_cursor_pos.first = m_cursor_pos.second;
    Erase(0, low, high - low);

    // make sure deletion has not left m_first_char_shown in an out-of-bounds position
    if (GetLineData()[0].char_data.empty())
        m_first_char_shown = CP0;
    else if (GetLineData()[0].char_data.size() <= m_first_char_shown)
        m_first_char_shown = CodePointIndexOf(0, INVALID_CP_SIZE, GetLineData());
}

void EditControl::AdjustView()
{
    int text_space = GetSize().x - 2 * PIXEL_MARGIN;
    int first_char_offset = FirstCharOffset();
    if (m_cursor_pos.second < m_first_char_shown) { // if the caret is at a place left of the current visible area
        if (m_first_char_shown - m_cursor_pos.second < 5) // if the caret is less than five characters before m_first_char_shown
            m_first_char_shown = (0 <= m_first_char_shown - 5) ? m_first_char_shown - 5 : CP0; // try to move the caret by five characters
        else // if the caret is more than five characters before m_first_char_shown, just move straight to that spot
            m_first_char_shown = m_cursor_pos.second;
    } else if (Length() && text_space <= (m_cursor_pos.second ? GetLineData()[0].char_data[Value(m_cursor_pos.second - 1)].extent : 0) - first_char_offset) { // if the caret is moving to a place right of the current visible area
        // try to move the text by five characters, or to the end if caret is at a location before the end - 5th character
        CPSize last_idx_to_use = (m_cursor_pos.second + 5 <= Length() - 1) ? m_cursor_pos.second + 5 : Length() - 1;
        const std::vector<Font::LineData::CharData>& char_data = GetLineData()[0].char_data;
        // number of pixels that the caret position overruns the right side of text area
        int pixels_to_move = (char_data[Value(last_idx_to_use)].extent - first_char_offset) - text_space;
        if (last_idx_to_use == Length() - 1) // if the caret is at the very end of the string, add the length of some spaces
            pixels_to_move += static_cast<int>(Value(m_cursor_pos.second + 5 - Length() - 1)) * GetFont()->SpaceWidth();
        CPSize move_to = m_first_char_shown;
        while (move_to < char_data.size() &&
               char_data[Value(move_to)].extent - first_char_offset < pixels_to_move) {
            ++move_to;
        }
        m_first_char_shown = move_to;
    }
}


////////////////////////////////////////////////////////////
// Free Functions
////////////////////////////////////////////////////////////
void Nebulae::GetTranslatedCodePoint( KeyCode key,
                                      uint32 key_code_point,
                                      Flags<ModKey > mod_keys,
                                      std::string& translated_code_point )
{
  // only process it if it's a valid code point or a known printable
  // key, and no significant modifiers are in use
  if( key_code_point )
  {
    try 
    {
      boost::uint32_t chars[] = { key_code_point };
      utf8::utf32to8( chars, chars + 1, std::back_inserter(translated_code_point) );
    } 
    catch( const utf8::invalid_code_point& ) 
    {
      translated_code_point.clear();
    }
  } 
  else 
  {
    Keyboard::KeypadKeyToPrintable( key, mod_keys );
    if( VKC_DELETE <= key || !isprint(key) )
    {
      translated_code_point.clear();
    }
    else
    {
      translated_code_point = key;
    }
  }
}
