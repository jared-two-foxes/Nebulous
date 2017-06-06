
#include "GuiManager.h"

#include <Nebulae/Beta/Gui/DepthList.h>
#include <Nebulae/Beta/Gui/Widget.h>
#include <Nebulae/Beta/Gui/WidgetEvent.h>
#include <Nebulae/Beta/Gui/WidgetFactory.h>
#include <Nebulae/Beta/Gui/Control/Control.h>

using namespace Nebulae;


// implementation data types
struct Nebulae::GUIImpl
{
  GUIImpl( GuiManager::FileArchivePtr fileSystem, std::shared_ptr<RenderSystem> renderSystem ) :
    m_focusWidget(0),
    m_mousePos(-1000, -1000),
    m_mouseRel(0, 0),
    m_modKeys(),
    m_buttonDownRepeatDelay(250),
    m_buttonDownRepeatInterval(66),
    m_lastButtonDownRepeatTime(0),
    m_doubleClickInterval(500),
    m_minDragTime(50),   //m_minDragTime(250),
    m_minDragDistance(1),//m_minDragDistance(5),
    m_prevButtonPressTime(-1),
    m_prevWidgetUnderCursor(0),
    m_prevWidgetUnderCursorTime(-1),
    m_currWidgetUnderCursor(0),
    m_dragWidgets(),
    m_currDragWidgetDragged(false),
    m_currDragWidget(0),
    m_currDragDropHereWidget(0),
    m_widgetRegion(WR_NONE),
    m_dragDropOriginatingWidget(0),
    m_delta_t(0),
    m_renderingDragDropWidgets(false),
    m_doubleClickWidget(0),
    m_doubleClickStartTime(-1),
    m_doubleClickTime(-1),
    m_fileSystem(fileSystem),
    m_widgetFactory(new WidgetFactory())
  {
    m_buttonState[0] = m_buttonState[1] = m_buttonState[2] = false;
    m_dragWidgets[0] = m_dragWidgets[1] = m_dragWidgets[2] = 0;

    m_start = std::chrono::high_resolution_clock::now();
  }

  bool HandlePress( GuiManager* mgr, uint32 mouseButton, const Point& pos, uint64 currTicks );
  bool HandleDrag( GuiManager* mgr, uint32 mouseButton, const Point& pos, uint64 currTicks );
  bool HandleRelease( GuiManager* mgr, uint32 mouseButton, const Point& pos, uint64 currTicks );

  GuiManager::FileArchivePtr              m_fileSystem;   ///< File archive controller.
  DepthList                               m_zlist;        ///< object that keeps the GUI windows in the correct depth ordering
  Widget*                                 m_focusWidget;  ///< GUI window that currently has the input focus (this is the base level focus window, used when no modal windows are active)
  std::list<std::pair<Widget*, Widget*> > m_modalWidgets; ///< modal GUI windows, and the window with focus for that modality (only the one in back is active, simulating a stack but allowing traversal of the list)

  std::chrono::high_resolution_clock::time_point m_start;

  bool          m_buttonState[3];           ///< the up/down states of the three buttons on the mouse are kept here
  Point         m_mousePos;                 ///< absolute position of mouse, based on last MOUSEMOVE event
  Point         m_mouseRel;                 ///< relative position of mouse, based on last MOUSEMOVE event
  Flags<ModKey> m_modKeys;                  ///< currently-depressed modifier keys, based on last KEYPRESS event

  uint64       m_buttonDownRepeatDelay;     ///< see note above GUI class definition
  uint64       m_buttonDownRepeatInterval;
  uint64       m_lastButtonDownRepeatTime;  ///< last time of a simulated button-down message

  int64        m_doubleClickInterval;       ///< the maximum interval allowed between clicks that is still considered a double-click, in ms
  uint64       m_minDragTime;               ///< the minimum amount of time that a drag must be in progress before it is considered a drag, in ms
  int          m_minDragDistance;           ///< the minimum distance that a drag must cover before it is considered a drag

  uint64       m_prevButtonPressTime;       ///< the time of the most recent mouse button press
  Point        m_prevButtonPressPos;        ///< the location of the most recent mouse button press
  Widget*      m_prevWidgetUnderCursor;     ///< GUI window most recently under the input cursor; may be 0
  int64        m_prevWidgetUnderCursorTime; ///< the time at which prev_wnd_under_cursor was initially set to its current value
  Widget*      m_currWidgetUnderCursor;     ///< GUI window currently under the input cursor; may be 0
  Widget*      m_dragWidgets[3];            ///< GUI window currently being clicked or dragged by each mouse button
  Point        m_prevWidgetDragPosition;    ///< the upper-left corner of the dragged window when the last *Drag message was generated
  Point        m_widgetDragOffset;          ///< the offset from the upper left corner of the dragged window to the cursor for the current drag
  bool         m_currDragWidgetDragged;     ///< true iff the currently-pressed window (m_drag_wnds[N]) has actually been dragged some distance (in which case releasing the mouse button is not a click)
  Widget*      m_currDragWidget;            ///< nonzero iff m_curr_drag_wnd_dragged is true (that is, we have actually started dragging the Wnd, not just pressed the mouse button); will always be one of m_drag_wnds
  Widget*      m_currDragDropHereWidget;    ///< the Wnd that most recently received a DragDropEnter or DragDropHere message (0 if DragDropLeave was sent as well, or if none)
  Point        m_widgetResizeOffset;        ///< offset from the cursor of either the upper-left or lower-right corner of the GUI window currently being resized
  WidgetRegion m_widgetRegion;              ///< window region currently being dragged or clicked; for non-frame windows, this will always be WR_NONE

