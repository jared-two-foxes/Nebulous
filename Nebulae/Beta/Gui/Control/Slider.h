
/** \file  Slider.h 
 *  \brief Contains the Slider widget class deceleration.
 */

#ifndef __NEBULAE_CONTROL_SLIDER_H__
#define __NEBULAE_CONTROL_SLIDER_H__

#include <Nebulae/Beta/Gui/Control/ButtonControl.h>
#include <boost/signals2.hpp>


namespace Nebulae
{

  /** The rendering styles of the line the tab slides over in a Slider. */
  enum SliderLineStyle 
  {
      FLAT,
      RAISED,
      GROOVED
  };

  /** \brief A slider control.

    This control class is templated so that arbitrary data types can be used
    with Slider.  All the built-in numeric types are supported by the code
    here.  If you want to use some other type, such as an enum type, you need
    to define operator+(), and operator-().  

    This control allows the user to drag a tab to a desired setting; it is 
    somewhat like a Scroll.  Sliders can be either vertical or horizontal, but
    cannot switch between the two.  Unlike vertical Scrolls, whose values 
    increase downward, vertical Sliders increase upward by default.  Note that
    it is acceptible to define a range that increases from min to max, or one 
    that decreases from min to max; both are legal. */
  class Slider : public Control
  {
  public:
    /** \name Signal Types */ ///@{
    typedef boost::signals2::signal<void (Real, Real, Real)> SlidSignalType;           ///< emitted whenever the slider is moved; the tab position and the upper and lower bounds of the slider's range are indicated, respectively
    typedef boost::signals2::signal<void (Real, Real, Real)> SlidAndStoppedSignalType; ///< emitted when the slider's tab is stopped after being dragged, the slider is adjusted using the keyboard, or the slider is moved programmatically; the tab position and the upper and lower bounds of the slider's range are indicated, respectively
    //@}

  private:
    Real            m_posn;
    Real            m_rangeMin;
    Real            m_rangeMax;
    Real            m_pageSize;
    uint32          m_lineWidth;
    uint32          m_tabWidth;
    SliderLineStyle m_lineStyle;
    int             m_tabDragOffset;
    ButtonControl*  m_tab;
    bool            m_draggingTab;
      
    public:
      /** \name Structors */ ///@{
      Slider( const WidgetFactory& factory, int x, int y, int w, int h, Real min, Real max,
              /*Orientation orientation, SliderLineStyle style, Clr color,*/
              uint32 tab_width, uint32 line_width = 5, uint32 flags = INTERACTIVE );
      //@}

      /** \name Accessors */ ///@{
      virtual Point         GetMinUsableSize() const;

      /** Returns the current tab position. */
      Real                  GetPosn() const;       

      /** Returns the defined possible range of control. */
      std::pair<Real, Real> GetSliderRange() const;

      /** Returns the current page size, or the amount that the slider
          increments/decrements when a click occurs off of the tab.  If not set,
          this defaults to 10% of the slider's range. */
      Real                  GetPageSize() const;

      /** Returns the width of the slider's tab, in pixels. */
      uint32                GetTabWidth() const;       

      /** Returns the width of the line along which the tab slides, in pixels. */
      uint32                GetLineWidth() const;      

      /** Returns the style of line used to render the control. */
      SliderLineStyle       GetLineStyle() const;    

      /** Returns the WidgetType enum that specifies the Slider control type. */
      virtual WidgetType    GetWidgetType() const { return SLIDER; }

      /** Returns true when the slider tab is being moved. */
      bool                  IsTabBeingDragged() const;

      /** Returns a pointer to the Button used as this control's sliding tab. */
      ButtonControl*        GetTab() const;                  

      mutable SlidSignalType           SlidSignal;           ///< returns the slid signal object for this Slider
      mutable SlidAndStoppedSignalType SlidAndStoppedSignal; ///< returns the slid-and-stopped signal object for this Slider
      //@}

      /** \name Mutators */ ///@{
      virtual void          SizeMove( const Point& ul, const Point& lr ) override;
      virtual void          SetDisabled( bool b = true ) override;
      virtual void          SetColour( const Colour& c ) override;

      void                  SizeSlider(Real min, Real max); ///< sets the logical range of the control; \a min must not equal \a max
      void                  SetMax(Real max);               ///< sets the maximum value of the control
      void                  SetMin(Real min);               ///< sets the minimum value of the control
      void                  SlideTo(Real p);                ///< slides the control to a certain spot

    protected:

      /** \name Structors */ ///@{
      Slider(); ///< default ctor
      //@}

      /** \name Accessors */ ///@{
      Real    PtToPosn(const Point& pt) const; ///< maps an arbitrary screen point to its nearest logical slider position
      //@}

      /** \name Mutators */ ///@{
      virtual void Clicked( const Point& pt, Flags<ModKey> mod_keys ) override;
      virtual void KeyPress( KeyCode key, uint32 key_code_pt, Flags<ModKey> mod_keys ) override;

      virtual bool EventFilter( Widget* w, const WidgetEvent& event ) override;

      void MoveTabToPosn(); ///< moves the tab to the current logical position
      //@}
      
    private:
      void SlideToImpl(Real p, bool signal);
      void UpdatePosn();

  }; 

} 

#endif // __NEBULAE_CONTROL_SLIDER_H__