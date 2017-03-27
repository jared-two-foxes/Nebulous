
#include "Widget.h"
#include "WidgetEvent.h"

using namespace Nebulae;

Widget::Widget( const WidgetFactory& factory, int x, int y, int w, int h, uint32 flags )
	: m_id(-1)
  , m_parent(NULL)
  , m_upperLeft(x, y)
  , m_lowerRight(x+w, y+h)
	, m_zorder(0)
	, m_visible(true)
  , m_maxSize(1 << 30, 1 << 30)
  , m_flags(flags)
  , m_widgetFactory(factory)
{

}

Widget::~Widget()
{
  // remove this-references from Wnds that this Widget filters
  for( std::set<Widget*>::iterator it1 = m_filtering.begin(); it1 != m_filtering.end(); ++it1 ) {
    std::vector<Widget*>::iterator it2 = std::find( (*it1)->m_filters.begin(), (*it1)->m_filters.end(), this );
    if( it2 != (*it1)->m_filters.end() ) {
      (*it1)->m_filters.erase(it2);
    }
  }

  // remove this-references from Widgets that filter this Widget
  for( std::vector<Widget*>::iterator it1 = m_filters.begin(); it1 != m_filters.end(); ++it1 ) {
    (*it1)->m_filtering.erase(this);
  }

  if( Widget* parent = GetParent() ) {
    parent->DetachChild( this );
  }

  //GUI::GetGUI()->WidgetDying( this );

  DeleteChildren();
}


int
Widget::GetID() const
{
  return m_id;
}


const std::string&
Widget::GetName() const
{
  return m_name;
}


bool 
Widget::IsInteractive() const
{ 
  return (m_flags & INTERACTIVE) != 0; 
}


bool 
Widget::RepeatButtonDown() const
{
  return (m_flags & REPEAT_BUTTON_DOWN) != 0; 
}


bool 
Widget::IsDragable() const
{ 
  return (m_flags & DRAGABLE) != 0;
}


bool 
Widget::IsResizable() const
{ 
  return (m_flags & RESIZABLE) != 0;
}


bool 
Widget::IsOnTop() const
{ 
  return !m_parent && ((m_flags & ONTOP) != 0); 
}


bool 
Widget::IsModal() const
{ 
  return !m_parent && ((m_flags & MODAL) != 0); 
}


// Widget::ChildClippingMode 
// Widget::GetChildClippingMode() const
// { 
//   return m_childClippingMode; 
// }


// bool 
// Widget::NonClientChild() const
// {
//   return m_nonClientChild; 
// }


bool 
Widget::IsVisible() const
{
  return m_visible;
}


const std::string& 
Widget::GetDragDropDataType() const
{ 
  return m_dragDropDataType; 
}