  Widget*                       m_dragDropOriginatingWidget; ///< the window that originally owned the Wnds in drag_drop_wnds
  std::map<Widget*, Point>      m_dragDropWidgets;           ///< the Wnds (and their offsets) that are being dragged and dropped between Wnds
  std::map<const Widget*, bool> m_dragDropWidgetsAcceptable; ///< the Wnds being dragged and dropped, and whether they are acceptable for dropping over their current target.

  uint64       m_delta_t;                   ///< the number of ms since the last frame
  bool         m_renderingDragDropWidgets;  ///< Currently rendering drag and drop widgets.
  Widget*      m_doubleClickWidget;         ///< GUI window most recently clicked
  int64        m_doubleClickButton;         ///< the index of the mouse button used in the last click
  int64        m_doubleClickStartTime;      ///< the time from which we started measuring double_click_time, in ms
  int64        m_doubleClickTime;           ///< time elapsed since last click, in ms

  std::shared_ptr<WidgetFactory >      m_widgetFactory;
  std::vector<std::shared_ptr<Font > > m_fonts;
};


bool 
GUIImpl::HandlePress( GuiManager* mgr, uint32 mouseButton, const Point& pos, uint64 currTicks )
{
  m_currWidgetUnderCursor     = mgr->CheckedGetWindowUnder(pos, m_modKeys);
  m_lastButtonDownRepeatTime  = 0;
  m_prevWidgetDragPosition    = Point(0,0);
  m_widgetDragOffset          = Point(0,0);
  m_prevButtonPressTime       = 0;
  m_prevWidgetUnderCursorTime = currTicks;
  m_prevButtonPressTime       = currTicks;
  m_prevButtonPressPos        = pos;
  m_buttonState[mouseButton]  = true;
  m_dragWidgets[mouseButton]  = m_currWidgetUnderCursor; // track this window as the one being dragged by this mouse button
  if( m_currWidgetUnderCursor ) {
    m_prevWidgetDragPosition    = m_dragWidgets[mouseButton]->GetUpperLeft();
    m_widgetDragOffset          = pos - m_prevWidgetDragPosition;
  }

  // if this window is not a disabled Control window, it becomes the focus window
  Control* control = 0;
  if( m_dragWidgets[mouseButton] && (!(control = dynamic_cast<Control*>(m_dragWidgets[mouseButton])) || !control->IsDisabled())) {
    mgr->SetFocusWidget(m_dragWidgets[mouseButton]);
  }

  if( m_dragWidgets[mouseButton] ) {
    m_widgetRegion = m_dragWidgets[mouseButton]->GetWidgetRegion(pos); // and determine whether a resize-region of it is being dragged
    if( m_widgetRegion % 3 == 0 ) { // left regions
      m_widgetResizeOffset.x = m_dragWidgets[mouseButton]->GetUpperLeft().x - pos.x;
    } else {
      m_widgetResizeOffset.x = m_dragWidgets[mouseButton]->GetLowerRight().x - pos.x;
    }
    if( m_widgetRegion < 3 ) { // top regions
      m_widgetResizeOffset.y = m_dragWidgets[mouseButton]->GetUpperLeft().y - pos.y;
    } else {
      m_widgetResizeOffset.y = m_dragWidgets[mouseButton]->GetLowerRight().y - pos.y;
    }
    Widget* dragWidgetsRootParent = m_dragWidgets[mouseButton]->GetRootParent();
    mgr->MoveUp(dragWidgetsRootParent ? dragWidgetsRootParent : m_dragWidgets[mouseButton]);
    m_dragWidgets[mouseButton]->HandleEvent( WidgetEvent(WidgetEvent::TouchDown, pos, m_modKeys) );
  }

  m_prevWidgetUnderCursor = m_currWidgetUnderCursor; // update this for the next time around

  return (m_prevWidgetUnderCursor != NULL && m_prevWidgetUnderCursor->IsInteractive());
}


