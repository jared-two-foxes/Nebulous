#ifndef NEBULAE_FONT_H__
#define NEBULAE_FONT_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Font/AlignmentFlags.h>
#include <Nebulae/Beta/Font/UnicodeCharsets.h>


namespace Nebulae {


/** Text formatting flags. */
NE_FLAG_TYPE(TextFormat);
extern const TextFormat FORMAT_NONE;        ///< Default format selected.
extern const TextFormat FORMAT_VCENTER;     ///< Centers text vertically.
extern const TextFormat FORMAT_TOP;         ///< Top-justifies text.
extern const TextFormat FORMAT_BOTTOM;      ///< Justifies the text to the bottom of the rectangle.
extern const TextFormat FORMAT_CENTER;      ///< Centers text horizontally in the rectangle.
extern const TextFormat FORMAT_LEFT;        ///< Aligns text to the left. 
extern const TextFormat FORMAT_RIGHT;       ///< Aligns text to the right. 
extern const TextFormat FORMAT_WORDBREAK;   ///< Breaks words. Lines are automatically broken between words if a word would extend past the edge of the control's bounding rectangle.  As always, a '\\n' also breaks the line.
extern const TextFormat FORMAT_LINEWRAP;    ///< Lines are automatically broken when the next glyph would be drawn outside the the text rectangle.  As always, a '\\n' also breaks the line.
extern const TextFormat FORMAT_IGNORETAGS;  ///< Text formatting tags (e.g. <rgba 0 0 0 255>) are treated as regular text.


/** \class Nebulae::StrSize
    \brief The string size and index value type.

    Such values refer to indices into UTF-8 encoded strings, \a not code
    points.  \see NE_STRONG_SIZE_TYPEDEF */
NE_STRONG_SIZE_TYPEDEF(StrSize);

/** \class Nebulae::CPSize
    \brief The code point size and index value type.

    Such values refer to indices of code points in Unicode strings, \a not
    indices into underlying UTF-8 encoded strings.  \see
    NE_STRONG_SIZE_TYPEDEF */
NE_STRONG_SIZE_TYPEDEF(CPSize);


// some useful size constants
extern const StrSize         S0;
extern const StrSize         S1;
extern const StrSize         INVALID_STR_SIZE;
extern const CPSize          CP0;
extern const CPSize          CP1;
extern const CPSize          INVALID_CP_SIZE;

extern const boost::uint32_t WIDE_SPACE;
extern const boost::uint32_t WIDE_NEWLINE;
extern const boost::uint32_t WIDE_CR;
extern const boost::uint32_t WIDE_FF;
extern const boost::uint32_t WIDE_TAB;
                             
extern const std::string     ALIGN_LEFT_TAG;
extern const std::string     ALIGN_CENTER_TAG;
extern const std::string     ALIGN_RIGHT_TAG;
extern const std::string     PRE_TAG;

extern const std::vector<std::pair<boost::uint32_t, boost::uint32_t> > PRINTABLE_ASCII_ALPHA_RANGES;
extern const std::vector<std::pair<boost::uint32_t, boost::uint32_t> > PRINTABLE_ASCII_NONALPHA_RANGES;


/** \brief A bitmapped font rendering class.

  Font creates one or more 16-bits-per-pixel OpenGL textures that contain
  rendered glyphs from a requested font file at the requested point size,
  including only the requested ranges of code points.  Once the textures
  have been created, text is rendered to the display by rendering quads
  textured with portions of the glyph textures.  The glyphs are rendered to
  the textures in white, with alpha blending used for antialiasing.  The
  user should set the desired text color with a call to glColor*() before
  any call to RenderText().  When text is rendered, DetermineLines() is
  called to determine where the line breaks are, so that text can be
  rendered centered, right-justified, or whatever.  To cut down on this
  computation, when the text is not changing very rapidly (ie not every
  frame), DetermineLines() can be called by the user once, and the result
  supplied to RenderText() repeatedly.  When this is done, the iteration
  through the text to determine line breaks is not necessary at render time.
  The user is responsible for ensuring that the line data applies to the
  text string supplied to RenderText().  See UnicodeCharsets.h for the
  ranges of code points available, including a function that allow one to
  determine which ranges are necessary for rendering a certain string.
  Point sizes above 250 are not supported.  Note that users should not
  normally need to use Font directly.  Users should instead use TextControl,
  Edit, or MultiEdit.

  <h3>Text Formatting Tags</h3>

  GG::Font supports a few text formatting tags for convenience.  These tags
  are similar to HTML or XML tags; there is an opening version "<tag>" and a
  closing version "</tag>" of each tag.  Tags can be nested.  For instance,
  consider the use of the italics tag \<i> here:

  \verbatim
    <i>some text <i>and </i>some more </i>text \endverbatim

  In this example, everything is italicized except for "text".  Each \<i>
  tag establishes that italics should be used for all further text until the
  next \a matching \</i> tag.  The first \<i> tag matches the second \</i>
  tag, and the two inner tags are matched.  Note that unmatched close-tags
  (e.g. \</i>) are ignored by the text parser Font uses to find tags -- they
  will appear as regular text.  The text justification tags are used on a
  per-line basis, since it makes no sense to, for instance, right-justify
  only a part of a line and center the rest. When more than one
  justification tag appears on a line, the last one is used.  A
  justification close-tag indicates that a line is to be the last one with
  that justification, and only applies if that justification is active.

  <br>The supported tags are:
  - \verbatim<i></i> \endverbatim                 Italics
  - \verbatim<u></u> \endverbatim                 Underline
  - \verbatim<rgba r g b a></rgba> \endverbatim   Color. Sets current rendering color to that specified by parameters.  Parameters may be either floating point values in the range [0.0, 1.0], or integer values in the range [0, 255].  All parameters must be in one format or the other.  The \</rgba> tag restores the previously set \<rgba> color, or restores the default color used to render the text when there are no other \<rgba> tags in effect.  Example tag: \<rgba 0.4 0.5 0.6 0.7>.
  - \verbatim<left></left> \endverbatim           Left-justified text.
  - \verbatim<center></center> \endverbatim       Centered text.
  - \verbatim<right></right> \endverbatim         Right-justified text.
  - \verbatim<pre></pre> \endverbatim             Preformatted.  Similar to HTML \<pre> tag, except this one only causes all tags to be ignored until a subsequent \</pre> tag is seen.  Note that due to their semantics, \<pre> tags cannot be nested.

  <p>Users of Font may wish to create their own tags as well.  Though Font
  will know nothing about the semantics of the new tags, it is possible to
  let Font know about them, in order for Font to render them invisible as it
  does with the tags listed above.  See the static methods
  RegisterKnownTag(), RemoveKnownTag(), and ClearKnownTags() for details.
  It is not possible to remove the built-in tags using these methods.  If
  you wish not to use tags at all, call DetermineLines() and RenderText()
  with the format parameter containing FORMAT_IGNORETAGS, or include a
  \<pre> tag at the beginning of the text to be rendered.
 */
class Font
{
public:
  /** \brief This just holds the essential data necessary to render a glyph
      from the texture(s) created at Font creation time. */
  struct Glyph
  {
    Glyph(); ///< Default ctor
    Glyph( int w, int h, int lb, int vb, int adv ); ///< Ctor

