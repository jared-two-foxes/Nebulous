// -*- C++ -*-

/** \file WidgetEvent.h \brief Contains the WidgetEvent class, which is used
    internally to dispatch event messages to Widgets. */

#ifndef NEBUALE_GUI_WidgetEvent_h_
#define NEBUALE_GUI_WidgetEvent_h_

#include <Nebulae/Common/Common.h> //#include <GG/Base.h>
#include <Nebulae/Common/Window/KeyCodes.h>
//#include <GG/Flags.h>

#include <map>


namespace Nebulae {

class Widget;

/** \brief Encapsulates a Widget event that is passed from the controller to a
    Widget.

    The various types of WidgetEvents correspond to the various message member
    functions of Widget, some of which have different parameterizations.  Rather
    than have a less-efficient but more-easily-extensible hierarchy of
    WidgetEvent types, a single WidgetEvent type exists that has all possible
    parameters to a Widget message function call.  Therefore, not all of
    WidgetEvent's accessors will return sensical results, depending on the
    EventType of the WidgetEvent.   */
class WidgetEvent
{
public:
  /** The types of Wnd events.  Each of these corresponds to a Wnd member
      function of the same name. */
  enum EventType {
    TouchDown,
    TouchDrag,
    TouchUp,
    Clicked,
    DoubleClicked,
    TouchEnter,
    TouchHere,
    TouchLeave,
    DragDropEnter,
    DragDropHere,
    DragDropLeave,
    KeyPress,
    KeyRelease,
    GainingFocus,
    LosingFocus
  };

private:
  EventType                 m_type;
  Point                     m_point;
  KeyCode                   m_key;
  uint32                    m_keyCodePoint;
  Flags<ModKey>             m_modKeys;
  Point                     m_dragMove;
  std::map<Widget*, Point > m_dragDropWidgets;


  public:
    /** Constructs an WidgetEvent that is used to invoke a function taking
        parameters (const GG::Pt& pt, Flags<ModKey> mod_keys), eg
        TouchDown(). */
    WidgetEvent( EventType type, const Point& pt, Flags<ModKey> mod_keys );

    /** Constructs an WidgetEvent that is used to invoke a function taking
        parameters (const Pt& pt, const Pt& move, Flags<ModKey> mod_keys), eg
        TouchDragged(). */
    WidgetEvent( EventType type, const Point& pt, const Point& move, Flags<ModKey> mod_keys );

    /** Constructs an WidgetEvent that is used to invoke a function taking
        parameters (const Pt& pt, const std::map<Wnd*, Pt>& drag_drop_wnds,
        Flags<ModKey> mod_keys), eg DragDropEnter(). */
    WidgetEvent( EventType type, const Point& pt, const std::map<Widget*, Point>& dragDropWidgets, Flags<ModKey> modKeys );

    /** Constructs an WidgetEvent that is used to invoke a function taking
        parameters (Key key, Flags<ModKey> mod_keys), eg KeyPress(). */
    WidgetEvent( EventType type, KeyCode key, uint32 code_point, Flags<ModKey> mod_keys );

    /** Constructs an WidgetEvent that is used to invoke a function taking no
        parameters, eg GainingFocus(). */
    explicit WidgetEvent( EventType type );

    EventType                       GetType() const;            ///< returns the type of the WidgetEvent
    const Point&                    GetPoint() const;           ///< returns the point at which the event took place, if any
    KeyCode                         GetKey() const;             ///< returns the key pressed or released in the WidgetEvent, if any
    uint32                          GetKeyCodePoint() const;    ///< returns the Unicode code point for the key pressed or released in the WidgetEvent, if any.  \note This may be zero, even in a KeyPress or KeyRelease event, if Unicode support is unavailable.
    Flags<ModKey>                   GetModKeys() const;         ///< returns the modifiers to the WidgetEvent's keypress, if any
    const Point&                    GetDragMove() const;        ///< returns the amount of drag movement represented by the WidgetEvent, if any
    const std::map<Widget*, Point>& GetDragDropWidgets() const; ///< returns the drag-and-drop wnds represented by the WidgetEvent, if any

};

} // namespace GG

#endif