bool 
GUIImpl::HandleDrag( GuiManager* mgr, uint32 mouseButton, const Point& pos, uint64 currTicks )
{
  if( m_widgetRegion == WR_MIDDLE || m_widgetRegion == WR_NONE ) { // send drag message to window or initiate drag-and-drop
    Point diff = m_prevButtonPressPos - pos;
    int dragDistance = diff.x * diff.x + diff.y * diff.y;
    // ensure that the minimum drag requirements are met
    if( m_minDragTime < (currTicks - m_prevButtonPressTime) &&
        (m_minDragDistance * m_minDragDistance < dragDistance) &&
        m_dragDropWidgets.find(m_dragWidgets[mouseButton]) == m_dragDropWidgets.end() ) {
      if( !m_dragWidgets[mouseButton]->IsDragable() &&
          m_dragWidgets[mouseButton]->GetDragDropDataType() != "" &&
          mouseButton == 0 ) {
        Widget* parent = m_dragWidgets[mouseButton]->GetParent();
        Point   offset = m_prevButtonPressPos - m_dragWidgets[mouseButton]->GetUpperLeft();
        mgr->RegisterDragDropWidget(m_dragWidgets[mouseButton], offset, parent);
        if( parent ) {
          parent->StartingChildDragDrop(m_dragWidgets[mouseButton], offset);
        }
      } else {
        Point startPos = m_dragWidgets[mouseButton]->GetUpperLeft();
        Point move     = (pos - m_widgetDragOffset) - m_prevWidgetDragPosition;
        m_dragWidgets[mouseButton]->HandleEvent( WidgetEvent(WidgetEvent::TouchDrag, pos, move, m_modKeys) );
        m_prevWidgetDragPosition = m_dragWidgets[mouseButton]->GetUpperLeft();
        if( startPos != m_dragWidgets[mouseButton]->GetUpperLeft()) {
          m_currDragWidgetDragged = true;
          m_currDragWidget        = m_dragWidgets[mouseButton];
        }
      }
    }
    // notify widget under cursor of presence of drag-and-drop wnd(s)
    if( (m_currDragWidgetDragged &&
      m_dragWidgets[mouseButton]->GetDragDropDataType() != "" &&
      mouseButton == 0) ||
      !m_dragDropWidgets.empty()) {
      bool unregisteredDrag = m_dragDropWidgets.empty();
      std::set<Widget*> ignores;
      if( unregisteredDrag ) {
        ignores.insert(m_dragWidgets[mouseButton]);
      }
      m_currWidgetUnderCursor = m_zlist.Pick(pos, mgr->GetModalWindow(), &ignores);
      std::map<Widget*, Point> dragDropWidgets;
      dragDropWidgets[m_dragWidgets[mouseButton]] = m_widgetDragOffset;
      std::map<Widget*, Point>& dragDropWidgetsToUse = unregisteredDrag ? dragDropWidgets : m_dragDropWidgets;
      if( m_currWidgetUnderCursor && m_prevWidgetUnderCursor == m_currWidgetUnderCursor ) {
          if( m_currDragDropHereWidget ) {
            NE_ASSERT( m_currWidgetUnderCursor == m_currDragDropHereWidget, "" )( );
            m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::DragDropHere, pos, dragDropWidgetsToUse, m_modKeys) );
            m_currWidgetUnderCursor->DropsAcceptable(m_dragDropWidgetsAcceptable.begin(),
                                                     m_dragDropWidgetsAcceptable.end(),
                                                     pos);
          } else {
            m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::DragDropEnter, pos, dragDropWidgetsToUse, m_modKeys) );
            m_currWidgetUnderCursor->DropsAcceptable(m_dragDropWidgetsAcceptable.begin(),
                                                     m_dragDropWidgetsAcceptable.end(),
                                                     pos);
            m_currDragDropHereWidget = m_currWidgetUnderCursor;
          }
      }
    }
  } else if( m_dragWidgets[mouseButton]->IsResizable() ) { // send appropriate resize message to window
    Point offsetPos = pos + m_widgetResizeOffset;
    if( Widget* parent = m_dragWidgets[mouseButton]->GetParent() ) {
      offsetPos -= parent->GetUpperLeft(); //GetClientUpperLeft();
    }
    switch( m_widgetRegion ) {
      case WR_TOPLEFT:
        m_dragWidgets[mouseButton]->SizeMove(
          offsetPos,
          m_dragWidgets[mouseButton]->GetRelativeLowerRight());
        break;
      case WR_TOP:
        m_dragWidgets[mouseButton]->SizeMove(
          Point(m_dragWidgets[mouseButton]->GetRelativeUpperLeft().x, offsetPos.y),
          m_dragWidgets[mouseButton]->GetRelativeLowerRight());
        break;
      case WR_TOPRIGHT:
        m_dragWidgets[mouseButton]->SizeMove(
          Point(m_dragWidgets[mouseButton]->GetRelativeUpperLeft().x, offsetPos.y),
          Point(offsetPos.x, m_dragWidgets[mouseButton]->GetRelativeLowerRight().y));
        break;
      case WR_MIDLEFT:
        m_dragWidgets[mouseButton]->SizeMove(
          Point(offsetPos.x, m_dragWidgets[mouseButton]->GetRelativeUpperLeft().y),
          m_dragWidgets[mouseButton]->GetRelativeLowerRight());
        break;
      case WR_MIDRIGHT:
        m_dragWidgets[mouseButton]->SizeMove(
          m_dragWidgets[mouseButton]->GetRelativeUpperLeft(),
          Point(offsetPos.x, m_dragWidgets[mouseButton]->GetRelativeLowerRight().y));
        break;
      case WR_BOTTOMLEFT:
        m_dragWidgets[mouseButton]->SizeMove(
          Point(offsetPos.x, m_dragWidgets[mouseButton]->GetRelativeUpperLeft().y),
          Point(m_dragWidgets[mouseButton]->GetRelativeLowerRight().x, offsetPos.y));
        break;
      case WR_BOTTOM:
        m_dragWidgets[mouseButton]->SizeMove(
          m_dragWidgets[mouseButton]->GetRelativeUpperLeft(),
          Point(m_dragWidgets[mouseButton]->GetRelativeLowerRight().x, offsetPos.y));
        break;
      case WR_BOTTOMRIGHT:
        m_dragWidgets[mouseButton]->SizeMove(
          m_dragWidgets[mouseButton]->GetRelativeUpperLeft(),
          offsetPos);
        break;
      default:
          break;
    }
  }

  return (m_prevWidgetUnderCursor != NULL && m_prevWidgetUnderCursor->IsInteractive());
}


