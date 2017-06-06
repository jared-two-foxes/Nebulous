
/** \file TextControl.h  \brief Contains the TextControl class, a control which
    represents a certain text string in a certain font, justification, etc. */

#ifndef __NEBULAE_TEXTCONTROL_H__
#define __NEBULAE_TEXTCONTROL_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Beta/Font/Font.h>
#include <Nebulae/Beta/Gui/Control/Control.h>


namespace Nebulae
{
  
class TextControl : public Control
///
/// A simple control that can display some text.
///
{
protected:
  std::string                 m_text;
  Flags<TextFormat>           m_format;      ///< the formatting used to display the text (vertical and horizontal alignment, etc.)
  Colour                      m_text_color;  ///< the color of the text itself (may differ from GG::Control::m_color)
  bool                        m_clip_text;
  bool                        m_set_min_size;
  std::vector<std::shared_ptr<Font::TextElement > >
                              m_text_elements;
  std::vector<Font::LineData> m_line_data;
  CPSize                      m_code_points;
  std::shared_ptr<Font >      m_font;
  bool                        m_fit_to_text; ///< when true, this window will maintain a minimum width and height that encloses the text
  Point                       m_text_ul;     ///< stored relative to the control's UpperLeft()
  Point                       m_text_lr;     ///< stored relative to the control's UpperLeft()

  public:

    /** \name Structors */ ///@{
    /** Ctor. */
    TextControl( const WidgetFactory& factory, int x, int y, int w, int h, 
                 const std::string& str, const std::shared_ptr<Font>& font,
                 Colour color = CLR_BLACK, Flags<TextFormat> format = FORMAT_NONE,
                 uint32 flags = 0 );

    /** Ctor that does not require window size.  Window size is determined
        from the string and font; the window will be large enough to fit the
        text as rendered, and no larger.  The private member FitToText() will
        also return true. \see TextControl::SetText() */
    TextControl( const WidgetFactory& factory, int x, int y, const std::string& str,
                 const std::shared_ptr<Font>& font, Colour color = CLR_BLACK,
                 Flags<TextFormat> format = FORMAT_NONE, uint32 flags = 0 );

    virtual ~TextControl();
    //@}

    /** \name Accessors */ ///@{
    virtual WidgetType GetWidgetType() const { return LABEL; }

    virtual Point      GetMinUsableSize() const;

    /** Returns the text displayed in this control. */
    const std::string& GetText() const;

    /** Returns the text format (vertical and horizontal justification, use of
        word breaks and line wrapping, etc.) */
    Flags<TextFormat>  GetTextFormat() const;

    /** Returns the text color (this may differ from the Control::Color() in
        some subclasses) */
    Colour             TextColor() const;

    /** Returns true iff the text control clips its text to its client area;
        by default this is not done. */
    bool               ClipText() const;

    /** Returns true iff the text control sets its MinSize() when the bounds
        of its text change because of a call to SetText() or SetTextFormat();
        by default this is not done.  The minimum size of the control in each
        dimension will be the larger of the text size and the current
        MinSize(), if any has been set.  Note that this operates independently
        of fit-to-text behavior, which sets the window size, not its minimum
        size. */
    bool               SetMinSize() const;

    /** Sets the value of \a t to the interpreted value of the control's text.
        If the control's text can be interpreted as an object of type T by
        boost::lexical_cast (and thus by a stringstream), then the >> operator
        will do so.  Note that the return type is void, so multiple >>
        operations cannot be strung together.  Also, because lexical_cast
        attempts to convert the entire contents of the string to a single
        value, a TextControl containing the string "4.15 3.8" will fill a
        float with 0.0 (the default construction of float), even though there
        is a perfectly valid 4.15 value that occurs first in the string.
        \note boost::lexical_cast usually throws boost::bad_lexical_cast when
        it cannot perform a requested cast, though >> will return a
        default-constructed T if one cannot be deduced from the control's
        text. */
    //template <class T> void operator>>(T& t) const;

    /** Returns the value of the control's text, interpreted as an object of
        type T.  If the control's text can be interpreted as an object of type
        T by boost::lexical_cast (and thus by a stringstream), then GetValue()
        will do so.  Because lexical_cast attempts to convert the entire
        contents of the string to a single value, a TextControl containing the
        string "4.15 3.8" will throw, even though there is a perfectly valid
        4.15 value that occurs first in the string.  \throw
        boost::bad_lexical_cast boost::lexical_cast throws
        boost::bad_lexical_cast when it cannot perform a requested cast. This
        is handy for validating data in a dialog box; Otherwise, using
        operator>>(), you may get the default value, even though the text in
        the control may not be the default value at all, but garbage. */
    //template <class T> T GetValue() const;


    bool   Empty() const;   ///< Returns true iff text string equals "".
    CPSize Length() const;  ///< Returns the number of code points in the text.

    /** Returns the upper-left corner of the text as it is would be rendered
        if it were not bound to the dimensions of this control. */
    Point TextUpperLeft() const;

    /** Returns the lower-right corner of the text as it is would be rendered
        if it were not bound to the dimensions of this control. */
    Point TextLowerRight() const;

    /** Returns the line data for the text in this TextControl. */
    const std::vector<Font::LineData>& GetLineData() const;

    /** Returns the Font used by this TextControl to render its text. */
    const std::shared_ptr<Font>& GetFont() const;

    /** Returns true iff this TextControl was constructed using the ctor
        without width and height parameters.  \see TextControl::SetText() */
    bool FitToText() const;
    //@}


    /** \name Mutators */ ///@{
    /** Sets the text displayed in this control to \a str.  May resize the
        window.  If FitToText() returns true (i.e. if the second ctor was
        used), calls to this function cause the window to be resized to
        whatever space the newly rendered text occupies. */
    virtual void SetText(const std::string& str);

    virtual void SizeMove(const Point& ul, const Point& lr);

    /** Sets the text format; ensures that the flags are sane. */
    void         SetTextFormat(TextFormat format);

    /** Sets the text color. */
    void         SetTextColour(Colour color);

    /** Just like Control::SetColor(), except that this one also adjusts the
        text color. */
    virtual void SetColour(Colour c);

    /** Enables/disables text clipping to the client area. */
    void         ClipText(bool b);

    /** Enables/disables setting the minimum size of the window to be the text
        size. */
    void         SetMinSize(bool b);

    /** Sets text to the empty string. */
    void  Clear();                          

    /** Inserts \a c at position \a pos within the text.  \note Just as with
        most string parameters throughout GG, \a c must be a valid UTF-8
        sequence. */
    void  Insert(CPSize pos, char c);

    /** Inserts \a s at position \a pos within the text. */
    void  Insert(CPSize pos, const std::string& s);
    
    /** Erases \a num code points from the text starting at position \a
        pos. */
    void  Erase(CPSize pos, CPSize num = CP1);

    /** Inserts \a c at text position \a pos within line \a line.  \note Just
        as with most string parameters throughout GG, \a c must be a valid
        UTF-8 sequence. */
    void  Insert(std::size_t line, CPSize pos, char c);

    /** Inserts \a s at text position \a pos within line \a line. */
    void  Insert(std::size_t line, CPSize pos, const std::string& s);

    /** Erases \a num code points from the text starting at position \a
        pos within line \a line. */
    void  Erase(std::size_t line, CPSize pos, CPSize num = CP1);
    //@}

  private:
    void ValidateFormat();      ///< ensures that the format flags are consistent
    void AdjustMinimumSize();
    void RecomputeTextBounds(); ///< recalculates m_text_ul and m_text_lr

};

}

#endif // __NEBULAE_TEXTCONTROL_H__