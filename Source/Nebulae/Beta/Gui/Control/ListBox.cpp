/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANint WARRANTY; without even the implied warranty of
   MERCHANTABILITint or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */

#include "ListBox.h"

// #include <GG/DrawUtil.h>
// #include <GG/Layout.h>
// #include <GG/Scroll.h>
// #include <GG/StaticGraphic.h>


#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <Nebulae/Beta/Gui/WidgetEvent.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>


#include <boost/cast.hpp>
#include <boost/assign/list_of.hpp>

#include <cmath>
#include <numeric>


using namespace Nebulae;

namespace {
  struct ListSignalEcho
  {
    ListSignalEcho(const ListBox& lb, const std::string& name) :
      m_LB(lb),
      m_name(name)
      {}
    void operator()()
      { std::cerr << "GG SIGNAL : " << m_name << "()\n"; }
    void operator()(const ListBox::SelectionSet& sels)
      {
        std::cerr << "GG SIGNAL : " << m_name
                  << "(sels=[ ";
        for (ListBox::SelectionSet::const_iterator it = sels.begin();
             it != sels.end();
             ++it) {
          std::cerr << RowIndex(*it) << ' ';
        }
        std::cerr << "])\n";
      }
    void operator()(ListBox::const_iterator it)
        { std::cerr << "GG SIGNAL : " << m_name << "(row=" << RowIndex(it) << ")\n"; }
    void operator()(ListBox::const_iterator it, const Nebulae::Point& pt)
        {
          std::cerr << "GG SIGNAL : " << m_name
                    << "(row=" << RowIndex(it) << /*" pt=" << pt <<*/ ")\n";
        }
    std::size_t RowIndex(ListBox::const_iterator it)
        { return std::distance(m_LB.begin(), it); }
    const ListBox& m_LB;
    std::string m_name;
  };

  const int SCROLL_WIDTH = 14;
  const int DEFAULT_ROW_WIDTH(50);
  const int DEFAULT_ROW_HEIGHT(22);

  class RowSorter // used to sort rows by a certain column (which may contain some empty cells)
  {
  public:
    RowSorter(const boost::function<bool (const ListBox::Row&, const ListBox::Row&, std::size_t)>& cmp,
              std::size_t col, bool invert) :
      m_cmp(cmp), m_sort_col(col), m_invert(invert) {}

    bool operator()(const ListBox::Row* l, const ListBox::Row* r)
    {
      bool retval = m_cmp(*l, *r, m_sort_col);
      return m_invert ? !retval : retval;
    }

  private:
    boost::function<bool (const ListBox::Row&, const ListBox::Row&, std::size_t)> m_cmp;
    std::size_t m_sort_col;
    bool m_invert;
  };

  bool LessThan(ListBox::const_iterator lhs, ListBox::const_iterator rhs, ListBox::const_iterator end)
  { return ListBox::RowPtrIteratorLess<std::list<ListBox::Row*> >::LessThan(lhs, rhs, end); }

  bool LessThanEqual(ListBox::const_iterator lhs, ListBox::const_iterator rhs, ListBox::const_iterator end)
  {
    return lhs == rhs ||
      ListBox::RowPtrIteratorLess<std::list<ListBox::Row*> >::LessThan(lhs, rhs, end);
  }

  void ResetIfEqual(ListBox::iterator& val, ListBox::iterator other, ListBox::iterator end)
  {
    if( val == other ) {
      val = end;
    }
  }

  ListBox::Row* SafeDeref(const ListBox::iterator& it, const ListBox::iterator& end)
  { return it == end ? 0 : *it; }

  struct ScopedSet
  {
    ScopedSet(ListBox::iterator*& var, ListBox::iterator* value) :
      m_var(var = value)
      {}
    ~ScopedSet()
      { m_var = 0; }
    ListBox::iterator*& m_var;
  };

  Alignment AlignmentFromStyle(uint32 style)
  {
    Alignment retval = ALIGN_NONE;
    if (style & LIST_LEFT)
      retval = ALIGN_LEFT;
    if (style & LIST_CENTER)
      retval = ALIGN_CENTER;
    if (style & LIST_RIGHT)
      retval = ALIGN_RIGHT;
    return retval;
  }
}

///////////////////////////////////////
// ListBoxStyle
///////////////////////////////////////
// const ListBoxStyle GG::LIST_NONE            (0);
// const ListBoxStyle GG::LIST_VCENTER         (1 << 0);
// const ListBoxStyle GG::LIST_TOP             (1 << 1);
// const ListBoxStyle GG::LIST_BOTTOM          (1 << 2);
// const ListBoxStyle GG::LIST_CENTER          (1 << 3);
// const ListBoxStyle GG::LIST_LEFT            (1 << 4);
// const ListBoxStyle GG::LIST_RIGHT           (1 << 5);
// const ListBoxStyle GG::LIST_NOSORT          (1 << 6);
// const ListBoxStyle GG::LIST_SORTDESCENDING  (1 << 7);
// const ListBoxStyle GG::LIST_NOSEL           (1 << 8);
// const ListBoxStyle GG::LIST_SINGLESEL       (1 << 9);
// const ListBoxStyle GG::LIST_QUICKSEL        (1 << 10);
// const ListBoxStyle GG::LIST_USERDELETE      (1 << 11);
// const ListBoxStyle GG::LIST_BROWSEUPDATES   (1 << 12);
//
//GG_FLAGSPEC_IMPL(ListBoxStyle);
//
//namespace {
//  bool RegisterListBoxStyles()
//  {
//    FlagSpec<ListBoxStyle>& spec = FlagSpec<ListBoxStyle>::instance();
//    spec.insert(LIST_NONE, "LIST_NONE", true);
//    spec.insert(LIST_VCENTER, "LIST_VCENTER", true);
//    spec.insert(LIST_TOP, "LIST_TOP", true);
//    spec.insert(LIST_BOTTOM, "LIST_BOTTOM", true);
//    spec.insert(LIST_CENTER, "LIST_CENTER", true);
//    spec.insert(LIST_LEFT, "LIST_LEFT", true);
//    spec.insert(LIST_RIGHT, "LIST_RIGHT", true);
//    spec.insert(LIST_NOSORT, "LIST_NOSORT", true);
//    spec.insert(LIST_SORTDESCENDING, "LIST_SORTDESCENDING", true);
//    spec.insert(LIST_NOSEL, "LIST_NOSEL", true);
//    spec.insert(LIST_SINGLESEL, "LIST_SINGLESEL", true);
//    spec.insert(LIST_QUICKSEL, "LIST_QUICKSEL", true);
//    spec.insert(LIST_USERDELETE, "LIST_USERDELETE", true);
//    spec.insert(LIST_BROWSEUPDATES, "LIST_BROWSEUPDATES", true);
//    return true;
//  }
//  bool dummy = RegisterListBoxStyles();
//}


////////////////////////////////////////////////
// GG::ListBox::Row::DeferAdjustLayout
////////////////////////////////////////////////
ListBox::Row::DeferAdjustLayout::DeferAdjustLayout(Row* row) :
    m_row(row)
{ m_row->m_ignore_adjust_layout = true; }