bool
GUIImpl::HandleRelease( GuiManager* mgr, uint32 mouseButton, const Point& pos, uint64 currTicks )
{
  m_lastButtonDownRepeatTime  = 0;
  m_prevWidgetDragPosition    = Point(0,0);
  m_prevWidgetUnderCursorTime = currTicks;

  Widget* clickWidget = m_dragWidgets[mouseButton];
  std::set<Widget*> ignores;
  if( m_currDragWidgetDragged ) {
    ignores.insert(clickWidget);
  }
  m_currWidgetUnderCursor = m_zlist.Pick(pos, mgr->GetModalWindow(), &ignores);

  bool inDragDrop = !m_dragDropWidgets.empty() ||
                    m_currDragWidgetDragged && 
                    clickWidget && 
                    clickWidget->GetDragDropDataType() != "" && 
                    mouseButton == 0;

  m_buttonState[mouseButton] = false;
  m_dragWidgets[mouseButton] = 0;              // if the mouse button is released, stop tracking the drag window.
  m_widgetRegion             = WR_NONE;        // and clear this, just in case

  // if the release is over the Widget where the button-down event occurred, and Widget has not been dragged
  if( !inDragDrop && clickWidget && m_currWidgetUnderCursor == clickWidget ) {
    // if this is second click over a window that just received an click within
    // the time limit -- it's a double-click, not a click
    if( m_doubleClickTime > 0 && m_doubleClickWidget == clickWidget &&
      m_doubleClickButton == mouseButton) {
      m_doubleClickWidget    = 0;
      m_doubleClickStartTime = -1;
      m_doubleClickTime      = -1;
      clickWidget->HandleEvent( WidgetEvent(WidgetEvent::DoubleClicked, pos, m_modKeys) );
    } else {
      if (m_doubleClickTime > 0) {
        m_doubleClickWidget    = 0;
        m_doubleClickStartTime = -1;
        m_doubleClickTime      = -1;
      } else {
        m_doubleClickStartTime = currTicks;
        m_doubleClickTime      = 0;
        m_doubleClickWidget    = clickWidget;
        m_doubleClickButton    = mouseButton;
      }
      clickWidget->HandleEvent( WidgetEvent(WidgetEvent::Clicked, pos, m_modKeys) );
    }
  } else {
    m_doubleClickWidget = 0;
    m_doubleClickTime = -1;
    if( clickWidget ) {
      clickWidget->HandleEvent( WidgetEvent(WidgetEvent::TouchUp, pos, m_modKeys) );
    }
    if( m_currWidgetUnderCursor ) {
      if( m_dragDropWidgets.empty() ) {
        if( clickWidget && clickWidget->GetDragDropDataType() != "" && mouseButton == 0 ) {
          m_dragDropOriginatingWidget = clickWidget->GetParent();
          m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::DragDropLeave) );
          m_currDragDropHereWidget = 0;
          m_dragDropWidgetsAcceptable[clickWidget] = false;
          m_currWidgetUnderCursor->DropsAcceptable( m_dragDropWidgetsAcceptable.begin(),
                                                    m_dragDropWidgetsAcceptable.end(),
                                                    pos );
          std::vector<Widget*>       acceptedWidgets;
          std::vector<const Widget*> unacceptedWidgets;
          if( m_dragDropWidgetsAcceptable[clickWidget] ) {
            acceptedWidgets.push_back(clickWidget);
          } else {
            unacceptedWidgets.push_back(clickWidget);
          }
          if( m_dragDropOriginatingWidget ) {
            m_dragDropOriginatingWidget->CancellingChildDragDrop( unacceptedWidgets );
            m_dragDropOriginatingWidget->ChildrenDraggedAway( acceptedWidgets, m_currWidgetUnderCursor );
          }
          if( !acceptedWidgets.empty() ) {
            m_currWidgetUnderCursor->AcceptDrops( acceptedWidgets, pos );
          }
        }
      } else {
        m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::DragDropLeave) );
        m_currDragDropHereWidget = 0;
        m_currWidgetUnderCursor->DropsAcceptable(m_dragDropWidgetsAcceptable.begin(),
                                                  m_dragDropWidgetsAcceptable.end(),
                                                  pos);
        std::vector<Widget*>       acceptedWidgets;
        std::vector<const Widget*> unacceptedWidgets;
        for( std::map<const Widget*, bool>::iterator it = m_dragDropWidgetsAcceptable.begin();
            it != m_dragDropWidgetsAcceptable.end();
            ++it ) {
          if (it->second) {
            acceptedWidgets.push_back(const_cast<Widget*>(it->first));
          } else {
            unacceptedWidgets.push_back(it->first);
          }
        }
        if( m_dragDropOriginatingWidget ) {
          m_dragDropOriginatingWidget->CancellingChildDragDrop( unacceptedWidgets );
          m_dragDropOriginatingWidget->ChildrenDraggedAway( acceptedWidgets, m_currWidgetUnderCursor );
        }
        if( !acceptedWidgets.empty() ) {
          m_currWidgetUnderCursor->AcceptDrops( acceptedWidgets, pos );
        }
      }
    }
  }
  m_dragDropOriginatingWidget = 0;
  m_prevWidgetUnderCursor     = m_currWidgetUnderCursor; // update this for the next time around
  m_currDragWidgetDragged     = false;
  m_currDragWidget            = 0;    
  m_dragDropWidgets.clear();
  m_dragDropWidgetsAcceptable.clear();

  return (m_prevWidgetUnderCursor != NULL && m_prevWidgetUnderCursor->IsInteractive());
}