void 
Widget::DropsAcceptable( DropsAcceptableIter first,
                         DropsAcceptableIter last,
                         const Point& pt ) const
{
  for( std::map<const Widget*, bool>::iterator it = first; it != last; ++it ) {
    it->second = false;
  }
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetUpperLeft() const 
{
  Point retval = m_upperLeft;
  if( m_parent ) { 
    retval += m_parent->GetClientUpperLeft();
  }
  return retval;
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetLowerRight() const 
{
  Point retval = m_lowerRight;
  if( m_parent ) {
    retval += m_parent->GetClientUpperLeft();
  }
  return retval;
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetRelativeUpperLeft() const
{
  return m_upperLeft; 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetRelativeLowerRight() const
{
  return m_lowerRight; 
}


int 
Widget::GetWidth() const
{
  return (m_lowerRight.x - m_upperLeft.x);
}


int 
Widget::GetHeight() const
{
  return (m_lowerRight.y - m_upperLeft.y);
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetSize() const
{
  return Point(m_lowerRight.x - m_upperLeft.x, m_lowerRight.y - m_upperLeft.y); 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetMinSize() const
{ 
  return m_minSize; 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetMaxSize() const
{ 
  return m_maxSize; 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetMinUsableSize() const
{ 
  return GetSize(); 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification 
Widget::GetClientUpperLeft() const
{
  return GetUpperLeft(); 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification 
Widget::GetClientLowerRight() const
{ 
  return GetLowerRight(); 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::GetClientSize() const
{
  return GetClientLowerRight() - GetClientUpperLeft(); 
}


int 
Widget::GetClientWidth() const
{
  return GetClientLowerRight().x - GetClientUpperLeft().x; 
}


int 
Widget::GetClientHeight() const
{ 
  return GetClientLowerRight().y - GetClientUpperLeft().y; 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::ScreenToWindow( const Point& pt ) const
{ 
  return pt - GetUpperLeft(); 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
Widget::ScreenToClient( const Point& pt ) const
{ 
  return pt - GetClientUpperLeft(); 
}


bool
Widget::InWindow( const Point& pt ) const
{ 
  return pt >= GetUpperLeft() && pt < GetLowerRight();
}


bool 
Widget::InClient( const Point& pt ) const
{
  return pt >= GetClientUpperLeft() && pt < GetClientLowerRight(); 
}


const std::list<Widget*>& 
Widget::GetChildren() const
{
  return m_children;
}


Widget* 
Widget::GetParent() const
{
  return m_parent;
}


Widget* 
Widget::GetRootParent() const
{
  Widget* retval = m_parent;
  while( retval && retval->GetParent() ) {
    retval = retval->GetParent();
  }
  return retval;
}


int 
Widget::GetDepth() const
{
  return m_zorder;
}


const WidgetFactory& 
Widget::GetWidgetFactory() const
{ 
  return m_widgetFactory; 
}


WidgetRegion 
Widget::GetWidgetRegion( const Point& pt ) const
{
  enum {LEFT = 0, MIDDLE = 1, RIGHT = 2};
  enum {TOP = 0, BOTTOM = 2};

  // window regions look like this:
  // 0111112
  // 3444445   // 4 is client area, 0,2,6,8 are corners
  // 3444445
  // 6777778

  int x_pos = MIDDLE;   // default & typical case is that the mouse is over the (non-border) client area
  int y_pos = MIDDLE;

  if( pt.x < GetClientUpperLeft().x ) { 
    x_pos = LEFT;
  } else if( pt.x > GetClientLowerRight().x ) {
    x_pos = RIGHT;
  }

  if( pt.y < GetClientUpperLeft().y ) {
    y_pos = TOP;
  } else if( pt.y > GetClientLowerRight().y ) {
    y_pos = BOTTOM;
  }

  return (IsResizable() ? WidgetRegion(x_pos + 3 * y_pos) : WR_NONE);
}


void 
Widget::SetDragDropDataType( const std::string& dataType )
{
  m_dragDropDataType = dataType; 
}


void
Widget::StartingChildDragDrop( const Widget* wnd, const Point& offset )
{
}


void 
Widget::AcceptDrops( const std::vector<Widget*>& widgets, const Point& pt )
{
}


void 
Widget::CancellingChildDragDrop( const std::vector<const Widget*>& widgets ) 
{
}


void 
Widget::ChildrenDraggedAway( const std::vector<Widget*>& widgets, const Widget* destination )
{
  for( std::vector<Widget*>::const_iterator it = widgets.begin(); it != widgets.end(); ++it ) {
    DetachChild(*it);
  }
}


void
Widget::SetName( const std::string& name )
{
  m_name = name; 
}


void 
Widget::Hide( bool children )
{
  m_visible = false;
  if( children ) {
    std::list<Widget*>::iterator it = m_children.begin();
    for( ; it != m_children.end(); ++it ) {
      (*it)->Hide( children );
    }
  }
}


void 
Widget::Show( bool children )
{
  m_visible = true;
  if( children ) {
    std::list<Widget*>::iterator it = m_children.begin();
    for( ; it != m_children.end(); ++it ) {
      (*it)->Show( children );
    }
  }
}


void 
Widget::MoveTo( const Point& pt )
{
  SizeMove( pt, pt + GetSize() ); 
}


void 
Widget::OffsetMove( const Point& pt )
{
  SizeMove(m_upperLeft + pt, m_lowerRight + pt); 
}


void
Widget::SizeMove( const Point& ul_, const Point& lr_ )
{
  Point ul = ul_, lr = lr_;
  Point original_sz = GetSize();
  bool  resized = (original_sz.x != (lr.x - ul.x)) || (original_sz.y != (lr.y - ul.y));
  if( resized ) {
    Point min_sz = GetMinSize();
    Point max_sz = GetMaxSize();
    if( lr.x - ul.x < min_sz.x ) {
      if( ul.x != m_upperLeft.x ) {
        ul.x = lr.x - min_sz.x;
      } else if( lr.x != m_lowerRight.x ) {
        lr.x = ul.x + min_sz.x;
      }
    } else if( max_sz.x < lr.x - ul.x ) {
      if( lr.x != m_lowerRight.x ) {
        lr.x = ul.x + max_sz.x;
      } else {
        ul.x = lr.x - max_sz.x;
      }
    }
    if(lr.y - ul.y < min_sz.y ) {
      if( ul.y != m_upperLeft.y ) {
        ul.y = lr.y - min_sz.y;
      } else if( lr.y != m_lowerRight.y ) {
        lr.y = ul.y + min_sz.y;
      }
    } else if( max_sz.y < lr.y - ul.y ) {
      if( lr.y != m_lowerRight.y ) {
        lr.y = ul.y + max_sz.y;
      } else {
        ul.y = lr.y - max_sz.y;
      }
    }
  }
  m_upperLeft  = ul;
  m_lowerRight = lr;
  if( resized ) {
    bool sizeChanged = GetSize() != original_sz;
    // if( m_layout && size_changed ) {
    //   m_layout->Resize(ClientSize());
    // }
    // if( m_containing_layout && size_changed && !dynamic_cast<Layout*>(this) ) {
    //   m_containing_layout->ChildSizeOrMinSizeOrMaxSizeChanged();
    // }
  }
}


void
Widget::Resize( const Point& sz )
{ 
  SizeMove(m_upperLeft, m_upperLeft + sz); 
}


void 
Widget::SetMinSize( const Point& sz )
{
  bool minSizeChanged = m_minSize != sz;
  m_minSize = sz;
  if( GetWidth() < m_minSize.x || GetHeight() < m_minSize.y ) {
    Resize( Point(std::max(GetWidth(), m_minSize.x), std::max(GetHeight(), m_minSize.y)) );
  }/* else if( m_containing_layout && minSizeChanged && !dynamic_cast<Layout*>(this) ) {
   m_containing_layout->ChildSizeOrMinSizeOrMaxSizeChanged();
  }*/
}


void
Widget::SetMaxSize( const Point& sz )
{
  m_maxSize = sz;
  if( m_maxSize.x < GetWidth() || m_maxSize.y < GetHeight() ) {
    Resize( Point(std::min(GetWidth(), m_maxSize.x), std::min(GetHeight(), m_maxSize.y)) );
  }
}


void 
Widget::AttachChild( Widget* widget )
{
  if( widget ) {
    // remove from previous parent, if any
    if( widget->GetParent() ) {
      widget->GetParent()->DetachChild( widget );
    }
    //GUI::GetGUI()->Remove( widget );
    m_children.push_back( widget );
    widget->m_parent = this;
  }
}


void 
Widget::MoveChildUp( Widget* widget )
{
  if( widget ) {
    if( std::find(m_children.begin(), m_children.end(), widget) != m_children.end() ) {
      m_children.remove( widget );
      m_children.push_back( widget );
    }
  }
}


void 
Widget::MoveChildDown( Widget* widget )
{
  if( widget ) {
    if( std::find(m_children.begin(), m_children.end(), widget) != m_children.end() ) {
      m_children.remove( widget );
      m_children.push_front( widget );
    }
  }
}


void
Widget::DetachChild( Widget* widget )
{
  if( widget ) {
    std::list<Widget*>::iterator it = std::find(m_children.begin(), m_children.end(), widget);
    if( it != m_children.end() ) {
      m_children.erase(it);
      widget->m_parent = 0;
    }
  }
}


void
Widget::DetachChildren()
{
  for( std::list<Widget*>::iterator it = m_children.begin(); it != m_children.end(); ) {
    std::list<Widget*>::iterator temp = it;
    ++it;
    DetachChild(*temp);
  }
}


void 
Widget::DeleteChild( Widget* widget )
{
  if( widget && std::find(m_children.begin(), m_children.end(), widget) != m_children.end() ) {
    delete widget;
  }
}


void 
Widget::DeleteChildren()
{
  for( std::list<Widget*>::iterator it = m_children.begin(); it != m_children.end(); ) {
    Widget* widget = *it++;
    delete widget;
  }
}

void
Widget::InstallEventFilter(Widget* widget)
{
  if( widget ) {
    RemoveEventFilter( widget );
    m_filters.push_back( widget );
    widget->m_filtering.insert( this );
  }
}

void 
Widget::RemoveEventFilter(Widget* widget)
{
  if( widget ) {
    std::vector<Widget*>::iterator it = std::find( m_filters.begin(), m_filters.end(), widget );
    if( it != m_filters.end() ) {
      m_filters.erase( it );
    }
    widget->m_filtering.erase( this );
  }
}


void 
Widget::TouchDown( const Point& pt, Flags<ModKey> mod_keys )
{
  if( !IsInteractive() ) {
    ForwardEventToParent(); 
  }
}


void 
Widget::TouchDrag( const Point& pt, const Point& move, Flags<ModKey> mod_keys )
{
  if( IsDragable() ) {
    OffsetMove(move);
  } else if( !IsInteractive() ) {
    ForwardEventToParent();
  }
}


void 
Widget::TouchUp( const Point& pt, Flags<ModKey> mod_keys )
{
  if( !IsInteractive() ) {
   ForwardEventToParent(); 
  }
}


void 
Widget::Clicked( const Point& pt, Flags<ModKey> mod_keys )
{ 
  if( !IsInteractive() ) {
    ForwardEventToParent(); 
  }
}


void
Widget::DragDropEnter( const Point& pt, const std::map<Widget*, Point>& dragDropWidgets, Flags<ModKey> mod_keys )
{
  if( !IsInteractive() ) {
    ForwardEventToParent(); 
  }
}


void 
Widget::DragDropHere( const Point& pt, const std::map<Widget*, Point>& dragDropWidgets, Flags<ModKey> mod_keys )
{ 
  if( !IsInteractive() ) {
    ForwardEventToParent(); 
  }
}


void 
Widget::DragDropLeave()
{ 
  if( !IsInteractive() ) {
    ForwardEventToParent(); 
  }
}


void
Widget::KeyPress( KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys )
{
  if( !IsInteractive() ) {
    ForwardEventToParent(); 
  }
}


void
Widget::KeyRelease( KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys )
{ 
  if( !IsInteractive() ) {
    ForwardEventToParent();
  }
}


void
Widget::HandleEvent( const WidgetEvent& event )
{
  for( std::vector<Widget*>::reverse_iterator it = m_filters.rbegin(); it != m_filters.rend(); ++it ) {
    if( (*it)->EventFilter(this, event) ) {
      return;
    }
  }
  
  switch( event.GetType() ) {
    case WidgetEvent::TouchDown:
      TouchDown(event.GetPoint(), event.GetModKeys());
      break;
    case WidgetEvent::TouchDrag:
      TouchDrag(event.GetPoint(), event.GetDragMove(), event.GetModKeys());
      break;
    case WidgetEvent::TouchUp:
      TouchUp(event.GetPoint(), event.GetModKeys());
      break;
    case WidgetEvent::Clicked:
      Clicked(event.GetPoint(), event.GetModKeys());
      break;
    case WidgetEvent::DragDropEnter:
      DragDropEnter(event.GetPoint(), event.GetDragDropWidgets(), event.GetModKeys());
      break;
    case WidgetEvent::DragDropHere:
      DragDropHere(event.GetPoint(), event.GetDragDropWidgets(), event.GetModKeys());
      break;
    case WidgetEvent::DragDropLeave:
      DragDropLeave();
      break;
    case WidgetEvent::KeyPress:
      KeyPress(event.GetKey(), event.GetKeyCodePoint(), event.GetModKeys());
      break;
    case WidgetEvent::KeyRelease:
      KeyRelease(event.GetKey(), event.GetKeyCodePoint(), event.GetModKeys());
      break;
    default:
      break;
  }  
}


bool 
Widget::EventFilter( Widget* widget, const WidgetEvent& event )
{ 
  return false; 
}


void 
Widget::ForwardEventToParent()
{ 
  //throw ForwardToParentException(); 
}