ListBox::Row::DeferAdjustLayout::~DeferAdjustLayout()
{
  m_row->m_ignore_adjust_layout = false;
  m_row->AdjustLayout();
}


////////////////////////////////////////////////
// GG::ListBox::Row
////////////////////////////////////////////////
ListBox::Row::Row(const WidgetFactory& factory) :
    Control(factory, 0, 0, DEFAULT_ROW_WIDTH, DEFAULT_ROW_HEIGHT),
    m_row_alignment(ALIGN_VCENTER),
    m_margin(2),
    m_ignore_adjust_layout(false)
{}

ListBox::Row::Row(const WidgetFactory& factory, int w, int h, const std::string& drag_drop_data_type,
                  Alignment align, unsigned int margin) : 
  Control(factory, 0, 0, w, h),
  m_row_alignment(align),
  m_margin(margin),
  m_ignore_adjust_layout(false)
{ SetDragDropDataType(drag_drop_data_type); }

ListBox::Row::~Row()
{}

std::string ListBox::Row::SortKey(std::size_t column) const
{
  //const TextControl* text_control = dynamic_cast<const TextControl*>(at(column));
  //return text_control ? text_control->Text() : "";
  return "";
}

std::size_t ListBox::Row::size() const
{ return m_cells.size(); }

bool ListBox::Row::empty() const
{ return m_cells.empty(); }

Control* ListBox::Row::operator[](std::size_t n) const
{ return m_cells[n]; }

Control* ListBox::Row::at(std::size_t n) const
{ return m_cells.at(n); }

Alignment ListBox::Row::RowAlignment() const
{ return m_row_alignment; }

Alignment ListBox::Row::ColAlignment(std::size_t n) const
{ return m_col_alignments[n]; }

int ListBox::Row::ColWidth(std::size_t n) const
{ return m_col_widths[n]; }

unsigned int ListBox::Row::Margin() const
{ return m_margin; }

Control* ListBox::Row::CreateControl(const std::string& str, const std::shared_ptr<Font>& font, Colour color) const
{ return GetWidgetFactory().CreateTextControl(0, 0, str, font, color); }

Control* ListBox::Row::CreateControl(const Texture& st) const
{ return NULL; /*return new StaticGraphic(0, 0, st.Width(), st.Height(), st, GRAPHIC_SHRINKFIT);*/ }

//void ListBox::Row::Render()
//{}

void ListBox::Row::push_back(Control* c)
{
  m_cells.push_back(c);
  m_col_widths.push_back(int(5));
  m_col_alignments.push_back(ALIGN_NONE);
  if (1 < m_cells.size()) {
    m_col_widths.back() = m_col_widths[m_cells.size() - 1];
  }
  AdjustLayout();
}

void ListBox::Row::push_back(const std::string& str, const std::shared_ptr<Font>& font,
                             Colour color)
{ push_back(CreateControl(str, font, color)); }

void ListBox::Row::push_back(const std::string& str, const std::string& font_filename, unsigned int pts,
                             Colour color)
{ /*push_back(CreateControl(str, GUI::GetGUI()->GetFont(font_filename, pts), color));*/ }

void ListBox::Row::push_back(const Texture& st)
{ push_back(CreateControl(st)); }

void ListBox::Row::clear()
{
  m_cells.clear();
  //RemoveLayout();
  DeleteChildren();
}

void ListBox::Row::resize(std::size_t n)
{
  if (n == m_cells.size())
    return;

  std::size_t old_size = m_cells.size();
  for (std::size_t i = n; i < old_size; ++i) {
    delete m_cells[i];
  }
  m_cells.resize(n);
  m_col_widths.resize(n);
  m_col_alignments.resize(n);
  for (std::size_t i = old_size; i < n; ++i) {
    m_col_widths[i] = old_size ? m_col_widths[old_size - 1] : int(5); // assign new cells reasonable default widths
    m_col_alignments[i] = ALIGN_NONE;
  }
  AdjustLayout();
}

void ListBox::Row::SetCell(std::size_t n, Control* c)
{
  NE_ASSERT(c != m_cells[n], "")();
  delete m_cells[n];
  m_cells[n] = c;
  AdjustLayout();
}

Control* ListBox::Row::RemoveCell(std::size_t n)
{
  Control* retval = m_cells[n];
  m_cells[n] = 0;
  AdjustLayout();
  return retval;
}

void ListBox::Row::SetRowAlignment(Alignment align)
{
  if (align == m_row_alignment)
    return;

  m_row_alignment = align;
  AdjustLayout();
}

void ListBox::Row::SetColAlignment(std::size_t n, Alignment align)
{
  if (align == m_col_alignments[n])
    return;

  m_col_alignments[n] = align;
  AdjustLayout();
}

void ListBox::Row::SetColWidth(std::size_t n, int width)
{
  if (width == m_col_widths[n])
    return;

  m_col_widths[n] = width;
  AdjustLayout();
}

void ListBox::Row::SetColAlignments(const std::vector<Alignment>& aligns)
{
  if (aligns == m_col_alignments)
    return;

  m_col_alignments = aligns;
  AdjustLayout();
}

void ListBox::Row::SetColWidths(const std::vector<int>& widths)
{
  if (widths == m_col_widths)
    return;

  m_col_widths = widths;
  AdjustLayout();
}

void ListBox::Row::SetMargin(unsigned int margin)
{
  if (margin == m_margin)
    return;

  m_margin = margin;
  AdjustLayout();
}

void ListBox::Row::AdjustLayout(bool adjust_for_push_back/* = false*/)
{
  if (m_ignore_adjust_layout)
    return;

  //RemoveLayout();
  DetachChildren();

  bool nonempty_cell_found = false;
  for (std::size_t i = 0; i < m_cells.size(); ++i) {
    if (m_cells[i]) {
      nonempty_cell_found = true;
      break;
    }
  }

  if (!nonempty_cell_found)
    return;

  //SetLayout(new Layout(X0, Y0, Width(), Height(), 1, m_cells.size(), m_margin, m_margin));
  //Layout* layout = GetLayout();
  //for (std::size_t i = 0; i < m_cells.size(); ++i) {
  //  layout->SetMinimumColumnWidth(i, m_col_widths[i]);
  //  if (m_cells[i]) {
  //    layout->Add(m_cells[i], 0, i, m_row_alignment | m_col_alignments[i]);
  //  }
  //}
}

////////////////////////////////////////////////
// GG::ListBox
////////////////////////////////////////////////
// static(s)
const unsigned int ListBox::BORDER_THICK = 2;