GuiManager::GuiManager( FileArchivePtr fileSystem, RenderSystemPtr renderDevice )
///
/// Constructor
///
{
  NE_ASSERT(!m_impl,"")();

  m_impl.reset( new GUIImpl(fileSystem, renderDevice) );
}


GuiManager::~GuiManager()
///
/// Destructor.
///
{}


Widget* 
GuiManager::GetFocusWidget() const
{ 
  return m_impl->m_modalWidgets.empty() ? m_impl->m_focusWidget : m_impl->m_modalWidgets.back().second; 
}


Widget* 
GuiManager::GetWindowUnder( const Point& pt ) const
{
  return m_impl->m_zlist.Pick( pt, GetModalWindow() );
}


uint64 
GuiManager::GetDeltaT() const
{ 
  return m_impl->m_delta_t; 
}


uint64
GuiManager::GetTicks() const
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_impl->m_start).count();
}


//uint32 
//GuiManager::GetRenderingDragDropWidgets() const
//{ 
//  return s_impl->m_renderingDragDropWidgets; 
//}


uint64 
GuiManager::GetButtonDownRepeatDelay() const
{
  return m_impl->m_buttonDownRepeatDelay; 
}


uint64 
GuiManager::GetButtonDownRepeatInterval() const
{ 
  return m_impl->m_buttonDownRepeatInterval; 
}


uint64 
GuiManager::GetDoubleClickInterval() const
{ 
  return m_impl->m_doubleClickInterval; 
}


uint64 
GuiManager::GetMinDragTime() const
{ 
  return m_impl->m_minDragTime; 
}


uint64 
GuiManager::GetMinDragDistance() const
{ 
  return m_impl->m_minDragDistance; 
}


bool 
GuiManager::IsDragDropWidget( const Widget* widget ) const
{ 
  return m_impl->m_dragDropWidgets.find(const_cast<Widget*>(widget)) != m_impl->m_dragDropWidgets.end(); 
}


