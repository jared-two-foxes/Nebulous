#ifndef __NEBULAE_GUI_WIDGETFACTORY_H__
#define __NEBULAE_GUI_WIDGETFACTORY_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Gui/Widget.h>
#include <Nebulae/Beta/Font/Font.h>

namespace Nebulae {

class SubTexture;

class ButtonControl;
class EditControl;
class ListBox;
class Slider;
class StaticGraphic;
class TextControl;

 
class WidgetFactory
{
  public:
    /** \name Structors */ ///@{
    /** Default ctor. */
    WidgetFactory(); 

    /** Virtual dtor. */
    virtual ~WidgetFactory(); 
    //@}

    /** \name Controls */ ///@{
    /** Returns a new Button. */
    virtual ButtonControl*        CreateButton( int32 x, int32 y, int32 width, int32 height, const std::string& str, const std::shared_ptr<Font>& font,
                                         Colour colour, Colour textColour = CLR_BLACK, uint32 flags = INTERACTIVE ) const;

    /** Returns a new StaticGraphic. */
    virtual StaticGraphic* CreateStaticGraphic( int32 x, int32 y, int32 width, int32 height, SubTexture* texture, uint32 flags = 0 ) const;
      

    /** Returns a new EditControl. */
    virtual EditControl*    CreateEditControl( int32 x, int32 y, int32 w, const std::string& str, const std::shared_ptr<Font >& font,
                                               Colour colour, Colour text_colour = CLR_BLACK, Colour interior = CLR_ZERO,
                                               uint32 flags = INTERACTIVE ) const;

    /** Returns a new ListBox. */
    virtual ListBox*       CreateListBox( int32 x, int32 y, int32 w, int32 h, Colour color, Colour interior = CLR_ZERO,
                                          uint32 flags = INTERACTIVE ) const;

    /** Returns a new Slider object. */
    virtual Slider*        CreateSlider( int32 x, int32 y, int32 w, int32 h, Real min, Real max/*, Orientation orientation,
                                         SliderLineStyle style, Clr color*/, int32 tab_width, int32 line_width = 5,
                                         uint32 flags = INTERACTIVE ) const;

    /** Returns a new TextControl. */
    virtual TextControl*   CreateTextControl( int32 x, int32 y, int32 w, int32 h, const std::string& str,
                                              const std::shared_ptr<Font>& font, Colour color = CLR_BLACK,
                                              Flags<TextFormat> format = FORMAT_NONE, uint32 flags = 0 ) const;

    /** Returns a new TextControl whose size is exactly that required to hold its text. */
    virtual TextControl*   CreateTextControl( int32 x, int32 y, const std::string& str, const std::shared_ptr<Font>& font,
                                              Colour color = CLR_BLACK, Flags<TextFormat> format = FORMAT_NONE,
                                              uint32 flags = 0 ) const;
    //@}


    /** \name Subcontrols */ ///@{
    /** Returns a new ListBox, to be used in a DropDownList. */
    //virtual ListBox*           NewDropDownListListBox(X x, Y y, X w, Y h, Clr color, Clr interior = CLR_ZERO,
    //                                                  Flags<WndFlag> flags = INTERACTIVE) const;

    /** Returns a new vertical Scroll, to be used in a ListBox. */
    //virtual Scroll*            NewListBoxVScroll(X x, Y y, X w, Y h, Clr color, Clr interior,
    //                                             Flags<WndFlag> flags = INTERACTIVE | REPEAT_BUTTON_DOWN) const;

    /** Returns a new horizontal Scroll, to be used in a ListBox. */
    //virtual Scroll*            NewListBoxHScroll(X x, Y y, X w, Y h, Clr color, Clr interior,
    //                                             Flags<WndFlag> flags = INTERACTIVE | REPEAT_BUTTON_DOWN) const;

    /** Returns a new tab Button, to be used in a horizontal Slider. */
    virtual ButtonControl*            CreateSliderTabButton( int32 x, int32 y, int32 w, int32 h, const std::string& str,
                                                      const std::shared_ptr<Font>& font, Colour color, Colour text_color = CLR_BLACK,
                                                      uint32 flags = INTERACTIVE ) const;
    //@}

    /** \name Wnds */ ///@{
    /** Returns a new TabWnd. */
    //virtual TabWidget*        CreateWidget( int32 x, int32 y, int32 width, int32 height ) const;
    //@}

    /** \name Dialogs */ ///@{
    /** Returns a new ThreeButtonDlg. */
    // virtual ThreeButtonDlg*    NewThreeButtonDlg(X x, Y y, X w, Y h, const std::string& msg,
    //                                              const std::shared_ptr<Font>& font, Clr color, Clr border_color,
    //                                              Clr button_color, Clr text_color, int32 buttons, const std::string& zero = "",
    //                                              const std::string& one = "", const std::string& two = "") const;

    /** Returns a new ThreeButtonDlg that automatically centers itself in
        the app. */
    // virtual ThreeButtonDlg*    NewThreeButtonDlg(X w, Y h, const std::string& msg, const std::shared_ptr<Font>& font,
    //                                              Clr color, Clr border_color, Clr button_color, Clr text_color, int32 buttons,
    //                                              const std::string& zero = "", const std::string& one = "",
    //                                              const std::string& two = "") const;
    //@}
      
};

}

#endif // __NEBULAE_GUI_WIDGETFACTORY_H__