#ifndef NEBULAE_WIDGETSERIALIZER_H__
#define NEBULAE_WIDGETSERIALIZER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class GuiManager;
class RenderSystem;
class SpriteAtlasManager;
class Widget;

class WidgetSerializer
{
public:
  typedef std::shared_ptr<GuiManager >         GuiManagerPtr;
  typedef std::shared_ptr<RenderSystem >       RenderSystemPtr;
  typedef std::shared_ptr<SpriteAtlasManager > SpriteAtlasManagerPtr;

private:
  GuiManagerPtr         m_gui;
  RenderSystemPtr       m_renderDevice;
  SpriteAtlasManagerPtr m_atlasManager;
  Point                 m_rootDeviceSize; ///< The dimensions of the root device that the UI was created for.

  public:
    WidgetSerializer( GuiManagerPtr& gui, const RenderSystemPtr& renderDevice, const SpriteAtlasManagerPtr& atlasManager );

    void    SetRootDimensions( const Point& point );
    Widget* ReadIn( File& stream );

  private:
  	Widget* LoadElement( const Json::Value& elementData, GuiManagerPtr gui );

};

}

#endif // NEBULAE_WIDGETSERIALIZER_H__