bool 
GuiManager::IsAcceptedDragDropWidget( const Widget* widget ) const
{
  std::map<const Widget*, bool>::const_iterator it = m_impl->m_dragDropWidgetsAcceptable.find( widget );
  return it != m_impl->m_dragDropWidgetsAcceptable.end() && it->second;
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
GuiManager::GetMousePosition() const
{
  return m_impl->m_mousePos; 
}


Nebulae::Point //@note symbol is ambiguous on iOS without qualification
GuiManager::GetMouseMovement() const
{
  return m_impl->m_mouseRel; 
}


const std::shared_ptr<WidgetFactory>& 
GuiManager::GetWidgetFactory() const
{ 
  return m_impl->m_widgetFactory; 
}


Widget*
GuiManager::Find( const char* name ) const
{
  return m_impl->m_zlist.Find( name );
}


const std::list<Widget*>& 
GuiManager::GetRegisteredWidgets() const
{
  return m_impl->m_zlist;
}
    

bool 
GuiManager::HandleEvents( EventType eventType, KeyCode key, uint32 key_code_point, Flags<ModKey> modKeys, const Point& pos, const Point& rel )
///
/// Responds to dispatched events.  
///
/// @param eventType
///   The type of event
///
/// @param key
///   The key that was pressed.
///
/// @param pos
///   The position of the mouse at time of event.
///
/// @return
///   Nothing.
///
{
  uint64 currTicks = GetTicks();

  // track double-click time and time-out any pending double-click that has outlived its interval
  if( m_impl->m_doubleClickTime >= 0 ) {
    m_impl->m_doubleClickTime = currTicks - m_impl->m_doubleClickStartTime;
    if( m_impl->m_doubleClickTime >= m_impl->m_doubleClickInterval ) {
      m_impl->m_doubleClickStartTime = -1;
      m_impl->m_doubleClickTime      = -1;
      m_impl->m_doubleClickWidget    = 0;
    }
  }

  switch( eventType ) {
    case IDLE: {
        if( (m_impl->m_currWidgetUnderCursor = CheckedGetWindowUnder(pos, modKeys)) != NULL ) {
          if( m_impl->m_buttonDownRepeatDelay && m_impl->m_currWidgetUnderCursor->RepeatButtonDown() &&
              m_impl->m_dragWidgets[0] == m_impl->m_currWidgetUnderCursor ) { // convert to a button-down message
            // ensure that the timing requirements are met
            if( currTicks - m_impl->m_prevButtonPressTime > m_impl->m_buttonDownRepeatDelay ) {
              if( !m_impl->m_lastButtonDownRepeatTime  ||
                currTicks - m_impl->m_lastButtonDownRepeatTime > m_impl->m_buttonDownRepeatInterval ) {
                m_impl->m_lastButtonDownRepeatTime = currTicks;
                m_impl->m_currWidgetUnderCursor->HandleEvent(WidgetEvent(WidgetEvent::TouchDown, pos, modKeys) );
              }
            }
          } else {
            //ProcessBrowseInfo();
          }
        }
        break;
      }

    case KEYPRESS: {
        if( GetFocusWidget() ) {
          GetFocusWidget()->HandleEvent( WidgetEvent(WidgetEvent::KeyPress, key, key_code_point, modKeys) );
        }
        break;
      }

    case KEYRELEASE: {
        if( GetFocusWidget() ) {
          GetFocusWidget()->HandleEvent( WidgetEvent(WidgetEvent::KeyRelease, key, key_code_point, modKeys) );
        }
        break;
      }

    case TOUCHDRAG: {
        m_impl->m_currWidgetUnderCursor = CheckedGetWindowUnder( pos, modKeys );

        m_impl->m_mousePos = pos; // record mouse position
        m_impl->m_mouseRel = rel; // record mouse movement

        if( m_impl->m_dragWidgets[0] || m_impl->m_dragWidgets[1] || m_impl->m_dragWidgets[2] ) {
          if( m_impl->m_dragWidgets[0] ) {
            m_impl->HandleDrag( this, 0, pos, currTicks );
          }
          if( m_impl->m_dragWidgets[1] ) {
            m_impl->HandleDrag( this, 1, pos, currTicks );
          }
          if( m_impl->m_dragWidgets[2] ) {
            m_impl->HandleDrag( this, 2, pos, currTicks );
          }
        } else if( m_impl->m_currWidgetUnderCursor && m_impl->m_prevWidgetUnderCursor == m_impl->m_currWidgetUnderCursor ) { // if !s_impl->m_drag_wnds[0] and we're moving over the same (valid) object we were during the last iteration
          m_impl->m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::TouchHere, pos, modKeys) );
        }
        if( m_impl->m_prevWidgetUnderCursor != m_impl->m_currWidgetUnderCursor ) {
          m_impl->m_prevWidgetUnderCursorTime = currTicks;
        }
        m_impl->m_prevWidgetUnderCursor = m_impl->m_currWidgetUnderCursor; // update this for the next time around
      
        return (m_impl->m_currWidgetUnderCursor != NULL);
      }

    case TOUCHDOWN:
      return m_impl->HandlePress( this, 0, pos, currTicks);

    case TOUCHUP:
      return m_impl->HandleRelease( this, 0, pos, currTicks );

    default:
      break;
  }

  return false;
}


void 
GuiManager::SetFocusWidget( Widget* widget )
{
  // inform old focus wnd that it is losing focus
  if( GetFocusWidget() ) {
    GetFocusWidget()->HandleEvent( WidgetEvent(WidgetEvent::LosingFocus) );
  }

  (m_impl->m_modalWidgets.empty() ? m_impl->m_focusWidget : m_impl->m_modalWidgets.back().second) = widget;

  // inform new focus wnd that it is gaining focus
  if( GetFocusWidget() ) {
    GetFocusWidget()->HandleEvent( WidgetEvent(WidgetEvent::GainingFocus) );
  }
}

  
void 
GuiManager::Register( Widget* widget )
{
	if( widget ) {
    m_impl->m_zlist.Add( widget );
  }
}

  
void
GuiManager::RegisterModal( Widget* widget )
{
  if( widget && widget->IsModal() ) {
    m_impl->m_modalWidgets.push_back( std::make_pair(widget, widget) );
    widget->HandleEvent( WidgetEvent(WidgetEvent::GainingFocus) );
  }
}


