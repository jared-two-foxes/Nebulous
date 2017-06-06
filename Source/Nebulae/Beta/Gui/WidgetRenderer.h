#ifndef __NEBULAE_BETA_GUI_WIDGETRENDERER_H__
#define __NEBULAE_BETA_GUI_WIDGETRENDERER_H__

#include <Nebulae/Beta/Overlay/OverlayRenderer.h>


namespace Nebulae {

class FileSystem;
class Font;
class FontRenderer;
class RenderSystem;
class Scheme;
class Widget;
 
class ButtonControl;
class EditControl;
class ListBox;
class Slider;
class TextControl;


class WidgetRenderer : public OverlayRenderer 
///
/// Is responsible for the rendering gui components (widgets).
///
{
public:
  typedef std::shared_ptr<FileSystem >   FileArchivePtr;
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

private:
  FileArchivePtr              m_fileSystem;      ///< File Access object.
  Scheme*                     m_activeScheme;    ///< Active skin to use while rendering.
  std::vector<FontRenderer* > m_fontRenderers;   ///< List of font renderers.
  const Widget*               m_focusWidget;     ///< The widget which is currently considered the focus.

  public:
    WidgetRenderer( FileArchivePtr fileSystem, SpriteBatch* batcher, RenderSystemPtr renderSystem );

    FontRenderer* AddFont( const std::shared_ptr<Font >& font ); 
    void          SetFocusWindow( Widget* focusWidget );

    void Render( const std::list<Widget*>& widgets );

  private: 
    const FontRenderer* FindFontRendererByFont( const std::shared_ptr<Font >& font ) const;

    void BeginClipping( const Widget& widget );
    void EndClipping( const Widget& widget );

    void RenderWidget( const Widget& widget );
    void RenderButton( const ButtonControl& control, int32 depth );
    void RenderEditControl( const EditControl& control, int32 depth );
    void RenderListBox( const ListBox& control, int32 depth );
    void RenderSlider( const Slider& control, int32 depth );
    void RenderTextControl( const TextControl& control, int32 depth );

};

}

#endif // __NEBULAE_BETA_GUI_WIDGETRENDERER_H__