ListBox::ListBox(const WidgetFactory& factory, int x, int y, int w, int h, Colour color, Colour interior,
                 uint32 flags) :
  Control(factory, x, y, w, h, flags),
  m_rows(),
  m_caret(m_rows.end()),
  m_selections(RowPtrIteratorLess<std::list<Row*> >(&m_rows)),
  m_old_sel_row(m_rows.end()),
  m_old_sel_row_selected(false),
  m_old_rdown_row(m_rows.end()),
  m_lclick_row(m_rows.end()),
  m_rclick_row(m_rows.end()),
  m_last_row_browsed(m_rows.end()),
  m_first_row_shown(m_rows.end()),
  m_first_col_shown(0),
  m_cell_margin(2),
  m_int_color(interior),
  m_hilite_color(CLR_SHADOW),
  m_style(LIST_NONE),
  m_header_row(new Row(factory, DEFAULT_ROW_WIDTH, DEFAULT_ROW_HEIGHT, "")),
  m_keep_col_widths(false),
  m_clip_cells(false),
  m_sort_col(0),
  m_sort_cmp(DefaultRowCmp<Row>()),
  m_auto_scroll_during_drag_drops(true),
  m_auto_scroll_margin(8),
  m_auto_scrolling_up(false),
  m_auto_scrolling_down(false),
  m_auto_scrolling_left(false),
  m_auto_scrolling_right(false),
  m_iterator_being_erased(0)
{
  Control::SetColour(color);
  ValidateStyle();
  //SetChildClippingMode(ClipToClient);

  InstallEventFilter(this);

  // if (INSTRUMENT_ALL_SIGNALS) {
  //   Connect(ClearedSignal, ListSignalEcho(*this, "ListBox::ClearedSignal"));
  //   Connect(InsertedSignal, ListSignalEcho(*this, "ListBox::InsertedSignal"));
  //   Connect(SelChangedSignal, ListSignalEcho(*this, "ListBox::SelChangedSignal"));
  //   Connect(DroppedSignal, ListSignalEcho(*this, "ListBox::DroppedSignal"));
  //   Connect(DropAcceptableSignal, ListSignalEcho(*this, "ListBox::DropAcceptableSignal"));
  //   Connect(LeftClickedSignal, ListSignalEcho(*this, "ListBox::LeftClickedSignal"));
  //   Connect(RightClickedSignal, ListSignalEcho(*this, "ListBox::RightClickedSignal"));
  //   Connect(DoubleClickedSignal, ListSignalEcho(*this, "ListBox::DoubleClickedSignal"));
  //   Connect(ErasedSignal, ListSignalEcho(*this, "ListBox::ErasedSignal"));
  //   Connect(BrowsedSignal, ListSignalEcho(*this, "ListBox::BrowsedSignal"));
  // }
}

ListBox::~ListBox()
{ delete m_header_row; }

void ListBox::DropsAcceptable(DropsAcceptableIter first, DropsAcceptableIter last, const Point& pt) const
{
  for (std::map<const Widget*, bool>::iterator it = first; it != last; ++it) {
    it->second = false;
    const Row* row = dynamic_cast<const Row*>(it->first);
    if (row &&
      (m_allowed_drop_types.find("") != m_allowed_drop_types.end() ||
       m_allowed_drop_types.find(row->GetDragDropDataType()) != m_allowed_drop_types.end())) {
      iterator insertion_it = RowUnderPt(pt);

      DropAcceptableSignal(insertion_it);
      it->second = true;
    }
  }
}

Nebulae::Point ListBox::GetMinUsableSize() const
{
  return Nebulae::Point(int(5 * SCROLL_WIDTH + 2 * BORDER_THICK),
            int(5 * SCROLL_WIDTH + 2 * BORDER_THICK));
}

Nebulae::Point ListBox::GetClientUpperLeft() const
{
  return GetUpperLeft() +
    Nebulae::Point(int(BORDER_THICK), static_cast<int>(BORDER_THICK) + (m_header_row->empty() ? 0 : m_header_row->GetHeight()));
}

Nebulae::Point ListBox::GetClientLowerRight() const
{ return GetLowerRight() - Nebulae::Point(static_cast<int>(BORDER_THICK) + RightMargin(), static_cast<int>(BORDER_THICK) + BottomMargin()); }

bool ListBox::Empty() const
{ return m_rows.empty(); }

ListBox::const_iterator ListBox::begin() const
{ return m_rows.begin(); }

ListBox::const_iterator ListBox::end() const
{ return m_rows.end(); }

ListBox::const_reverse_iterator ListBox::rbegin() const
{ return m_rows.rbegin(); }

ListBox::const_reverse_iterator ListBox::rend() const
{ return m_rows.rend(); }

const ListBox::Row& ListBox::GetRow(std::size_t n) const
{
  NE_ASSERT(n < m_rows.size(), "")();
  return **boost::next(m_rows.begin(), n);
}

ListBox::iterator ListBox::Caret() const
{ return m_caret; }

const ListBox::SelectionSet& ListBox::Selections() const
{ return m_selections; }

bool ListBox::Selected(iterator it) const
{ return m_selections.find(it) != m_selections.end(); }

Colour ListBox::InteriorColour() const
{ return m_int_color; }

Colour ListBox::HiliteColour() const
{ return m_hilite_color; }

uint32 ListBox::Style() const
{ return m_style; }

const ListBox::Row& ListBox::ColHeaders() const
{ return *m_header_row; }

ListBox::iterator ListBox::FirstRowShown() const
{ return m_first_row_shown; }

std::size_t ListBox::FirstColShown() const
{ return m_first_col_shown; }

ListBox::iterator ListBox::LastVisibleRow() const
{
  int visible_pixels = GetClientSize().y;
  int acc(0);
  iterator it = m_first_row_shown;
  for (; it != m_rows.end(); ) {
    acc += (*it)->GetHeight();
    iterator next_it = it;
    ++next_it;
    if (visible_pixels <= acc || next_it == m_rows.end())
      break;
    it = next_it;
  }
  return it;
}

std::size_t ListBox::LastVisibleCol() const
{
  int visible_pixels = GetClientSize().x;
  int acc(0);
  std::size_t i = m_first_col_shown;
  for (; i < m_col_widths.size(); ++i) {
    acc += m_col_widths[i];
    if (visible_pixels <= acc)
      break;
  }
  if (m_col_widths.size() <= i)
    i = m_col_widths.size() - 1;
  return i;
}

std::size_t ListBox::NumRows() const
{ return m_rows.size(); }

std::size_t ListBox::NumCols() const
{ return m_col_widths.size(); }

bool ListBox::KeepColWidths() const
{ return m_keep_col_widths; }

std::size_t ListBox::SortCol() const
{ return (m_sort_col < m_col_widths.size() ? m_sort_col : 0); }

int ListBox::ColWidth(std::size_t n) const
{ return m_col_widths[n]; }

Alignment ListBox::ColAlignment(std::size_t n) const
{ return m_col_alignments[n]; }

Alignment ListBox::RowAlignment(iterator it) const
{ return (*it)->RowAlignment(); }

const std::set<std::string>& ListBox::AllowedDropTypes() const
{ return m_allowed_drop_types; }

bool ListBox::AutoScrollDuringDragDrops() const
{ return m_auto_scroll_during_drag_drops; }

unsigned int ListBox::AutoScrollMargin() const
{ return m_auto_scroll_margin; }

