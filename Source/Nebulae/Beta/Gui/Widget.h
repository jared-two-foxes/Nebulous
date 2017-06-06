#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/KeyCodes.h>

namespace Nebulae {
  
class ModKey;

class RenderSystem;
class WidgetEvent;
class WidgetFactory;

/** "Type" of a widget,  Used to different different widget subclasses for 
    rendering. */
enum WidgetType 
{
  WIDGET,
  CONTROL,
  BUTTON,
  STATIC,
  LABEL,
  EDIT,
  SLIDER,
  LISTBOX
};

/** "Regions" of a widget; used e.g. to determine direction(s) of drag when a
    window that has a drag-frame is clicked. */
enum WidgetRegion 
{
  WR_NONE = -1, 
  WR_TOPLEFT = 0, 
  WR_TOP, 
  WR_TOPRIGHT, 
  WR_MIDLEFT, 
  WR_MIDDLE, 
  WR_MIDRIGHT, 
  WR_BOTTOMLEFT, 
  WR_BOTTOM, 
  WR_BOTTOMRIGHT
};

/** The orientations for scrollbars, sliders, etc. */
enum Orientation
{
  VERTICAL,  ///< Vertical orientation.
  HORIZONTAL ///< Horizontal orientation.
};


enum WidgetFlag
{ 
  /** Clicks hit this window, rather than passing through it, and mouse-overs
    detect that they are over this window. */
  INTERACTIVE = 1,

  /** When a mouse button is held down over this window, it expects to receive
      multiple *ButtonDown messages. */
  REPEAT_BUTTON_DOWN = 2,

  /** This window can be dragged around independently. */
  DRAGABLE = 4,

  /** This window can be resized by the user, with the mouse. */
  RESIZABLE = 8,

  /** This windows is an "on-top" window, and will always appear above all
      non-on-top and non-modal windows.  Note that this only applies to
    top-level (Parent()-less) Wnds. */
  ONTOP = 16,

