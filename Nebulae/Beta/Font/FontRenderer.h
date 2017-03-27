#ifndef NEBULAE_FONT_FONTRENDERER_H__
#define NEBULAE_FONT_FONTRENDERER_H__

#include <Nebulae/Beta/Font/Font.h>
#include <Nebulae/Beta/Overlay/OverlayRenderer.h>

namespace Nebulae {

class Font;
class Texture;
class SubTexture;

class FontRenderer : public OverlayRenderer { 

  public:
    /** \brief Holds the state of tags during rendering of text.

        By keeping track of this state across multiple calls to RenderText(),
        the user can preserve the functionality of the text formatting tags,
        if present. */
    struct RenderState
    {
      std::stack<Colour > colours;                /** The stack of text colors (as set by previous tags). */
    };

    typedef std::unordered_map<boost::uint32_t, SubTexture > GlyphTextureMap;  

  private:
    const std::shared_ptr<Font > m_font;          ///< The font that this renderer is capable of drawing.
    std::vector<Texture* >       m_textures;      ///< The texture objects in which the glyphs can be found
    GlyphTextureMap              m_glyphTextures; ///< Mapping of character to the subtexture which represents it.

    public:
      FontRenderer( RenderSystemPtr renderSystem, SpriteBatch* batcher, const std::shared_ptr<Font >& font );

			FontRenderer( FontRenderer&& ) = delete;
			FontRenderer& operator =( FontRenderer&& ) = delete;

      const std::shared_ptr<Font > GetFont() const;

      virtual bool Init( File* is );

      /** Renders glyph for \a c and returns advance of glyph rendered.  \note
          Just as with most string parameters throughout GG, \a c must be a
          valid UTF-8 sequence. */
      int RenderGlyph( const Point& pt, char c, RenderState* render_state = nullptr ) const;

      /** Renders glyph for \a c and returns advance of glyph rendered. */
      int RenderGlyph( const Point& pt, boost::uint32_t c, RenderState* render_state = nullptr ) const;

      /** Unformatted text rendering; repeatedly calls RenderGlyph, then returns
          advance of entire string. */
      int RenderText( const Point& pt, const std::string& text, const Colour& colour, RenderState* render_state = nullptr  ) const;

      /** Formatted text rendering. */
      void RenderText( const Point& pt1, const Point& pt2, const std::string& text, Flags<TextFormat>& format,
                       const Colour& colour, const std::vector<Font::LineData>* line_data = nullptr, 
                       RenderState* render_state = nullptr ) const;

      /** Formatted text rendering over a subset of lines and code points.  The
          glyphs rendered are in the range [CodePointIndexOf(<i>begin_line</i>,
          <i>begin_char</i>, <i>line_data</i>), CodePointIndexOf(<i>end_line</i> -
          1, <i>end_char</i>, <i>line_data</i>)). */
      void RenderText( const Point& pt1, const Point& pt2, const std::string& text, Flags<TextFormat>& format, 
                       const std::vector<Font::LineData>& line_data, RenderState& render_state,
                       std::size_t begin_line, CPSize begin_char, 
                       std::size_t end_line, CPSize end_char ) const;

    private:
      int  RenderGlyph( const Point& pt, const Font::Glyph& glyph, const SubTexture& subtexture,
                        const RenderState* render_state ) const;

};

}

#endif // NEBULAE_FONT_FONTRENDERER_H__