    int width;            ///< The width of the glyph only.
    int height;           ///< The height of the glyph only.
    int left_bearing;     ///< The space that should remain before the glyph.
    int vertical_bearing; ///< The distance from the baseline to the top of the glyph.
    int advance;          ///< The amount of space the glyph should occupy, including glyph graphic and inter-glyph spacing.
  };

  /** \brief Used to encapsulate a token-like piece of text to be rendered
      using GG::Font. */
  struct TextElement
  {
    /** The types of token-like entities that can be represented by a
        TextElement. */
    enum TextElementType {
      TEXT,       ///< Some non-whitespace text (e.g. "The").
      WHITESPACE, ///< Some whitespace text (e.g. "  \n").

      /** A newline.  Newline TextElements represent the newline code
          point when it is encountered in a rendered string, though they
          do not contain the actual newline character -- their \a text
          members are always ""). */
      NEWLINE
    };

    /** Ctor.  \a ws indicates that the element contains only whitespace;
        \a nl indicates that it is a newline element. */
    TextElement(bool ws, bool nl);

    virtual ~TextElement(); ///< Virtual dtor.

    TextElement(const TextElement&);            // Copy constructor
    TextElement(TextElement&&);                 // Move constructor
    TextElement& operator=(const TextElement&); // Copy assignment operator
    TextElement& operator=(TextElement&&);      // Move assignment operator