void 
GuiManager::Remove( Widget* widget )
{
  if( widget ) {
    if( m_impl->m_currWidgetUnderCursor == widget ) {
      m_impl->m_currWidgetUnderCursor = 0;
    }
    if( !m_impl->m_modalWidgets.empty() && m_impl->m_modalWidgets.back().first == widget ) { // if it's the current modal window, remove it from the modal list
      m_impl->m_modalWidgets.pop_back();
    } else { // if it's not a modal window, remove it from the z-order
      m_impl->m_zlist.Remove( widget );
    }
  }
}


void 
GuiManager::WidgetDying( Widget* widget )
{
  if( widget ) {
    Remove( widget );
    if( MatchesOrContains(widget, m_impl->m_focusWidget) ) {
      m_impl->m_focusWidget = 0;
    }
    for( std::list<std::pair<Widget*, Widget*> >::iterator it = m_impl->m_modalWidgets.begin(); it != m_impl->m_modalWidgets.end(); ++it ) {
      if( MatchesOrContains(widget, it->second) ) {
        if( MatchesOrContains(widget, it->first) ) {
          it->second = 0;
        } else { // if the modal window for the removed window's focus level is available, revert focus to the modal window
          if( (it->second = it->first) != NULL ) {
            it->first->HandleEvent( WidgetEvent(WidgetEvent::GainingFocus) );
          }
        }
      }
    }
    if( MatchesOrContains(widget, m_impl->m_prevWidgetUnderCursor) ) {
      m_impl->m_prevWidgetUnderCursor = 0;
    }
    if( MatchesOrContains(widget, m_impl->m_currWidgetUnderCursor) ) {
      m_impl->m_currWidgetUnderCursor = 0;
    }
    if( MatchesOrContains(widget, m_impl->m_dragWidgets[0])) {
      m_impl->m_dragWidgets[0] = 0;
      m_impl->m_widgetRegion   = WR_NONE;
    }
    if( MatchesOrContains(widget, m_impl->m_dragWidgets[1]) ) {
      m_impl->m_dragWidgets[1] = 0;
      m_impl->m_widgetRegion   = WR_NONE;
    }
    if( MatchesOrContains(widget, m_impl->m_dragWidgets[2]) ) {
      m_impl->m_dragWidgets[2] = 0;
      m_impl->m_widgetRegion   = WR_NONE;
    }
    if( MatchesOrContains(widget, m_impl->m_currDragDropHereWidget) ) {
      m_impl->m_currDragDropHereWidget = 0;
    }
    if( MatchesOrContains(widget, m_impl->m_dragDropOriginatingWidget) ) {
      m_impl->m_dragDropOriginatingWidget = 0;
    }
    m_impl->m_dragDropWidgets.erase( widget );
    m_impl->m_dragDropWidgetsAcceptable.erase( widget );
    if( MatchesOrContains(widget, m_impl->m_doubleClickWidget) ) {
      m_impl->m_doubleClickWidget    = 0;
      m_impl->m_doubleClickStartTime = -1;
      m_impl->m_doubleClickTime      = -1;
    }
  }
}


void 
GuiManager::MoveUp( Widget* widget )
{
  if( widget ) {
   m_impl->m_zlist.MoveUp( widget ); 
  }
}


void
GuiManager::MoveDown( Widget* widget )
{ 
  if( widget ) {
    m_impl->m_zlist.MoveDown( widget ); 
  }
} 


void 
GuiManager::RegisterDragDropWidget( Widget* widget, const Point& offset, Widget* originatingWidget )
{
  NE_ASSERT(widget,"")();
  if( !m_impl->m_dragDropWidgets.empty() && originatingWidget != m_impl->m_dragDropOriginatingWidget ) {
    throw std::runtime_error("GuiManager::RegisterDragDropWnd() : Attempted to register a drag drop item dragged from "
                             "one window, when another window already has items being dragged from it.");
  }
  m_impl->m_dragDropWidgets[widget]           = offset;
  m_impl->m_dragDropWidgetsAcceptable[widget] = false;
  m_impl->m_dragDropOriginatingWidget         = originatingWidget;
}


//void 
//GuiManager::EnableMouseButtonDownRepeat( uint32 delay, uint32 interval )
//{
//  if( !delay ) { // setting delay = 0 completely disables mouse drag repeat
//    s_impl->m_buttonDownRepeatDelay    = 0;
//    s_impl->m_buttonDownRepeatInterval = 0;
//  } else {
//    s_impl->m_buttonDownRepeatDelay    = delay;
//    s_impl->m_buttonDownRepeatInterval = interval;
//  }
//}


