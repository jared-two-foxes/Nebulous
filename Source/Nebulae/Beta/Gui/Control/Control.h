/**
 *  \file Control.h
 *  \brief Contains the Control class, the base class for all Gui controls.
 */

#ifndef NEBULAE_BETA_GUI_CONTROL_CONTROL_H_
#define NEBULAE_BETA_GUI_CONTROL_CONTROL_H_

#include <Nebulae/Beta/Gui/Widget.h>

namespace Nebulae
{

class Control : public Widget
///
/// Simple extension of a Widget which acts as the root class for any user
/// controls.
///
/// @todo [jared.watt 08-11-2013]
///   Is it possible to merge this functionality into the Widget class?
///   Does it make sense not to?
///
{
protected:
  bool m_disabled; ///< Indicates if the control can be interacted with.
  Colour m_colour; ///< The color of the control

public:
  /** \name Structors */ ///@{
  Control( const WidgetFactory& factory, int x, int y, int w, int h, uint32 flags = INTERACTIVE );
  ~Control() override;
  //@}

  /** \name Accessors */ ///@{
  /** Returns the widget type of this widget. */
  WidgetType GetWidgetType() const override { return CONTROL; }

  void DropsAcceptable( DropsAcceptableIter first, DropsAcceptableIter last, const Point& pt ) const override;

  /* Returns the color of the control. */
  const Colour& GetColour() const;

  /** Returns true if the control is disabled, false otherwise. */
  bool IsDisabled() const;
  //@}

  /** \name Mutators */ ///@{
  void AcceptDrops( const std::vector<Widget*>& widgets, const Point& pt ) override;

  /** Sets the color of the control. */
  virtual void SetColour( const Colour& c );

  /** disables/enables the control; disabled controls appear greyed. */
  virtual void SetDisabled( bool bDisabled = true );
  //@}

protected:
  /** \name Mutators */ ///@{
  // virtual void MouseWheel( const Point& pt, int move, Flags<ModKey> modKeys );
  void KeyPress( KeyCode key, uint32 keyCodePoint, Flags<ModKey> modKeys ) override;
  void KeyRelease( KeyCode key, uint32 keyCodePoint, Flags<ModKey> modKeys ) override;
  //@}

}; // Control

} // namespace Nebulae

#endif // NEBULAE_BETA_GUI_CONTROL_CONTROL_H_