    /** Returns the width of the element. */
    int Width() const;

    /* Returns the number of characters in the original string that the
       element represents. */
    StrSize StringSize() const;

    /** Returns the number of code points in the original string that the
        element represents. */
    CPSize CodePointSize() const;

    
    std::string       text;       ///< The text from the original string represented by the element.
    std::vector<int > widths;     ///< The widths of the glyphs in \a text.
    const bool        whitespace; ///< True iff this is a whitespace element.
    const bool        newline;    ///< True iff this is a newline element.

  protected:
    TextElement();

  private:
    mutable int cached_width;

  };


  /** \brief Holds the essential data on each line that a string occupies when
      rendered with given format flags.

      \a char_data contains the visible glyphs for each line, plus any text
      formatting tags present on that line as well. */
  struct LineData
  {
    LineData(); ///< Default ctor.

    /** \brief Contains the extent, the index into the original string,
        and the text formatting tags that should be applied before
        rendering of a visible glyph. */
    struct CharData
    {
      /** Defauilt ctor. */
      CharData();

      /** Ctor. */
      CharData( int extent_, StrSize str_index, StrSize str_size, CPSize cp_index );

      /** The code point of this glyph. */
      boost::uint32_t code_point;

      /** Copy of the glyph which exists at this position. */
      Glyph glyph;

      /** The furthest-right extent of this glyph as it appears on the
          line. */
      int extent;

      /** The position in the original string of the first character of
          this glyph. */
      StrSize string_index;

      /** The size in the original string of the characters that make up
          this glyph. */
      StrSize string_size;

      /** The code point index of this glyph. */
      CPSize code_point_index;
    };

    int  Width() const; ///< Returns the width of the line.
    bool Empty() const; ///< Returns true iff char_data has size 0.

    /** FORMAT_LEFT, FORMAT_CENTER, or FORMAT_RIGHT; derived from text
        format flags and/or formatting tags in the text. */
    Alignment justification;
    
    /** Data on each individual glyph. */
    std::vector<CharData > char_data;
  };

  
  typedef std::unordered_map<boost::uint32_t, Glyph> GlyphMap;    

  typedef GlyphMap::iterator       iterator;
  typedef GlyphMap::const_iterator const_iterator;

private:
  std::string                  m_font_filename;
  unsigned int                 m_pt_sz;
  std::vector<UnicodeCharset > m_charsets;         ///< The sets of glyphs that are covered by this font object
  int                          m_ascent;           ///< Maximum amount above the baseline the text can go
  int                          m_descent;          ///< Maximum amount below the baseline the text can go
  int                          m_height;           ///< Ascent - descent
  int                          m_lineskip;         ///< Distance that should be placed between lines
  int                          m_space_width;      ///< The width of the glyph for the space character
  GlyphMap                     m_glyphs;           ///< The locations of the images of each glyph within the textures

  public:
    /** \name Structors */ ///@{
    /** Ctor.  Construct a font using only the printable ASCII characters.
        \throw Font::Exception Throws a subclass of Font::Exception if the
        condition specified for the subclass is met. */
    Font( const std::string& font_filename, uint32 pts );

    ~Font();
    //@}

    bool Load( File* file );