void 
GuiManager::SetDoubleClickInterval( uint32 interval )
{
  m_impl->m_doubleClickInterval = interval; 
}


void
GuiManager::SetMinDragTime( uint32 time )
{
  m_impl->m_minDragTime = time; 
}


void 
GuiManager::SetMinDragDistance( uint32 distance )
{
  m_impl->m_minDragDistance = distance; 
}


void
GuiManager::SetWidgetFactory( const std::shared_ptr<WidgetFactory>& factory )
{
  m_impl->m_widgetFactory = factory;
  if( !m_impl->m_widgetFactory ) {
    m_impl->m_widgetFactory.reset( new WidgetFactory() );
  }
}


std::shared_ptr<Font> 
GuiManager::GetFont( const std::string& filename, unsigned int pts ) 
{
  std::vector<std::shared_ptr<Font > >::const_iterator itr = m_impl->m_fonts.begin(), end = m_impl->m_fonts.end(); 
  for( ; itr != end; ++itr ) {
    if( (*itr)->FontName() == filename && (*itr)->PointSize() == pts ) {
      return *itr;
    }   
  }

  File* is = m_impl->m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, filename );
  if( is == NULL ) {
    return std::shared_ptr<Font >();
  }

  std::shared_ptr<Font > font( new Font(filename, pts) );
  font->Load( is );

  m_impl->m_fonts.push_back( font );
  return font;
}


Widget* 
GuiManager::GetModalWindow() const
{
  Widget* retval = 0;
  if( !m_impl->m_modalWidgets.empty() ) {
    retval = m_impl->m_modalWidgets.back().first;
  }
  return retval;
}


Widget*
GuiManager::CheckedGetWindowUnder( const Point& pt, Flags<ModKey> mod_keys )
{
  Widget* widget            = GetWindowUnder( pt );
  Widget* draggedWidget     = m_impl->m_currDragWidget;
  bool unregisteredDragDrop = draggedWidget && !draggedWidget->GetDragDropDataType().empty();
  bool registeredDragDrop   = !m_impl->m_dragDropWidgets.empty();
  std::map<Widget*, Point> dragDropWidgets;
  dragDropWidgets[draggedWidget] = m_impl->m_widgetDragOffset;
  if( m_impl->m_currDragDropHereWidget && !unregisteredDragDrop && !registeredDragDrop ) {
    m_impl->m_currDragDropHereWidget->HandleEvent( WidgetEvent(WidgetEvent::DragDropLeave) );
    m_impl->m_currDragDropHereWidget = 0;
  }
  if( widget != m_impl->m_currWidgetUnderCursor ) {
    if( m_impl->m_currWidgetUnderCursor ) {
      if( unregisteredDragDrop ) {
        m_impl->m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::DragDropLeave) );
        m_impl->m_dragDropWidgetsAcceptable[draggedWidget] = false;
        m_impl->m_currDragDropHereWidget = 0;
      } else if( registeredDragDrop ) {
        m_impl->m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::DragDropLeave) );
        for( std::map<const Widget*, bool>::iterator it = m_impl->m_dragDropWidgetsAcceptable.begin();
          it != m_impl->m_dragDropWidgetsAcceptable.end();
          ++it) {
          it->second = false;
        }
        m_impl->m_currDragDropHereWidget = 0;
      } else {
        m_impl->m_currWidgetUnderCursor->HandleEvent( WidgetEvent(WidgetEvent::TouchLeave) );
      }
    }
    if( widget ) {
      if( unregisteredDragDrop ) {
        widget->HandleEvent( WidgetEvent(WidgetEvent::DragDropEnter, pt, dragDropWidgets, mod_keys) );
        m_impl->m_dragDropWidgetsAcceptable[draggedWidget] = false;
        widget->DropsAcceptable( m_impl->m_dragDropWidgetsAcceptable.begin(),
                                 m_impl->m_dragDropWidgetsAcceptable.end(),
                                 pt );
        m_impl->m_currDragDropHereWidget = widget;
      } else if( registeredDragDrop ) {
        widget->HandleEvent( WidgetEvent(WidgetEvent::DragDropEnter, pt, m_impl->m_dragDropWidgets, mod_keys) );
        widget->DropsAcceptable( m_impl->m_dragDropWidgetsAcceptable.begin(),
                                 m_impl->m_dragDropWidgetsAcceptable.end(),
                                 pt);
        m_impl->m_currDragDropHereWidget = widget;
      } else {
        widget->HandleEvent( WidgetEvent(WidgetEvent::TouchEnter, pt, mod_keys) );
      }
    }
  }
  return widget;
}


bool 
Nebulae::MatchesOrContains( const Widget* lwidget, const Widget* rwidget )
{
  if( rwidget ) {
    for( const Widget* w = rwidget; w; w = w->GetParent() ) {
      if( w == lwidget ) {
        return true;
      }
    }
  } else if( rwidget == lwidget ) {
    return true;
  }
  return false;
}