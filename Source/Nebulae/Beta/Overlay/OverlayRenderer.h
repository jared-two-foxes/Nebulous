#ifndef NEBULAE_BETA_OVERLAYRENDERER_H__
#define NEBULAE_BETA_OVERLAYRENDERER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class RenderSystem;
class Material;
class SpriteBatch;
class SubTexture;

class OverlayRenderer 
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

protected:
  RenderSystemPtr m_renderDevice;    ///< Device to send render calls to.
  Material*       m_basicMaterial;   ///< Material to use when rendering for the basic quad.
  Material*       m_complexMaterial; ///< Material to use when rendering for the complex quad.
  
  SpriteBatch*    m_batcher;

  public:
    OverlayRenderer( RenderSystemPtr renderDevice, SpriteBatch* batcher );
    //@todo - Destructor.... (virtual)

    virtual bool Init();

    /** Renders a rectangle starting at ul and ending just before lr, and
        assumes that OpenGL in in a "2D" state.  The border is drawn in the
        desired thickness and color, then whatever is space is left inside
        that is filled with color \a color.  No checking is done to make sure
        that \a borderThick * 2 is <= \a lr.x - \a ul.x (or <= \a lr.y - \a
        ul.y, for that matter).  This method of drawing and the 2D
        requirements are true for all functions that follow. */
    void FlatRectangle( Point ul, Point lr, Colour color, Colour borderColor, uint32 borderThick = 2 ) const;

    /** Like FlatRectangle(), but with a "beveled" appearance.  The
        borderColor used to create a lighter and a darker version of
        borderColor, which are used to draw beveled edges around the inside
        of the rectangle to the desired thickness.  If \a up is true, the
        beveled edges are lighter on the top and left, darker on the bottom
        and right, effecting a raised appearance.  If \a up is false, the
        opposite happens, and the rectangle looks depressed.  This is true of
        all the Beveled*() functions. */
    void BeveledRectangle( Point ul, Point lr, Colour color, Colour borderColor, bool up, uint32 bevelThick = 2,
                            bool bevelLeft = true, bool bevelTop = true, bool bevelRight = true, bool bevelBottom = true ) const;

  protected: 
    /** Returns the lightened version of color clr.  LightColor leaves the
        alpha channel unchanged, and multiplies the other channels by a some
        factor.  (The factor is defined within LightColor().) */
    Colour GetLightColour( Colour clr ) const;

    /** Returns the darkened version of color clr.  DarkColor leaves the alpha
        channel unchanged, and divides the other channels by a some factor.
        (The factor is defined within DarkColor().) */
    Colour GetDarkColour( Colour clr ) const;

    /** Returns the "disabled" (grayed) version of color clr.  DisabledColor
        leaves the alpha channel unchanged, and adjusts the other channels in
        the direction of gray (GG_CLR_GRAY) by a factor between 0.0 and 1.0.
        (The factor is defined within DisabledColor().)  This is used
        throughout the GG classes to render disabled controls. */
    Colour GetDisabledColour( Colour clr ) const;


    void BeginScissorClipping( Point ul, Point lr );
    void EndScissorClipping();

    void BeginStencilClipping( Point inner_ul, Point inner_lr, Point outer_ul, Point outer_lr );
    void EndStencilClipping();

    /** */
    void Rectangle( RenderSystemPtr renderDevice, Nebulae::Point ul, Nebulae::Point lr, Colour colour,
                    Colour borderColour1, Colour borderColour2, uint32 bevelThick, bool bevelLeft, 
                    bool bevelTop, bool bevelRight, bool bevelBottom ) const;

    void DrawQuad( RenderSystemPtr renderer, const Nebulae::Point& upperLeft, const Nebulae::Point& lowerRight,
                    const Colour& colour = CLR_WHITE, const SubTexture* texture = NULL, float depth = 0.0f, 
                    float rotation = 0.0f ) const;

    void DrawComplexQuad( RenderSystemPtr renderer, const Nebulae::Point& ul, const Nebulae::Point& ur,
                          const Nebulae::Point& ll, const Nebulae::Point& lr, const Colour& colour = CLR_WHITE,
                          const SubTexture* texture = NULL, float depth = 0.0f, float rotation = 0.0f ) const;

};

}

#endif // NEBULAE_BETA_OVERLAYRENDERER_H__