void ListBox::StartingChildDragDrop(const Widget* wnd, const Point& offset)
{
  if (m_selections.empty())
    return;

  int vertical_offset = offset.y;
  iterator wnd_it = std::find(m_rows.begin(), m_rows.end(), wnd);
  NE_ASSERT(wnd_it != m_rows.end(), "Could not find Widget in list")();
  SelectionSet::iterator wnd_sel_it = m_selections.find(wnd_it);
  NE_ASSERT(wnd_sel_it != m_selections.end(), "Could not find Widget in Selection")();
  for (SelectionSet::iterator sel_it = m_selections.begin(); sel_it != wnd_sel_it; ++sel_it) {
    vertical_offset += (**sel_it)->GetHeight();
  }    
  for (SelectionSet::iterator sel_it = m_selections.begin(); sel_it != m_selections.end(); ++sel_it) {
    Widget* row_wnd = **sel_it;
    if (row_wnd != wnd) {
      //GUI::GetGUI()->RegisterDragDropWnd(row_wnd, Point(offset.x, vertical_offset), this);
      vertical_offset -= row_wnd->GetHeight();
    } else {
      vertical_offset -= wnd->GetHeight();
    }
  }
}

void ListBox::AcceptDrops(const std::vector<Widget*>& wnds, const Point& pt)
{
  // TODO: Pull the call to RowUnderPoint() out and reuse the value in each loop iteration.
  for (std::vector<Widget*>::const_iterator it = wnds.begin(); it != wnds.end(); ++it) {
    Row* row = boost::polymorphic_downcast<Row*>(*it);
    iterator insertion_it = RowUnderPt(pt);
    Insert(row, insertion_it, true);
  }
}

void ListBox::ChildrenDraggedAway(const std::vector<Widget*>& wnds, const Widget* destination)
{
  if (!MatchesOrContains(this, destination)) {
    for (std::vector<Widget*>::const_iterator it = wnds.begin(); it != wnds.end(); ++it) {
      Row* row = boost::polymorphic_downcast<Row*>(*it);
      iterator row_it = std::find(m_rows.begin(), m_rows.end(), row);
      NE_ASSERT(row_it != m_rows.end(), "Could not find row in rows.")();
      Erase(row_it, false, true);
    }
  }
}

void ListBox::SizeMove(const Point& ul, const Point& lr)
{
  Widget::SizeMove(ul, lr);
  if (!m_header_row->empty())
    NormalizeRow(m_header_row);
}

void ListBox::SetDisabled(bool b)
{
  Control::SetDisabled(b);
}

void ListBox::SetColour(const Colour& c)
{
  Control::SetColour(c);
}

ListBox::iterator ListBox::Insert(Row* row, iterator it)
{ return Insert(row, it, false); }

ListBox::iterator ListBox::Insert(Row* row)
{ return Insert(row, m_rows.end(), false); }

ListBox::Row* ListBox::Erase(iterator it)
{ return Erase(it, false, false); }

void ListBox::Clear()
{
  m_rows.clear();
  m_caret = m_rows.end();
  DetachChild(m_header_row);
  DeleteChildren();
  AttachChild(m_header_row);
  m_first_row_shown = m_rows.end();
  m_first_col_shown = 0;
  m_selections.clear();
  m_old_sel_row = m_rows.end();
  m_lclick_row = m_rows.end();

  if (!m_keep_col_widths) { // remove column widths and alignments, if needed
    m_col_widths.clear();
    m_col_alignments.clear();
  }

  if (m_iterator_being_erased)
    *m_iterator_being_erased = m_rows.end();
}

void ListBox::SelectRow(iterator it)
{
  if (it != m_rows.end() && !(m_style & LIST_NOSEL) && m_selections.find(it) == m_selections.end()) {
    if (m_style & LIST_SINGLESEL)
      m_selections.clear();
    m_selections.insert(it);
  }
}

void ListBox::DeselectRow(iterator it)
{
  if (m_selections.find(it) != m_selections.end())
    m_selections.erase(it);
}

void ListBox::SelectAll()
{
  if (m_selections.size() < m_rows.size()) {
    for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
      m_selections.insert(it);
    }
  }
}

void ListBox::DeselectAll()
{
  if (!m_selections.empty()) {
    m_selections.clear();
    m_caret = m_rows.end();
  }
}

ListBox::iterator ListBox::begin()
{ return m_rows.begin(); }

ListBox::iterator ListBox::end()
{ return m_rows.end(); }

ListBox::reverse_iterator ListBox::rbegin()
{ return m_rows.rbegin(); }

ListBox::reverse_iterator ListBox::rend()
{ return m_rows.rend(); }

ListBox::Row& ListBox::GetRow(std::size_t n)
{
  NE_ASSERT(n < m_rows.size(), "index is out of bounds")( n, m_rows.size() );
  return **boost::next(m_rows.begin(), n);
}

void ListBox::SetSelections(const SelectionSet& s)
{ m_selections = s; }

void ListBox::SetCaret(iterator it)
{ m_caret = it; }

void ListBox::BringRowIntoView(iterator it)
{
  if (it != m_rows.end()) {
    if (LessThan(it, m_first_row_shown, m_rows.end())) {
      m_first_row_shown = it;
    } else if (LessThanEqual(LastVisibleRow(), it, m_rows.end())) {
      // Find the row that preceeds the target row by about ClientSize().y
      // pixels, and make it the first row shown.
      m_first_row_shown = FirstRowShownWhenBottomIs(it, GetClientHeight());
    }
  }
}

void ListBox::SetInteriorColour(Colour c)
{ m_int_color = c; }

void ListBox::SetHiliteColour(Colour c)
{ m_hilite_color = c; }

void ListBox::SetStyle(uint32 s)
{
  uint32 old_style = m_style;
  m_style = s;
  ValidateStyle();

  // if we're going from an unsorted style to a sorted one, do the sorting now
  if (old_style & LIST_NOSORT) {
    if (!(m_style & LIST_NOSORT))
      Resort();
  // if we're changing the sorting order of a sorted list, reverse the contents
  } else if ((old_style & LIST_SORTDESCENDING) != (m_style & LIST_SORTDESCENDING)) {
    Resort();
  }
}

void ListBox::SetColHeaders(Row* r)
{
  int client_height = GetClientHeight();
  delete m_header_row;
  if (r) {
    m_header_row = r;
    // if this column header is being added to an empty listbox, the listbox takes on some of the
    // attributes of the header, similar to the insertion of a row into an empty listbox; see Insert()
    if (m_rows.empty() && m_col_widths.empty()) {
      m_col_widths.resize(m_header_row->size(),
                          (GetClientSize().x - SCROLL_WIDTH) / static_cast<int>(m_header_row->size()));
      // put the remainder in the last column, so the total width == ClientSize().x - SCROLL_WIDTH
      m_col_widths.back() += (GetClientSize().x - SCROLL_WIDTH) % static_cast<int>(m_header_row->size());
      m_col_alignments.resize(m_header_row->size(), AlignmentFromStyle(m_style));
    }
    NormalizeRow(m_header_row);
    m_header_row->MoveTo(Point(0, -m_header_row->GetHeight()));
    AttachChild(m_header_row);
  } else {
    m_header_row = new Row(GetWidgetFactory(), DEFAULT_ROW_WIDTH, DEFAULT_ROW_HEIGHT, "");
  }
}

