#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Window/InputListener.h>

namespace Nebulae {

struct GUIImpl;

class FileSystem;
class Font;
class ModKey;
class RenderSystem;
class Widget;
class WidgetFactory;


class GuiManager
///
/// Encapsulates all the gui components
///
{
  friend struct GUIImpl;

public:
  enum EventType 
  {
    IDLE,         ///< nothing has changed since the last message, but the GUI might want to update some things anyway
    KEYPRESS,     ///< a down key press or key repeat, with or without modifiers like Alt, Ctrl, Meta, etc.
    KEYRELEASE,   ///< a key release, with or without modifiers like Alt, Ctrl, Meta, etc.
    TOUCHDOWN,    ///< a left mouse button press
    TOUCHUP,      ///< a left mouse button release
    TOUCHDRAG     ///< movement of the mouse; may include relative motion in addition to absolute position    
  };

public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;
  typedef std::shared_ptr<FileSystem >   FileArchivePtr;

private:
  std::shared_ptr<GUIImpl > m_impl; 

  public:
    /** \name Structors */ ///@{
    GuiManager( FileArchivePtr fileSystem, RenderSystemPtr renderSystem );
    virtual ~GuiManager();
    //@}

    
    /** \name Accessors */ ///@{
    const std::string& GetAppName() const;                                     ///< returns the user-defined name of the application
    Widget*            GetFocusWidget() const;                                 ///< returns the Widget that currently has the input focus
    Widget*            GetWindowUnder( const Point& pt ) const;                ///< returns the Widget under the point pt
    uint64             GetDeltaT() const;                                      ///< returns ms since last frame was rendered
    uint64             GetTicks() const;                                       ///< returns milliseconds since the app started running
    uint64             GetButtonDownRepeatDelay() const;                       ///< returns the \a delay value set by EnableMouseButtonDownRepeat()
    uint64             GetButtonDownRepeatInterval() const;                    ///< returns the \a interval value set by EnableMouseButtonDownRepeat()
    uint64             GetDoubleClickInterval() const;                         ///< returns the maximum interval allowed between clicks that is still considered a double-click, in ms
    uint64             GetMinDragTime() const;                                 ///< returns the minimum time (in ms) an item must be dragged before it is a valid drag
    uint64             GetMinDragDistance() const;                             ///< returns the minimum distance an item must be dragged before it is a valid drag
    bool               IsDragDropWidget( const Widget* widget ) const;         ///< returns true if \a Widget is currently begin dragged as part of a drag-and-drop operation
    bool               IsAcceptedDragDropWidget( const Widget* widget ) const; ///< returns true if \a Widget is currently begin dragged as part of a drag-and-drop operation, and it is over a drop target that will accept it
    Point              GetMousePosition() const;                               ///< returns the absolute position of mouse, based on the last mouse motion event
    Point              GetMouseMovement() const;                               ///< returns the relative position of mouse, based on the last mouse motion event
    const std::shared_ptr<WidgetFactory>& GetWidgetFactory() const;            ///< returns the current Widget factory.
    Widget*            Find( const char* name ) const;                         ///< returns a widget which has been given the specified name.
    const std::list<Widget*>& GetRegisteredWidgets() const;                    ///< returns a list of all of the Widgets.
    //@}


    /** \name Mutators */ ///@{
    /** Destroys the current contents of the controller returning it to its
        initial state. */
    void Clear();

    /** Event handler for user events. */
    bool HandleEvents( EventType eventType, KeyCode key, uint32 key_code_point,
      Flags<ModKey> mod_keys, const Point& pos, const Point& rel );

    /** sets the input focus window to \a widget.*/
    void SetFocusWidget( Widget* widget );

    /** Adds \a Widget into the z-list.  Registering a null pointer or registering 
        the same window multiple times is a no-op. */
    void Register( Widget* widget );

    /** adds \a Widget onto the modal windows "stack". */
    void RegisterModal( Widget* widget );

    /** Removes \a Widget from the z-list.  Removing a null pointer or removing the 
        same window multiple times is a no-op. */
    void Remove( Widget* widget );

    /** Removes \a Widget from all GUI state variables, so that none of them point
        to a deleted object. */
    void WidgetDying( Widget* widget );

    /** moves \a Widget to the top of the z-list. */
    void MoveUp( Widget* widget );

    /** moves \a Widget to the bottom of the z-list. */
    void MoveDown( Widget* widget );

    /** Adds \a Widget to the set of current drag-and-drop Widgets, to be rendered
        \a offset pixels from the cursor position. \a originating_Widget
        indicates the original owner of \a Widget before the drag-and-drop.
        \throw std::runtime_error May throw std::runtime_error if there are
        already other Widgets registered that belong to a window other than \a
        originating_widget. */
    void RegisterDragDropWidget( Widget* widget, const Point& offset, Widget* originating_widget );
    
    /** Clears the set of current drag-and-drop Widgets. */
    void CancelDragDrop();

    /** Delay and interval are in ms; Setting delay to 0 disables mouse 
        button-down repeating completely. */
    void EnableMouseButtonDownRepeat( uint32 delay, uint32 interval );

    /** Sets the maximum interval allowed between clicks that is still considered
        a double-click, in ms. */
    void SetDoubleClickInterval( uint32 interval );

    /** Sets the minimum time (in ms) an item must be dragged before it is a valid
        drag. */
    void SetMinDragTime( uint32 time );

    /** sets the minimum distance an item must be dragged before it is a valid
        drag. */
    void SetMinDragDistance( uint32 distance );

    /** sets the currently-installed style factory. */
    void SetWidgetFactory( const std::shared_ptr<WidgetFactory>& factory );

    /** Returns a shared_ptr to the desired font, supporting all printable
        ASCII characters. */
    std::shared_ptr<Font> GetFont( const std::string& filename, unsigned int pts );
    //@}


  private:
    /** \name Accessors */ ///@{
    // returns the current modal window, if any
    Widget* GetModalWindow() const;

    /** Returns the window under \a pt, sending Mouse{Enter|Leave} or
        DragDrop{Enter|Leave} as appropriate. */
    Widget* CheckedGetWindowUnder( const Point& pt, Flags<ModKey> modkeys );
    //@}

};

/** Returns true if lwidget == rwidget or if lwidget contains rwidget */
bool MatchesOrContains( const Widget* lwidget, const Widget* rwidget );

} //namespace Nebulae

#endif // __GUIMANAGER_H__