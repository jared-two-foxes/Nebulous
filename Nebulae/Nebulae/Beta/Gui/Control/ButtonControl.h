
/** \file Button.h 
 *  \brief Contains the Button class.
 */

#ifndef __NEBULAE_BUTTON_H__
#define __NEBULAE_BUTTON_H__

#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <boost/signals2.hpp>

namespace Nebulae {
  
class SubTexture;

class ButtonControl : public TextControl
///
/// A simple button control that has different visual states depending upon whether the control
/// is currently being pressed or not.  When the button is "clicked" it will respond by invoking
/// an optional boost::function object.
///
{
public:

  /// the states of being for a Nebulae::Button
  enum ButtonState
  {
    BN_PRESSED,    ///< The button is being pressed by the user, and the cursor is over the button
    BN_UNPRESSED,  ///< The button is unpressed
    BN_ROLLOVER    ///< The button has the cursor over it, but is unpressed
  };

public:
  /** \name Signal Types */ ///@{
  typedef boost::signals2::signal<void ()> ClickedSignalType; ///< Emitted when the button is clicked by the user
  //@}

protected:
  ButtonState              m_buttonState;      ///< Current button state.
  const SubTexture*        m_unpressedGraphic; ///< Visual appearance when not pressed.
  const SubTexture*        m_pressedGraphic;   ///< Visual appearance when pressed.
    
public:
  /** \name Structors */ ///@{
  ButtonControl( const WidgetFactory& factory, int x, int y, int w, int h, const std::string& str, 
    const std::shared_ptr<Font>& font, Colour color, Colour text_color = CLR_BLACK,
    uint32 flags = INTERACTIVE );
  virtual ~ButtonControl();
  //@}

  /** \name Accessors */ ///@{
  /** Returns button state \see ButtonState. */
  ButtonState               GetState() const;
  const SubTexture*         GetUnpressedGraphic() const;
  const SubTexture*         GetPressedGraphic() const;
  virtual WidgetType        GetWidgetType() const;

  mutable ClickedSignalType ClickedSignal; ///< The clicked signal object for this Button
  //@}

  /** \name Mutators */ ///@{
  void SetState( ButtonState state );                     /** Sets button state programmatically \see ButtonState. */
  void SetUnpressedGraphic( const SubTexture& texture );  /** Sets the Texture to be used as the image of the button when unpressed. */
  void SetPressedGraphic( const SubTexture& texture );    /** Sets the Texture to be used as the image of the button when pressed. */
  //@}

protected:

  /** \name Mutators */ ///@{
  virtual void TouchDown( const Point& pt, Flags<ModKey> mod_keys );
  virtual void TouchDrag( const Point& pt, Flags<ModKey> mod_keys );
  virtual void TouchUp( const Point& pt, Flags<ModKey> mod_keys );
  virtual void Clicked( const Point& pt, Flags<ModKey> mod_keys ); 
  //@}
    
}; 

} 

#endif // __STATICGRAPHIC_H__