void ListBox::RemoveColHeaders()
{ SetColHeaders(0); }

void ListBox::SetNumCols(std::size_t n)
{
  if (m_col_widths.size()) {
    m_col_widths.resize(n);
    m_col_alignments.resize(n);
  } else {
    m_col_widths.resize(n, GetClientSize().x / static_cast<int>(n));
    m_col_widths.back() += GetClientSize().x % static_cast<int>(n);
    Alignment alignment = ALIGN_NONE;
    if (m_style & LIST_LEFT)
      alignment = ALIGN_LEFT;
    if (m_style & LIST_CENTER)
      alignment = ALIGN_CENTER;
    if (m_style & LIST_RIGHT)
      alignment = ALIGN_RIGHT;
    m_col_alignments.resize(n, alignment);
  }
  if (n <= m_sort_col)
    m_sort_col = 0;
  for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
    NormalizeRow(*it);
  }
}

void ListBox::SetColWidth(std::size_t n, int w)
{
  m_col_widths[n] = w;
  for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
    (*it)->SetColWidth(n, w);
  }
}

void ListBox::SetSortCol(std::size_t n)
{
  bool needs_resort = m_sort_col != n && !(m_style & LIST_NOSORT);
  m_sort_col = n;
  if (needs_resort)
    Resort();
}

void ListBox::SetSortCmp(const boost::function<bool (const Row&, const Row&, std::size_t)>& sort_cmp)
{
  m_sort_cmp = sort_cmp;
  if (!(m_style & LIST_NOSORT))
    Resort();
}

void ListBox::LockColWidths()
{ m_keep_col_widths = true; }

void ListBox::UnLockColWidths()
{ m_keep_col_widths = false; }

void ListBox::SetColAlignment(std::size_t n, Alignment align)
{
  m_col_alignments[n] = align;
  for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
    (*it)->SetColAlignment(n, align);
  }
}

void ListBox::SetRowAlignment(iterator it, Alignment align)
{ (*it)->SetRowAlignment(align); }

void ListBox::AllowDropType(const std::string& str)
{ m_allowed_drop_types.insert(str); }

void ListBox::DisallowDropType(const std::string& str)
{ m_allowed_drop_types.erase(str); }

void ListBox::AutoScrollDuringDragDrops(bool auto_scroll)
{ m_auto_scroll_during_drag_drops = auto_scroll; }

void ListBox::SetAutoScrollMargin(unsigned int margin)
{ m_auto_scroll_margin = margin; }

void ListBox::SetAutoScrollInterval(unsigned int interval)
{ assert( 0 ); }

int ListBox::RightMargin() const
{ return 0; }

int ListBox::BottomMargin() const
{ return 0; }

unsigned int ListBox::CellMargin() const
{ return m_cell_margin; }

ListBox::iterator ListBox::RowUnderPt(const Point& pt) const
{
  iterator retval = m_first_row_shown;
  int acc = GetClientUpperLeft().y;
  for ( ; retval != m_rows.end(); ++retval) {
    acc += (*retval)->GetHeight();
    if (pt.y <= acc)
      break;
  }
  return retval;
}

ListBox::iterator ListBox::OldSelRow() const
{ return m_old_sel_row; }

ListBox::iterator ListBox::OldRDownRow() const
{ return m_old_rdown_row; }

ListBox::iterator ListBox::LClickRow() const
{ return m_lclick_row; }

ListBox::iterator ListBox::RClickRow() const
{ return m_rclick_row; }

bool ListBox::AutoScrollingUp() const
{ return m_auto_scrolling_up; }

bool ListBox::AutoScrollingDown() const
{ return m_auto_scrolling_down; }

bool ListBox::AutoScrollingLeft() const
{ return m_auto_scrolling_left; }

bool ListBox::AutoScrollingRight() const
{ return m_auto_scrolling_right; }

void ListBox::KeyPress(KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys)
{
  if (!IsDisabled()) {
    bool bring_caret_into_view = true;
    switch (key) {
    case VKC_SPACE: // space bar (selects item under caret like a mouse click)
      if (m_caret != m_rows.end()) {
        m_old_sel_row_selected = m_selections.find(m_caret) != m_selections.end();
        ClickAtRow(m_caret, mod_keys);
      }
      break;
    case VKC_DELETE: // delete key
      if (m_style & LIST_USERDELETE) {
        if (m_style & LIST_NOSEL) {
          if (m_caret != m_rows.end())
            delete Erase(m_caret, false, true);
        } else {
          for (SelectionSet::iterator it = m_selections.begin(); it != m_selections.end(); ++it) {
            delete Erase(*it, false, true);
          }
          m_selections.clear();
        }
      }
      break;

    // vertical scrolling keys
    case VKC_UP: // arrow-up (not numpad arrow)
      if (m_caret != m_rows.end() && m_caret != m_rows.begin())
        --m_caret;
      break;
    case VKC_DOWN: // arrow-down (not numpad arrow)
      if (m_caret != m_rows.end() && m_caret != --m_rows.end())
        ++m_caret;
      break;
    case VKC_PAGEUP: // page up key (not numpad key)
      if (m_caret != m_rows.end()) {
        int space = GetClientSize().y;
        while (m_caret != m_rows.begin() && 0 < (space -= (*boost::prior(m_caret))->GetHeight())) {
          --m_caret;
        }
      }
      break;
    case VKC_PAGEDOWN: // page down key (not numpad key)
      if (m_caret != m_rows.end()) {
        int space = GetClientSize().y;
        while (m_caret != --m_rows.end() && 0 < (space -= (*m_caret)->GetHeight())) {
          ++m_caret;
        }
      }
      break;
    case VKC_HOME: // home key (not numpad)
      if (m_caret != m_rows.end())
        m_caret = m_rows.begin();
      break;
    case VKC_END: // end key (not numpad)
      if (m_caret != m_rows.end())
        m_caret = --m_rows.end();
      break;

    // horizontal scrolling keys
    case VKC_LEFT: // left key (not numpad key)
      if (m_first_col_shown) {
        --m_first_col_shown;
        /*m_hscroll->ScrollTo(
          Value(std::accumulate(m_col_widths.begin(), m_col_widths.begin() + m_first_col_shown, 0)));
        SignalScroll(*m_hscroll, true);*/
      }
      break;
    case VKC_RIGHT:{ // right key (not numpad)
      std::size_t last_fully_visible_col = LastVisibleCol();
      if (std::accumulate(m_col_widths.begin(), m_col_widths.begin() + last_fully_visible_col, 0) >
          GetClientSize().x) {
        --last_fully_visible_col;
      }
      if (last_fully_visible_col < m_col_widths.size() - 1) {
        ++m_first_col_shown;
        //m_hscroll->ScrollTo(
        //  Value(std::accumulate(m_col_widths.begin(), m_col_widths.begin() + m_first_col_shown, 0)));
        //SignalScroll(*m_hscroll, true);
      }
      break;}

    // any other key gets passed along to the parent
    default:
      Control::KeyPress(key, key_code_point, mod_keys);
      bring_caret_into_view = false;
    }

    if (bring_caret_into_view &&
      key != VKC_SPACE && key != VKC_DELETE && key != VKC_LEFT && key != VKC_RIGHT) {
      BringCaretIntoView();
    }
  } else {
    Control::KeyPress(key, key_code_point, mod_keys);
 }
}

