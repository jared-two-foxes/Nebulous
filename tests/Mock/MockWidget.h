#ifndef NEBULAE_BETA_TESTS_MOCKWIDGET_H__
#define NEBULAE_BETA_TESTS_MOCKWIDGET_H__

#include <Nebulae/Beta/Gui/Widget.h>

#include <gmock/gmock.h>

namespace Nebulae {

class MockWidget : public Widget 
{
private:
  public:
    MockWidget( const WidgetFactory& factory, int x, int y, int width, int height ) 
      : Widget(factory,x,y,width,height)
      {}

    MOCK_METHOD2( TouchDown, void ( const Point& pt, Flags<ModKey> mod_keys) );
    MOCK_METHOD3( TouchDrag, void ( const Point& pt, const Point& move, Flags<ModKey> mod_keys ) );
    MOCK_METHOD2( TouchUp,   void ( const Point& pt, Flags<ModKey> mod_keys ) );
    MOCK_METHOD2( Clicked,   void ( const Point& pt, Flags<ModKey> mod_keys ) );
};

}

#endif //NEBULAE_BETA_TESTS_MOCKWIDGET_H__