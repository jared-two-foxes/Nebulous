
#include "Slider.h"

#include <Nebulae/Beta/Gui/WidgetEvent.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>


using namespace Nebulae;


const Real INVALID_PAGE_SIZE = std::numeric_limits<Real>::max();


Slider::Slider( const WidgetFactory& factory, int x, int y, int w, int h, Real min, Real max,
                uint32 tabWidth, uint32 lineWidth, uint32 flags )
  : Control( factory, x, y, w, h, flags )
  , m_posn(min)
  , m_rangeMin(min)
  , m_rangeMax(max)
  , m_pageSize(INVALID_PAGE_SIZE)
  , m_lineWidth(lineWidth)
  , m_tabWidth(tabWidth)
  , m_lineStyle(RAISED)
  , m_tabDragOffset(-1)
  , m_tab( GetWidgetFactory().CreateSliderTabButton(0, 0, m_tabWidth, GetHeight(), "", std::shared_ptr<Font>(), CLR_WHITE) )
  , m_draggingTab(false)
{
  //Control::SetColour(color);
  AttachChild( m_tab );
  m_tab->InstallEventFilter( this );
  SizeMove( GetUpperLeft(), GetLowerRight() );
}


Nebulae::Point
Slider::GetMinUsableSize() const
{
  Point tabMin = m_tab->GetMinUsableSize();
  return Point( GetSize().x, tabMin.y );
}


Real
Slider::GetPosn() const
{
  return m_posn; 
}


std::pair<Real, Real>
Slider::GetSliderRange() const
{ 
  return std::pair<Real, Real>(m_rangeMin, m_rangeMax); 
}


Real 
Slider::GetPageSize() const
{
  return m_pageSize != INVALID_PAGE_SIZE ? m_pageSize : (m_rangeMax - m_rangeMin) / 10; 
}


uint32 
Slider::GetTabWidth() const
{ 
  return m_tabWidth; 
}


uint32 
Slider::GetLineWidth() const
{ 
  return m_lineWidth; 
}


SliderLineStyle
Slider::GetLineStyle() const
{ 
  return m_lineStyle; 
}

bool
Slider::IsTabBeingDragged() const
{
  return (m_tab->GetState() == ButtonControl::BN_PRESSED);
}


void
Slider::SizeMove( const Point& ul, const Point& lr )
{
  Widget::SizeMove( ul, lr );
  m_tab->SizeMove( Point(0,0), Point(int(m_tabWidth), lr.y - ul.y) );
  MoveTabToPosn();
}


void
Slider::SetDisabled( bool b )
{
  Control::SetDisabled( b );
  m_tab->SetDisabled( b );
}


void
Slider::SetColour( const Colour& c )
{
  Control::SetColour( c );
  m_tab->SetColour( c );
}


void
Slider::SizeSlider( Real min, Real max )
{
  NE_ASSERT( m_rangeMin != m_rangeMax, "")( m_rangeMin, m_rangeMax );

  m_rangeMin = min;
  m_rangeMax = max;
  if (m_posn < m_rangeMin) {
    SlideToImpl(m_rangeMin, false);
  } else if (m_rangeMax < m_posn) {
    SlideToImpl(m_rangeMax, false);
  } else {
    MoveTabToPosn();
  }
}


void 
Slider::SetMax(Real max)
{ 
  SizeSlider(m_rangeMin, max); 
}


void
Slider::SetMin(Real min)
{
  SizeSlider(min, m_rangeMax); 
}


void
Slider::SlideTo(Real p)
{ 
  SlideToImpl(p, false); 
}


ButtonControl* 
Slider::GetTab() const
{ 
  return m_tab; 
}


Real
Slider::PtToPosn(const Point& pt) const
{
  Point  ul                     = GetUpperLeft();
  Point  lr                     = GetLowerRight();
  int    lineMin                = m_tab->GetWidth() / 2;
  int    lineMax                = GetWidth() - (m_tab->GetWidth() - m_tab->GetWidth() / 2); //< this is OBVIOUSLY incorrect
  int    pixelNearestToPtOnLine = std::max(lineMin, std::min(pt.x - ul.x, lineMax));
  double fractionalDistance     = static_cast<double>(pixelNearestToPtOnLine) / (lineMax - lineMin);

  return m_rangeMin + static_cast<Real>((m_rangeMax - m_rangeMin) * fractionalDistance);
}