void ListBox::DragDropEnter(const Point& pt, const std::map<Widget*, Point>& drag_drop_wnds, Flags<ModKey> mod_keys)
{
    ResetAutoScrollVars();
    DragDropHere(pt, drag_drop_wnds, mod_keys);
}

void ListBox::DragDropHere(const Point& pt, const std::map<Widget*, Point>& drag_drop_wnds, Flags<ModKey> mod_keys)
{
    if (!m_rows.empty() && m_auto_scroll_during_drag_drops && InClient(pt)) {
        const Point MARGIN_OFFSET = Point(int(m_auto_scroll_margin), int(m_auto_scroll_margin));
        Rect client_no_scroll_hole(GetClientUpperLeft() + MARGIN_OFFSET, GetClientLowerRight() - MARGIN_OFFSET);
        m_auto_scrolling_up = pt.y < client_no_scroll_hole.ul.y;
        m_auto_scrolling_down = client_no_scroll_hole.lr.y < pt.y;
        m_auto_scrolling_left = pt.x < client_no_scroll_hole.ul.x;
        m_auto_scrolling_right = client_no_scroll_hole.lr.x < pt.x;
        if (m_auto_scrolling_up || m_auto_scrolling_down || m_auto_scrolling_left || m_auto_scrolling_right) {
            bool acceptible_drop = false;
            for (std::map<Widget*, Point>::const_iterator it = drag_drop_wnds.begin(); it != drag_drop_wnds.end(); ++it) {
                if (m_allowed_drop_types.find("") != m_allowed_drop_types.end() ||
                    m_allowed_drop_types.find(it->first->GetDragDropDataType()) != m_allowed_drop_types.end()) {
                    acceptible_drop = true;
                    break;
                }
            }
            if (acceptible_drop) {
                //if (!m_auto_scroll_timer.Running()) {
                //    m_auto_scroll_timer.Reset(GUI::GetGUI()->Ticks());
                //    m_auto_scroll_timer.Start();
                //}
            } else {
                DragDropLeave();
            }
        }
    }
}

void ListBox::DragDropLeave()
{ ResetAutoScrollVars(); }

//void ListBox::TimerFiring(unsigned int ticks, Timer* timer)
//{
//    if (timer == &m_auto_scroll_timer && !m_rows.empty()) {
//        if (m_vscroll) {
//            if (m_auto_scrolling_up &&
//                m_first_row_shown != m_rows.end() &&
//                m_first_row_shown != m_rows.begin()) {
//                m_vscroll->ScrollTo(m_vscroll->PosnRange().first -
//                                    Value((*boost::prior(m_first_row_shown))->Height()));
//                SignalScroll(*m_vscroll, true);
//            }
//            if (m_auto_scrolling_down) {
//                iterator last_visible_row = LastVisibleRow();
//                if (last_visible_row != m_rows.end() &&
//                    (last_visible_row != --m_rows.end() ||
//                     ClientLowerRight().y < (*last_visible_row)->LowerRight().y)) {
//                    m_vscroll->ScrollTo(m_vscroll->PosnRange().first +
//                                        Value((*m_first_row_shown)->Height()));
//                    SignalScroll(*m_vscroll, true);
//                }
//            }
//        }
//        if (m_hscroll) {
//            if (m_auto_scrolling_left && 0 < m_first_col_shown) {
//                m_hscroll->ScrollTo(m_hscroll->PosnRange().first -
//                                    Value(m_col_widths[m_first_col_shown - 1]));
//                SignalScroll(*m_hscroll, true);
//            }
//            if (m_auto_scrolling_right) {
//                std::size_t last_visible_col = LastVisibleCol();
//                if (last_visible_col < m_col_widths.size() - 1 ||
//                    ClientLowerRight().x < m_rows.front()->LowerRight().x) {
//                    m_hscroll->ScrollTo(m_hscroll->PosnRange().first +
//                                        Value(m_col_widths[m_first_col_shown]));
//                    SignalScroll(*m_hscroll, true);
//                }
//            }
//        }
//    }
//}

bool ListBox::EventFilter(Widget* w, const WidgetEvent& event)
{
    NE_ASSERT(w == this || dynamic_cast<Row*>(w), "")();

    if (!IsDisabled()) {
        Point pt = event.GetPoint();
        Flags<ModKey> mod_keys = event.GetModKeys();

        switch (event.GetType()) {
        case WidgetEvent::TouchDown: {
          m_old_sel_row = RowUnderPt(pt);
          if (!InClient(pt)) {
            m_old_sel_row = m_rows.end();
          } else if (m_old_sel_row != m_rows.end()) {
            m_old_sel_row_selected = m_selections.find(m_old_sel_row) != m_selections.end();
            if (!(m_style & LIST_NOSEL) && !m_old_sel_row_selected)
              ClickAtRow(m_old_sel_row, mod_keys);
          }
          break;
        }

        case WidgetEvent::TouchUp: {
          m_old_sel_row = m_rows.end();
          break;
        }

        case WidgetEvent::Clicked: {
          if (m_old_sel_row != m_rows.end() && InClient(pt)) {
            iterator sel_row = RowUnderPt(pt);
            if (sel_row == m_old_sel_row) {
              if (m_style & LIST_NOSEL)
                m_caret = sel_row;
              else
                  ClickAtRow(sel_row, mod_keys);
              m_lclick_row = sel_row;
              LeftClickedSignal(sel_row, pt);
            }
          }
          break;
        }

        case WidgetEvent::DoubleClicked: {
          iterator row = RowUnderPt(pt);
          if (row != m_rows.end() && row == m_lclick_row && InClient(pt)) {
            DoubleClickedSignal(row);
            m_old_sel_row = m_rows.end();
          } else {
            Clicked(pt, mod_keys);
          }
          break;
        }

        //case WndEvent::MouseEnter: {
        //    if (m_style & LIST_BROWSEUPDATES) {
        //        iterator sel_row = RowUnderPoint(pt);
        //        if (m_last_row_browsed != sel_row)
        //            BrowsedSignal(m_last_row_browsed = sel_row);
        //    }
        //    break;
        //}

        //case WndEvent::MouseHere:
        //    break;

        //case WndEvent::MouseLeave: {
        //    if (m_style & LIST_BROWSEUPDATES) {
        //        if (m_last_row_browsed != m_rows.end())
        //            BrowsedSignal(m_last_row_browsed = m_rows.end());
        //    }
        //    break;
        //}

        //case WndEvent::GainingFocus: {
        //    if (w == this)
        //        return false;
        //    GUI::GetGUI()->SetFocusWnd(this);
        //    break;
        //}

        //case WndEvent::MouseWheel:
        //    return false;

        case WidgetEvent::DragDropEnter:
        case WidgetEvent::DragDropHere:
        case WidgetEvent::DragDropLeave:
            if (w == this)
                return false;
            HandleEvent(event);
            break;

        case WidgetEvent::KeyPress:
        case WidgetEvent::KeyRelease:
            return false;

        default:
            break;
        }
    }
    return true;
}