    /** \name Accessors */ ///@{
    const_iterator  begin() const;               ///< returns an iterator to the first list row
    const_iterator  end() const;                 ///< returns an iterator to the imaginary row one past the last
    
    iterator        find( const boost::uint32_t& );
    const_iterator  find( const boost::uint32_t& ) const;

    /** Returns the name of the file from which this font was created. */
    const std::string& FontName() const;

    /** Returns the point size in which the characters in the font object are
        rendered. */
    uint32 PointSize() const;

    /** Returns the range(s) of code points rendered in the font */
    const std::vector<UnicodeCharset >& UnicodeCharsets() const;

    /** Returns the maximum amount above the baseline the text can go. */
    int Ascent() const;

    /** Returns the maximum amount below the baseline the text can go. */
    int Descent() const;

    /** Returns (Ascent() - Descent()). */
    int Height() const;

    /** Returns the distance that should be placed between lines.  This is
        usually not equal to Height(). */
    int Lineskip() const;

    /** Returns the width of the glyph for the space character. */
    int SpaceWidth() const;

    /** Returns the maximum dimensions of the string in x and y, and populates
        \a line_data. */
    Point DetermineLines( const std::string& text, Flags<TextFormat>& format, int box_width,
                          std::vector<LineData>& line_data ) const;

    /** Returns the maximum dimensions of the string in x and y, and populates
        \a line_data and \a text_elements.  Note that \a text_elements must be
        empty. */
    Point DetermineLines( const std::string& text, Flags<TextFormat>& format, int box_width,
                          std::vector<LineData>& line_data,
                          std::vector<std::shared_ptr<TextElement> >& text_elements ) const;

    /** Returns the maximum dimensions of the string in x and y, and populates
        \a line_data.  The contents of \a text_elements will be used, and the
        equivalent work done by DetermineLines() will be skipped.  Supplying a
        \a text and a \a text_elements that are incompatible will result in
        undefined behavior. */
    Point DetermineLines( const std::string& text, Flags<TextFormat>& format, int box_width,
                          const std::vector<std::shared_ptr<TextElement> >& text_elements,
                          std::vector<LineData>& line_data) const;

    /** Returns the maximum dimensions of the string in x and y.  Provided as
        a convenience; it just calls DetermineLines with the given
        parameters. */
    Point TextExtent( const std::string& text, Flags<TextFormat> format = FORMAT_NONE,
                      int box_width = 0 ) const;

    /** Returns the maximum dimensions of the text in x and y. */
    Point TextExtent( const std::string& text, const std::vector<LineData>& line_data ) const;
    //@}
  
  private:
    /** \name Accessors */ ///@{
    Point DetermineLinesImpl( const std::string& text,
                              Flags<TextFormat>& format,
                              int box_width,
                              std::vector<LineData>& line_data,
                              std::vector<std::shared_ptr<TextElement> >* text_elements_ptr) const;

    void ValidateFormat( Flags<TextFormat>& format ) const;
    //@}

};
    
CPSize CodePointIndexOf( std::size_t line, CPSize index, const std::vector<Font::LineData>& line_data );

StrSize StringIndexOf( std::size_t line, CPSize index, const std::vector<Font::LineData>& line_data );
    
std::pair<std::size_t,CPSize> LinePositionOf( CPSize index, const std::vector<Font::LineData>& line_data );

namespace detail {
  template <class CharT, bool CharIsSigned = std::is_signed<CharT>::value>
  struct ValidUTFChar;
    
  template <class CharT>
  struct ValidUTFChar<CharT, true>
  {
      bool operator()(CharT c)
      { return 0x0 <= c; }
  };
    
  template <class CharT>
  struct ValidUTFChar<CharT, false>
  {
      bool operator()(CharT c)
      { return c <= 0x7f; }
  };
}
    
}

inline boost::uint32_t CharToUint32_t(char c)
{ return c < 0 ? 256 + c : c; }

#endif // NEBULAE_FONT_H__