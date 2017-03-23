#include "StaticGraphic.h"
#include <Nebulae/Common/Window/Window.h>
//#include <Nebulae/Beta/Gui/WidgetUtils.h>

using namespace Nebulae;

StaticGraphic::StaticGraphic( const WidgetFactory& factory, int x, int y, int w, int h, SubTexture* pTexture, uint32 flags )
  : Control( factory, x, y, w, h, flags ),
    m_pGraphic( pTexture )
{

}

StaticGraphic::~StaticGraphic()
{

}

/*void StaticGraphic::Render( RenderSystem* pRenderSystem ) const
{
 // if ( !m_pGraphic->IsLoaded() )
 // {
 //   m_pGraphic->Load();
 // }

	//Point origin = WidgetUtils::DetermineChildWidgetOrigin( this, m_pParent );

	//WidgetUtils::RenderWidget( pRenderSystem, origin, (float)GetWidth(), (float)GetHeight(), m_pGraphic );
}*/