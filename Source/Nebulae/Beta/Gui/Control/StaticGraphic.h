
/** \file StaticGraphic.h 
 *  \brief Contains the StaticGraphic class, a fixed image control. 
 */

#ifndef __STATICGRAPHIC_H__
#define __STATICGRAPHIC_H__

#include <Nebulae/Beta/Gui/Control/Control.h>

namespace Nebulae {

class SubTexture;

class StaticGraphic : public Control
{
protected:
  SubTexture* m_pGraphic;

  public:
    /** \name Structors */ ///@{
    StaticGraphic( const WidgetFactory& factory, int x, int y, int w, int h, SubTexture* texture/*,
                   Flags<GraphicStyle> style = GRAPHIC_NONE*/, uint32 flags = INTERACTIVE );

    virtual ~StaticGraphic();
    //@}

    virtual WidgetType GetWidgetType() const { return STATIC; }
    
    const SubTexture* GetGraphic() const { return m_pGraphic; }

}; //StaticGraphic

} //namespace Nebulae

#endif // __STATICGRAPHIC_H__