ListBox::iterator ListBox::Insert(Row* row, iterator it, bool dropped)
{
    // Track the originating row in case this is an intra-ListBox
    // drag-and-drop.
    iterator original_dropped_position = m_rows.end();
    if (dropped)
        original_dropped_position = std::find(m_rows.begin(), m_rows.end(), row);

    iterator retval = it;

    // The first row inserted into an empty list box defines the number of
    // columns, and initializes the column widths and alignments.
    if (m_rows.empty() && (m_col_widths.empty() || !m_keep_col_widths)) {
        const int WIDTH = GetClientSize().x - SCROLL_WIDTH;

        m_col_widths.resize(row->size());
        m_col_alignments.resize(row->size());
        int total = 0;
        for (std::size_t i = 0; i < row->size(); ++i) {
            // use the column width from the Row
            total += row->ColWidth(i);

            // use the column alignment from the Row, if it has been set;
            // otherwise, use the one dictated by the ListBoxStyle flags
            Alignment a = row->ColAlignment(i);
            if (a == ALIGN_NONE)
                a = AlignmentFromStyle(m_style);
            m_col_alignments[i] = a;
        }

        const int SCALE_FACTOR = 1.0 * WIDTH / total;

        total = 0;
        for (std::size_t i = 0; i < row->size(); ++i) {
            total += (m_col_widths[i] = row->ColWidth(i) * SCALE_FACTOR);
        }
        m_col_widths.back() += total - WIDTH;

        if (!m_header_row->empty())
            NormalizeRow(m_header_row);
    }

    row->InstallEventFilter(this);
    NormalizeRow(row);

    Point insertion_pt;
    if (m_rows.empty()) {
        m_rows.push_back(row);
        retval = m_rows.begin();
    } else {
        if (!(m_style & LIST_NOSORT)) {
            retval = m_rows.begin();
            RowSorter cmp(m_sort_cmp, m_sort_col, (m_style & LIST_SORTDESCENDING) != 0);
            while (retval != m_rows.end() && !cmp(row, *retval)) {
                ++retval;
            }
        }
        int y = retval == m_rows.end() ?
            m_rows.back()->GetRelativeLowerRight().y : (*retval)->GetRelativeUpperLeft().y;
        insertion_pt = Point(0, y);
        retval = m_rows.insert(retval, row);
    }

    int row_height = row->GetHeight();
    for (iterator it2 = boost::next(retval); it2 != m_rows.end(); ++it2) {
        (*it2)->OffsetMove(Point(0, row_height));
    }

    AttachChild(row);
    row->MoveTo(insertion_pt);

    if (m_first_row_shown == m_rows.end())
        m_first_row_shown = m_rows.begin();

    if (dropped) {
        // TODO: Can these be inverted without breaking anything?  It would be
        // semantically clearer if they were.
        DroppedSignal(retval);
        if (original_dropped_position != m_rows.end())
            Erase(original_dropped_position, true, false);
    }

    return retval;
}

ListBox::Row* ListBox::Erase(iterator it, bool removing_duplicate, bool signal)
{
    if (it != m_rows.end()) {
        if (m_iterator_being_erased) {
            *m_iterator_being_erased = m_rows.end();
            return 0;
        }

        Row* row = *it;
        int row_height = row->GetHeight();
        if (!removing_duplicate) {
            DetachChild(row);
            row->RemoveEventFilter(this);
        }

        for (iterator it2 = boost::next(it); it2 != m_rows.end(); ++it2) {
            (*it2)->OffsetMove(Point(0, -row_height));
        }

        ResetIfEqual(m_old_sel_row, it, m_rows.end());
        ResetIfEqual(m_old_rdown_row, it, m_rows.end());
        ResetIfEqual(m_lclick_row, it, m_rows.end());
        ResetIfEqual(m_rclick_row, it, m_rows.end());
        ResetIfEqual(m_last_row_browsed, it, m_rows.end());

        bool check_first_row_and_caret_for_end = false;
        if (m_first_row_shown == it) {
            ++m_first_row_shown;
            check_first_row_and_caret_for_end = true;
        }
        if (m_caret == it) {
            ++m_caret;
            check_first_row_and_caret_for_end = true;
        }

        // Tracking this iterator is necessary because the signal may indirectly
        // cause the iterator to be invalidated.
        ScopedSet scoped_set(m_iterator_being_erased, &it);

        if (signal && !removing_duplicate)
            ErasedSignal(it);

        if (it != m_rows.end()) {
            m_selections.erase(it);
            m_rows.erase(it);
        }

        if (check_first_row_and_caret_for_end && m_first_row_shown == m_rows.end() && !m_rows.empty())
            --m_first_row_shown;
        if (check_first_row_and_caret_for_end && m_caret == m_rows.end() && !m_rows.empty())
            --m_caret;

        return row;
    } else {
        return 0;
    }
}

void ListBox::BringCaretIntoView()
{ BringRowIntoView(m_caret); }

void ListBox::ResetAutoScrollVars()
{
    m_auto_scrolling_up = false;
    m_auto_scrolling_down = false;
    m_auto_scrolling_left = false;
    m_auto_scrolling_right = false;
}

void ListBox::Resort()
{
    Row* caret = SafeDeref(m_caret, m_rows.end());
    std::set<Row*> selections;
    for (SelectionSet::const_iterator it = m_selections.begin(); it != m_selections.end(); ++it) {
        selections.insert(**it);
    }
    m_selections.clear();
    Row* old_sel_row = SafeDeref(m_old_sel_row, m_rows.end());
    Row* old_rdown_row = SafeDeref(m_old_rdown_row, m_rows.end());
    Row* lclick_row = SafeDeref(m_lclick_row, m_rows.end());
    Row* rclick_row = SafeDeref(m_rclick_row, m_rows.end());
    Row* last_row_browsed = SafeDeref(m_last_row_browsed, m_rows.end());

    std::vector<Row*> rows_vec(m_rows.size());
    std::copy(m_rows.begin(), m_rows.end(), rows_vec.begin());
    std::stable_sort(rows_vec.begin(), rows_vec.end(),
                     RowSorter(m_sort_cmp, m_sort_col, (m_style & LIST_SORTDESCENDING) != 0));
    m_rows.clear();
    m_rows.insert(m_rows.begin(), rows_vec.begin(), rows_vec.end());

    if (m_iterator_being_erased)
        *m_iterator_being_erased = m_rows.end();

    int y(0);
    for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
        Row* row = *it;
        if (caret == row)
            m_caret = it;
        if (selections.find(row) != selections.end())
            m_selections.insert(it);
        if (old_sel_row == row)
            m_old_sel_row = it;
        if (old_rdown_row == row)
            m_old_rdown_row = it;
        if (lclick_row == row)
            m_lclick_row = it;
        if (rclick_row == row)
            m_rclick_row = it;
        if (last_row_browsed == row)
            m_last_row_browsed = it;

        row->MoveTo(Point(0, y));
        y += row->GetHeight();
    }

    m_first_row_shown = m_rows.empty() ? m_rows.end() : m_rows.begin();
}