bool
Slider::EventFilter( Widget* w, const WidgetEvent& event )
{
  if( w == m_tab ) {
    switch( event.GetType() ) {
      case WidgetEvent::TouchDrag: {
        if( !IsDisabled() ) {
          Point new_ul = m_tab->GetRelativeUpperLeft() + event.GetDragMove();
          new_ul.x = std::max( 0, std::min(new_ul.x, GetClientWidth() - m_tab->GetWidth()) );
          new_ul.y = m_tab->GetRelativeUpperLeft().y;
          m_tab->MoveTo(new_ul);
          UpdatePosn();
        }
        return true;
      }
      case WidgetEvent::TouchDown:
        m_draggingTab = true;
        break;
      case WidgetEvent::TouchUp:
      case WidgetEvent::Clicked: {
        if( !IsDisabled() ) {
          SlidAndStoppedSignal( m_posn, m_rangeMin, m_rangeMax );
        }
        m_draggingTab = false;
        break;
      }
      case WidgetEvent::TouchLeave:
        return m_draggingTab;
      default:
        break;
    }
  }
  return false;
}


void
Slider::Clicked( const Point& pt, Flags<ModKey> mod_keys)
{
  SlideToImpl( m_posn < PtToPosn(pt) ? m_posn + GetPageSize() : m_posn - GetPageSize(), true );
}


void
Slider::KeyPress( KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys )
{
  if( !IsDisabled() ) {
    switch( key ) {
      case VKC_HOME:
        SlideToImpl( m_rangeMin, true );
        break;
      case VKC_END:
        SlideToImpl( m_rangeMax, true );
        break;
      case VKC_RIGHT:
        SlideToImpl( m_posn + (0 < (m_rangeMax - m_rangeMin) ? 1 : -1), true );
        break;
      case VKC_LEFT:  
        SlideToImpl( m_posn - (0 < (m_rangeMax - m_rangeMin) ? 1 : -1), true );
        break;
      case VKC_PLUS:
      case VKC_KP_PLUS:
        SlideToImpl( m_posn + 1, true );
        break;
      case VKC_MINUS:
      case VKC_KP_MINUS:
        SlideToImpl( m_posn - 1, true );
        break;
      default:
        Control::KeyPress( key, key_code_point, mod_keys );
        break;
    }
  } else {
    Control::KeyPress( key, key_code_point, mod_keys );
  }
}


void
Slider::MoveTabToPosn()
{
  NE_ASSERT( m_rangeMin <= m_posn && m_posn <= m_rangeMax || m_rangeMax <= m_posn && m_posn <= m_rangeMin, "" )( m_rangeMin, m_rangeMax, m_posn );

  double fractionalDistance = static_cast<double>(m_posn - m_rangeMin) / (m_rangeMax - m_rangeMin);
  int    tabWidth           = m_tab->GetWidth();
  int    lineLength         = GetWidth() - tabWidth;
  int    pixelDistance      = static_cast<int>(lineLength * fractionalDistance);

  m_tab->MoveTo( Point(pixelDistance, m_tab->GetRelativeUpperLeft().y) );
}


void
Slider::UpdatePosn()
{
  Real oldPosn            = m_posn;
  int  lineLength         = GetWidth() - m_tab->GetWidth();
  int  tabPosn            = m_tab->GetRelativeUpperLeft().x;
  Real fractionalDistance = static_cast<Real>(tabPosn) / lineLength;
  
  m_posn = m_rangeMin + (m_rangeMax - m_rangeMin) * fractionalDistance;
  if( m_posn != oldPosn ) {
    SlidSignal( m_posn, m_rangeMin, m_rangeMax);
  }
}


void
Slider::SlideToImpl( Real p, bool signal )
{
  Real oldPosn = m_posn;
  if( 0 < (m_rangeMax - m_rangeMin) ? p < m_rangeMin : p > m_rangeMin ) {
    m_posn = m_rangeMin;
  } else if( 0 < (m_rangeMax - m_rangeMin) ? m_rangeMax < p : m_rangeMax > p ) {
    m_posn = m_rangeMax;
  } else {
    m_posn = p;
  }
  MoveTabToPosn();
  if( signal && m_posn != oldPosn ) {
    SlidSignal( m_posn, m_rangeMin, m_rangeMax );
    SlidAndStoppedSignal( m_posn, m_rangeMin, m_rangeMax );
  }
}
