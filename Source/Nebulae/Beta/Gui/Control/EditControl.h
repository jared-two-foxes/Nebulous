// -*- C++ -*-
/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */

/** \file Edit.h \brief Contains the Edit class, a single-line editable
    text-box control. */

#ifndef _NE_Edit_h_
#define _NE_Edit_h_

#include <Nebulae/Beta/Gui/Control/TextControl.h>
#include <boost/signals2.hpp>

namespace Nebulae {

/** \brief This is a single-line edit box control.

    Note that the constructor determines the height of the control based on
    the height of the font used and the value of the constant PIXEL_MARGIN.
    There are two types of signals emitted by an Edit control.  The first is
    EditedSignal; this is emitted every time the contents of the Edit change.
    Sometimes, however, you don't want that.  For instance, say you want to
    keep the value of the text in the Edit to between (numerical values) 100
    and 300.  If the Edit currently reads "200", the user may decide to
    highlight the "2", hit delete, then type a "1".  If updates are immediate,
    you will receive notification that the Edit says "00" (an invalid value),
    when that is just a temporary value you don't care about.  In such
    situations, the other signal, FocusUpdateSignal, should be useful.  It is
    only emitted when the Edit has changed and is losing focus, or one of
    enter or return is pressed.  So you would only receive a single update,
    namely "100", which is a valid number for that control, and you would
    receive it only when it is certain that the user is finished editing the
    text (when the focus changes).  Note that both signals may be used at the
    same time, if desired. */
class EditControl : public TextControl
{
public:
  /** \name Signal Types */ ///@{
  /** Emitted whenever the text of the Edit are altered (keypresses,
      deletes, etc.); provides the new text of the Edit. */
  typedef boost::signals2::signal<void (const std::string&)> EditedSignalType;

  /** Emitted whenever the Edit has changed and has lost the input focus, or
      one of enter or return has been pressed; provides the new text of the
      Edit. */
  typedef boost::signals2::signal<void (const std::string&)> FocusUpdateSignalType;
  //@}

public:
  /** The number of pixels to leave between the text and the control's
      frame. */
  static const int PIXEL_MARGIN;

private:
  /** If .first == .second, the caret is drawn before character at
      m_cursor_pos.first; otherwise, the range is selected (when range is
      selected, caret is considered at .second) */
  std::pair<CPSize, CPSize> m_cursor_pos;

  CPSize       m_first_char_shown; ///< Index of the first character on the left end of the control's viewable area
  Colour       m_int_color;        ///< Color of background inside text box
  Colour       m_hilite_color;     ///< Color behind selected range
  Colour       m_sel_text_color;   ///< Color of selected text

  bool         m_recently_edited;  ///< The contents when the focus was last gained

public:
    /** \name Structors */ ///@{
    /** Ctor. Height is determined from the font and point size used. */
    EditControl( const WidgetFactory& factory, int x, int y, int w, const std::string& str, 
      const std::shared_ptr<Font >& font, Colour color, Colour text_color = CLR_BLACK, 
      Colour interior = CLR_ZERO, uint32 flags = INTERACTIVE);
    //@}

    /** \name Accessors */ ///@{
    virtual WidgetType GetWidgetType() const { return EDIT; }

    virtual Point MinUsableSize() const;
    virtual Point ClientUpperLeft() const;
    virtual Point ClientLowerRight() const;

    /** Returns the current position of the cursor (first selected character
        to one past the last selected one). */
    const std::pair<CPSize, CPSize>& CursorPosn() const;

    /** Returns the color used to render the iterior of the control. */
    Colour InteriorColour() const;

    /** Returns the color used to render hiliting around selected text. */
    Colour HiliteColour() const;

    /** Returns the color used to render selected text. */
    Colour SelectedTextColour() const;

    /** The edited signal object for this Edit. */
    mutable EditedSignalType EditedSignal;

    /** The focus update signal object for this Edit. */
    mutable FocusUpdateSignalType FocusUpdateSignal;
    //@}

    /** \name Mutators */ ///@{
    virtual void SetColour(Colour c);

    /** Sets the interior color of the control. */
    void SetInteriorColour(Colour c);

    /** Sets the color used to render hiliting around selected text. */
    void SetHiliteColour(Colour c);

    /** Sets the color used to render selected text. */
    void SetSelectedTextColour(Colour c);

    /** Selects all text in the given range.  When \a from == \a to, this
        function just places the caret at \a from.  Note that it is legal to
        pass values such that \a to < \a from.  The difference is that \a from
        < \a to simulates a drag-selection from left to right, and \a to < \a
        from simulates one from right to left.  The direction of the simulated
        drag affects which part of the text is visible at the end of the
        function call. */
    void SelectRange(CPSize from, CPSize to);

    /** Selects all text in the entire control.  This function leaves the
        beginning of the text in view; see SelectRange(). */
    void SelectAll();

    virtual void SetText(const std::string& str) override;

    /** Returns true if >= 1 characters selected. */
    virtual bool MultiSelected() const;

    /** Returns the index of the first character visible in the Edit. */
    CPSize FirstCharShown() const;

    /** Returns the index of the code point \a x pixels from left edge of
        visible portion of string. */
    CPSize CharIndexOf(int x) const;

    /** Returns the distance from the beginning of the string to just before
        the first visible character. */
    int FirstCharOffset() const;

    /** Returns the last visible char (\a not one past the last visible
        char). */
    CPSize LastVisibleChar() const;
    //@}

protected:
    /** \name Accessors */ ///@{
    /** Returns true iff the contents have been changed since the last time
        the focus was gained. */
    bool RecentlyEdited() const;

    /** Returns the screen x-coordinate of the left side of the code point at
        index \a idx. */
    int ScreenPosOfChar(CPSize idx) const;

    /** Returns the value of GUI::Ticks() at the last left button press. */
    unsigned int LastButtonDownTime() const;
    //@}

    /** \name Mutators */ ///@{
    virtual void TouchDown(const Point& point, Flags<ModKey> mod_keys) override;
    virtual void TouchDrag(const Point& point, const Point& move, Flags<ModKey> mod_keys) override;
    virtual void KeyPress(KeyCode key, boost::uint32_t keyCodePoint, Flags<ModKey> mod_keys) override;
    virtual void GainingFocus();
    virtual void LosingFocus();
    //@}

private:
    void ClearSelected(); ///< Clears (deletes) selected characters, as when a del, backspace, or character is entered
    void AdjustView();    ///< Makes sure the caret ends up in view after an arbitrary move

};

void GetTranslatedCodePoint( KeyCode key, uint32 keyCodePoint,
                             Flags<ModKey> mod_keys,
                             std::string& translated_code_point );

} // namespace GG

#endif