  /** This window is modal; while it is active, no other windows are
      interactive.  Modal windows are considered above "on-top" windows, and
      should not be flagged as OnTop.  Note that this only applies to top-level
      (Parent()-less) Wnds. */
  MODAL = 32
};


class Widget
/**
    <h3>Event Filters</h3>

    <br>Every Wnd can also have its incoming WndEvents filtered by an
    arbitrary number of other Wnds.  Each such Wnd in a Wnd's "filter chain"
    gets an opportunity, one at a time, to process an incoming WndEvent, or
    pass it on to the next filter in the chain.  If all EventFilter() calls in
    the chain return false, the filtered Wnd then gets the chance to process
    the WndEvent as normal.  Filter Wnds are traversed in reverse order that
    they are installed, and no filter Wnd can be in a filter chain more than
    once.  Installing the same filter Wnd more than once removes the Wnd from
    the filter chain and re-adds it to the beginning of the chain.  Note that
    the default implementation of EventFilter() is to return false and do
    nothing else, so installing a Wnd-derived type with no overridden
    EventFilter() in a filter Wnd will have no effect.  Also note that just as
    it is legal for keyboard accelerator slots to do nontrivial work and still
    return false (causing a keystroke event to be generated), EventFilter()
    may return false even when it does nontrivial work, and the next filter in
    the chain will also get a chance to process the WndEvent.  It is even
    possible to have an arbitrary number of filters that all do processing on
    an WndEvent, and finally let the filtered Wnd do its normal WndEvent
    processing.
**/
{
  friend class DepthList; ///< ZList needs access to \a m_zorder in order to order windows

public:
  
  /** The type of the iterator parameters passed to DropsAcceptable(). */
  typedef std::map<const Widget*, bool>::iterator DropsAcceptableIter;

protected:
  int                  m_id;               ///< Unique identifier.
  std::string          m_name;             ///< An optional Unique name. 
  Widget*              m_parent;           ///< The parent of THIS widget.
  std::list<Widget*>   m_children;         ///< List of any child widgets.
  Point                m_upperLeft;        ///< upper left corner of the widget
  Point                m_lowerRight;       ///< lower right corner of the widget.
  Point                m_minSize;          ///< Minimum window size Pt(0, 0) (= none) by default
  Point                m_maxSize;          ///< Maximum window size Pt(1 << 30, 1 << 30) (= none) by default    
  float                m_rotation;         ///< Angle of rotation.
  int                  m_zorder;           ///< Render depth.
  std::string          m_dragDropDataType; ///< The type of drag-and-drop data this Wnd represents, if any
  bool                 m_visible;          ///< Is the object currently visible.
  const WidgetFactory& m_widgetFactory;    ///< The style factory to use when creating dialogs or child controls
  uint32               m_flags;            ///< Flags supplied at window creation for clickability, dragability, resizability, etc.
  std::vector<Widget*> m_filters;          ///< The Wnds that are filtering this Wnd's events. These are in reverse order: top of the stack is back().
  std::set<Widget*>    m_filtering;        ///< The Wnds in whose filter chains this Wnd lies

  public:
    /** \name Structors */ ///@{
    virtual ~Widget();
    //@}

    /** \name Accessors */ ///@{
    /** Returns the unique identifier of this widget. */
    int GetID() const;

    /** Returns the name of this Wnd.  This name is not used internally in any way;
        it only exists for user convenience. */
    const std::string& GetName() const;

    /** Returns true iff a click over this window does not pass through.  Note
        that this also determines whether a mouse-over will detect this window
        or the ones under it. */
    bool IsInteractive() const;

    /** Returns true iff holding a mouse button down over this Wnd generates
        multiple button-down messages. */
    bool RepeatButtonDown() const;

    /** Returns true iff this Wnd be dragged by the user. */
    bool IsDragable() const;

    /** Returns true iff this Wnd can be resized by the user. */
    bool IsResizable() const;

    /** Returns true iff this Wnd is an on-top Wnd. */
    bool IsOnTop() const;

    /** Returns true iff this Wnd is a modal Wnd. */
    bool IsModal() const;

    /** Returns true iff this Wnd will be rendered if it is registered. */
    bool IsVisible() const;

    /** Returns the string key that defines the type of data that this Wnd
        represents in drag-and-drop drags.  Returns the empty string when this
        Wnd cannot be used in drag-and-drop. */
    const std::string& GetDragDropDataType() const;

    /** Sets the \a second member of each iterator to true or false,
        indicating whether the Wnd in the \a first member would be accepted if
        dropped on this Wnd at \a pt. */
    virtual void DropsAcceptable(DropsAcceptableIter first,
                                 DropsAcceptableIter last,
                                 const Point& pt) const;

    /** Returns the upper-left corner of window in \a screen \a coordinates
        (taking into account parent's screen position, if any) */
    Point GetUpperLeft() const;

    /** Returns (one pixel past) the lower-right corner of window in \a screen
        \a coordinates (taking into account parent's screen position, if
        any) */
    Point GetLowerRight() const;

    /** Returns the upper-left corner of window, relative to its parent's
        client area, or in screen coordinates if no parent exists. */
    Point GetRelativeUpperLeft() const;

    /** Returns (one pixel past) the lower-right corner of window, relative to
        its parent's client area, or in screen coordinates if no parent
        exists. */
    Point GetRelativeLowerRight() const;

    /** Returns the width of the widget. */
    int GetWidth() const;  

    /** Returns the height of the window. */
    int GetHeight() const; 

    /** Returns a point packed with the widgets width and height. */
    Point GetSize() const;

    /** Returns the minimum allowable size of window. */
    Point GetMinSize() const; 

    /** Returns the maximum allowable size of window. */
    Point GetMaxSize() const; 

    /** Returns the size of the minimum bounding box that can enclose the Wnd
        and still show all of its elements, plus enough room for interaction
        with those elements (if applicable).  For example, a TextControl's
        MinUsableSize() is just the area of its text, and a Scroll's
        MinUsableSize() is the combined sizes of its up-button, down-button,
        and tab (plus a bit of room in which to drag the tab). */
    virtual Point GetMinUsableSize() const;

    /** Returns upper-left corner of window's client area in screen
        coordinates (or of the entire area, if no client area is specified).
        Virtual because different windows have different shapes (and so ways
        of calculating client area). */
    virtual Point GetClientUpperLeft() const;

    /** Returns (one pixel past) lower-right corner of window's client area in
        screen coordinates (or of the entire area, if no client area is
        specified).  Virtual because different windows have different shapes
        (and so ways of calculating client area). */
    virtual Point GetClientLowerRight() const;

    /** Returns the size of the client area \see Size(). */
    Point GetClientSize() const;

    /** Returns the width of the client area. */
    int  GetClientWidth() const; 

    /** Returns the height of the client area.  */
    int  GetClientHeight() const;

    /** Returns \a pt translated from screen- to window-coordinates. */
    Point ScreenToWindow( const Point& pt ) const;

    /** Returns \a pt translated from screen- to client-coordinates. */
    Point ScreenToClient( const Point& pt ) const;

    /** Returns true if screen-coordinate point \a pt falls within the
        window. */
    virtual bool InWindow( const Point& pt ) const;

    /** Returns true if screen-coordinate point \a pt falls within the
        window's client area. */
    virtual bool InClient(const Point& pt) const;

    /** Returns child list; the list is const, but the children may be
        manipulated. */
    const std::list<Widget*>& GetChildren() const;

    /** Returns the window's parent (may be null). */
    Widget* GetParent() const;

    /** Returns the earliest ancestor window (may be null). */
    Widget* GetRootParent() const;

    /** Returns the position of this window in the z-order. Applies to root, non-child,
        windows only. */
    int GetDepth() const;

    /** Returns the currently-installed style factory if none exists, or the
        GUI-wide one otherwise. */
    const WidgetFactory& GetWidgetFactory() const;

    /** Returns the region under point \a pt. */
    virtual WidgetRegion GetWidgetRegion( const Point& pt ) const;

    /** Returns the Widget's type. */
    virtual WidgetType GetWidgetType() const { return WIDGET; }
    //@}

    /** \name Mutators */ ///@{
    /** Handles all messages, and calls appropriate function (LButtonDown(),
        LDrag(), etc.). */
    void HandleEvent( const WidgetEvent& event );

    /** Sets the string key that defines the type of data that this Widget
        represents in a drag-and-drop drag.  This should be set to the empty
        string when this Widget cannot be used in drag-and-drop. */
    void SetDragDropDataType( const std::string& dataType );

    /** Indicates to the Widget that a child Widget \a wnd is being dragged in a
        drag-and-drop operation, which gives it the opportunity to add other
        associated drag-and-drop Wnds (see GUI::RegisterDragDropWnd()).  \a
        offset indicates the position of the mouse relative to \a wnd's
        UpperLeft(). */
    virtual void StartingChildDragDrop( const Widget* widget, const Point& offset );

    /** When the user drops Wnds onto this Widget, DropsAcceptable() is passed
        the list of dropped Wnds.  The Wnds marked acceptable by
        DropsAcceptable() are then passed to AcceptDrops(), which handles the
        receipt of one or more drag-and-drop widgetss into this Widget. */
    virtual void AcceptDrops( const std::vector<Widget*>& widgets, const Point& pt );

    /** Handles the cancellation of the dragging of one or more child windows,
        whose dragging was established by the most recent call to
        StartingChildDragDrop().  Note that even if an accepting Widget accepts
        some but not all Wnds via DropsAcceptable(), this function will be
        called on those Wnds not accepted.  \note CancellingChildDragDrop()
        and ChildrenDraggedAway() are always called in that order, and are
        always called at the end of any drag-and-drop sequence performed on a
        child of this Widget, whether the drag-and-drop is successful or not. */
    virtual void CancellingChildDragDrop( const std::vector<const Widget*>& widget );

    /** Handles the removal of one or more child windows that have been
        dropped onto another window which has accepted them as drops via
        DropsAcceptable().  The accepting window retains ownership, so this
        function must not delete the children.  \note
        CancellingChildDragDrop() and ChildrenDraggedAway() are always called
        in that order, and are always called at the end of any drag-and-drop
        sequence performed on a child of this Widget, whether the drag-and-drop
        is successful or not. */
    virtual void ChildrenDraggedAway( const std::vector<Widget*>& widgets,
                                     const Widget* destination );

    /** Sets a name for this Widget.  This name is not used by GG in any way; it
        only exists for user convenience. */
    void SetName( const std::string& name );

    /** Suppresses rendering of this window (and possibly its children) during
        render loop. */
    void Hide( bool children = true );

    /** Enables rendering of this window (and possibly its children) during
        render loop. */
    void Show( bool children = true );

    /** Moves upper-left corner of window to \a pt. */
    void MoveTo( const Point& pt );     

    /** Moves window by \a pt pixels. */
    void OffsetMove( const Point& pt ); 

    /** Resizes and/or moves window to new upper-left and lower right
        boundaries. */
    virtual void SizeMove( const Point& ul, const Point& lr );

    /** Resizes window without moving upper-left corner. */
    void Resize( const Point& size );

    /** Sets the minimum allowable size of window \a pt. */
    void SetMinSize( const Point& sz );

    /** Sets the maximum allowable size of window \a pt. */
    void SetMaxSize( const Point& sz );

    /** Places \a wnd in child ptr list, sets's child's \a m_parent member to
    \a this. */
    void AttachChild( Widget* widget );

    /** Places \a wnd at the end of the child ptr list, so it is rendered last
        (on top of the other children). */
    void MoveChildUp( Widget* widget );

    /** Places \a wnd at the beginning of the child ptr list, so it is
        rendered first (below the other children). */
    void MoveChildDown( Widget* widget );

    /** Removes \a wnd from child ptr list, sets child's m_parent = 0. */
    void DetachChild( Widget* widget );

    /** Removes all Wnds from child ptr list, sets childrens' m_parent = 0. */
    void DetachChildren();

    /** Removes, detaches, and deletes \a wnd; does nothing if wnd is not in
        the child list. */
    void DeleteChild( Widget* widget );

    /** Removes, detaches, and deletes all Wnds in the child list. */
    void DeleteChildren();

    /** Adds \a widget to the front of the event filtering chain. */
    void InstallEventFilter( Widget* widget );

    /** Removes \a widget from the filter chain. */
    void RemoveEventFilter( Widget* widget );
    //@}


  protected:

    /** \name Structors */ ///@{
    /** Ctor that allows a size and position to be specified, as well as
      creation flags. */
    Widget( const WidgetFactory& factory, int x, int y, int width, int height, uint32 flags = INTERACTIVE | DRAGABLE );
    //@}

    /** \name Mutators */ ///@{
    /** Respond to left button down msg.  A window receives this whenever any
        input device button changes from up to down while over the window.
        \note If this Wnd was created with the REPEAT_BUTTON_DOWN flag, this
        method may be called multiple times during a single button
        press-release cycle.  \see GG::GUI */
    virtual void TouchDown( const Point& pt, Flags<ModKey> mod_keys );

    /** Respond to left button drag msg (even if this Wnd is not dragable).
        Drag messages are only sent to the window over which the button was
        pressed at the beginning of the drag. A window receives this whenever
        any input device button is down and the cursor is moving while over
        the window.  The window will also receive drag messages when the mouse
        is being dragged outside the window's area. */
    virtual void TouchDrag( const Point& pt, const Point& move, Flags<ModKey> mod_keys );

    /** Respond to release of left mouse button outside this Wnd, if it was
        originally depressed over this Wnd.  A Wnd will receive an LButtonUp()
        message whenever a drag that started over its area ends, even if the
        cursor is not currently over the window when this happens. */
    virtual void TouchUp( const Point& pt, Flags<ModKey> mod_keys );

    /** Respond to release of left mouse button over this Wnd, if it was also
        originally depressed over this Wnd.  A Wnd will receive an LButtonUp()
        message whenever a drag that started over its area ends over its area
        as well. */
    virtual void Clicked( const Point& pt, Flags<ModKey> mod_keys );

    /** Respond to the cursor entering the Wnd's coords while dragging
        drag-and-drop Wnds.  The Pts in \a drag_drop_wnds are the Wnds'
        offsets from \a pt. */
    virtual void DragDropEnter( const Point& pt, const std::map<Widget*, Point>& dragDropWidgets,
                                Flags<ModKey> modKeys );

    /** Respond to cursor moving about within the Wnd, or to cursor lingering
        within the Wnd for a long period of time, while dragging drag-and-drop
        Wnds.  A DragDropHere() message will not be generated the first time
        the cursor enters the window's area.  In that case, a DragDropEnter()
        message is generated The Pts in \a drag_drop_wnds are the Wnds'
        offsets from \a pt. */
    virtual void DragDropHere( const Point& pt, const std::map<Widget*, Point>& dragDropWidget,
                               Flags<ModKey> modKeys );

    /** Respond to cursor leaving the Wnd's bounds while dragging
        drag-and-drop Wnds. */
    virtual void DragDropLeave();

    /** Respond to down-keystrokes (focus window only).  A window may receive
        KeyPress() messages passed up to it from its children.  For instance,
        Control-derived classes pass KeyPress() messages to their Parent()
        windows by default.  \note Though mouse clicks consist of a press and
        a release, all Control classes by default respond immediately to
        KeyPress(), not KeyRelease(); in fact, by default no Wnd class does
        anything at all on a KeyRelease event.  \note \a key_code_point will
        be zero if Unicode support is unavailable. */
    virtual void KeyPress( KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys );

    /** Respond to up-keystrokes (focus window only).  A window may receive
        KeyRelease() messages passed up to it from its children.  For
        instance, Control-derived classes pass KeyRelease() messages to their
        Parent() windows by default.  \note \a key_code_point will be zero if
        Unicode support is unavailable. */
    virtual void KeyRelease( KeyCode key, uint32 key_code_point, Flags<ModKey> mod_keys );

    /** Handles an WidgetEvent destined for Widget \a w, but which this Widget
        is allowed to handle first.  Returns true if this filter processed the
        message. */
    virtual bool EventFilter( Widget* w, const WidgetEvent& event );

    /** Sends the current event to Parent() for processing, if Parent() is
        non-null. */
    void ForwardEventToParent();
    //@}

}; 

} // namespace Nebulae

#endif // __WIDGET_H__