void ListBox::ConnectSignals()
{
    //if (m_vscroll)
    //    Connect(m_vscroll->ScrolledSignal, &ListBox::VScrolled, this);
    //if (m_hscroll)
    //    Connect(m_hscroll->ScrolledSignal, &ListBox::HScrolled, this);
}

void ListBox::ValidateStyle()
{
    int dup_ct = 0;   // duplication count
    if (m_style & LIST_LEFT) ++dup_ct;
    if (m_style & LIST_RIGHT) ++dup_ct;
    if (m_style & LIST_CENTER) ++dup_ct;
    if (dup_ct != 1) {  // exactly one must be picked; when none or multiples are picked, use LIST_LEFT by default
        m_style &= ~(LIST_RIGHT | LIST_CENTER);
        m_style |= LIST_LEFT;
    }
    dup_ct = 0;
    if (m_style & LIST_TOP) ++dup_ct;
    if (m_style & LIST_BOTTOM) ++dup_ct;
    if (m_style & LIST_VCENTER) ++dup_ct;
    if (dup_ct != 1) {  // exactly one must be picked; when none or multiples are picked, use LIST_VCENTER by default
        m_style &= ~(LIST_TOP | LIST_BOTTOM);
        m_style |= LIST_VCENTER;
    }
    dup_ct = 0;
    if (m_style & LIST_NOSEL) ++dup_ct;
    if (m_style & LIST_SINGLESEL) ++dup_ct;
    if (m_style & LIST_QUICKSEL) ++dup_ct;
    if (1 < dup_ct)  // at most one of these may be picked; when multiples are picked, disable all of them
        m_style &= ~(LIST_NOSEL | LIST_SINGLESEL | LIST_QUICKSEL);
}

void ListBox::VScrolled(int tab_low, int tab_high, int low, int high)
{
    m_first_row_shown = m_rows.empty() ? m_rows.end() : m_rows.begin();
    int accum(0);
    int position(0);
    for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
        int row_height = (*it)->GetHeight();
        if (tab_low < accum + row_height / 2) {
            m_first_row_shown = it;
            position = -accum;
            break;
        }
        accum += row_height;
    }
    int initial_x = m_rows.empty() ? 0 : (*m_rows.begin())->GetRelativeUpperLeft().x;
    for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
        (*it)->MoveTo(Point(initial_x, position));
        position += (*it)->GetHeight();
    }
}

void ListBox::HScrolled(int tab_low, int tab_high, int low, int high)
{
    m_first_col_shown = 0;
    int accum(0);
    int position(0);
    for (std::size_t i = 0; i < m_col_widths.size(); ++i) {
        int col_width = m_col_widths[i];
        if (tab_low < accum + col_width / 2) {
            m_first_col_shown = i;
            position = -accum;
            break;
        }
        accum += col_width;
    }
    for (iterator it = m_rows.begin(); it != m_rows.end(); ++it) {
        (*it)->MoveTo(Point(position, (*it)->GetRelativeUpperLeft().y));
    }
    m_header_row->MoveTo(Point(position, m_header_row->GetRelativeUpperLeft().y));
}

void ListBox::ClickAtRow(iterator it, Flags<ModKey> mod_keys)
{
    NE_ASSERT(it != m_rows.end(),"")();
    NE_ASSERT(!m_rows.empty(),"")();

    SelectionSet previous_selections = m_selections;
    if (m_style & LIST_SINGLESEL) {
        // No special keys are being used; just clear all previous selections,
        // select this row, set the caret here.
        m_selections.clear();
        m_selections.insert(it);
        m_caret = it;
    } else {
        if (mod_keys & MOD_KEY_CTRL) { // control key depressed
            if (mod_keys & MOD_KEY_SHIFT && m_caret != m_rows.end()) {
                // Both shift and control keys are depressed.
                iterator low  = LessThan(m_caret, it, m_rows.end()) ? m_caret : it;
                iterator high = LessThan(m_caret, it, m_rows.end()) ? it : m_caret;
                bool erase = m_selections.find(m_caret) == m_selections.end();
                if (high != m_rows.end())
                    ++high;
                for (iterator it2 = low; it2 != high; ++it2) {
                    if (erase)
                        m_selections.erase(it2);
                    else
                        m_selections.insert(it2);
                }
            } else { // just the control key is depressed: toggle the item selected, adjust the caret
                if (m_old_sel_row_selected)
                    m_selections.erase(it);
                else
                    m_selections.insert(it);
                m_caret = it;
            }
        } else if (mod_keys & MOD_KEY_SHIFT) { // shift key depressed
            bool erase = m_selections.find(m_caret) == m_selections.end();
            if (!(m_style & LIST_QUICKSEL))
                m_selections.clear();
            if (m_caret == m_rows.end()) {
                // No previous caret exists; select this row, mark it as the caret.
                m_selections.insert(it);
                m_caret = it;
            } else { // select all rows between the caret and this row (inclusive), don't move the caret
                iterator low  = LessThan(m_caret, it, m_rows.end()) ? m_caret : it;
                iterator high = LessThan(m_caret, it, m_rows.end()) ? it : m_caret;
                if (high != m_rows.end())
                    ++high;
                for (iterator it2 = low; it2 != high; ++it2) {
                    if (erase)
                        m_selections.erase(it2);
                    else
                        m_selections.insert(it2);
                }
            }
        } else { // unless LIST_QUICKSEL is used, this is treated just like LIST_SINGLESEL above
            if (m_style & LIST_QUICKSEL) {
                if (m_old_sel_row_selected)
                    m_selections.erase(it);
                else
                    m_selections.insert(it);
                m_caret = it;
            } else {
                m_selections.clear();
                m_selections.insert(it);
                m_caret = it;
            }
        }
    }

    if (previous_selections != m_selections)
        SelChangedSignal(m_selections);
}

void ListBox::NormalizeRow(Row* row)
{
    Row::DeferAdjustLayout defer_adjust_layout(row);
    row->resize(m_col_widths.size());
    row->SetColWidths(m_col_widths);
    row->SetColAlignments(m_col_alignments);
    row->SetMargin(m_cell_margin);
    row->Resize(Point(std::accumulate(m_col_widths.begin(), m_col_widths.end(), 0), row->GetHeight()));
}

ListBox::iterator ListBox::FirstRowShownWhenBottomIs(iterator bottom_row, int client_height)
{
    if (bottom_row == m_rows.end())
        return m_rows.begin();
    int available_space = client_height - (*bottom_row)->GetHeight();
    iterator it = bottom_row;
    while (it != m_rows.begin() && (*boost::prior(it))->GetHeight() <= available_space) {
        available_space -= (*--it)->GetHeight();
    }
    return it;
}

std::size_t ListBox::FirstColShownWhenRightIs(std::size_t right_col, int client_width)
{
    if (right_col == static_cast<std::size_t>(-1))
        return 0;
    int available_space = client_width - m_col_widths[right_col];
    std::size_t i = right_col;
    while (0 < i && m_col_widths[i - 1] <= available_space) {
        available_space -= m_col_widths[--i];
    }
    return i;
}
