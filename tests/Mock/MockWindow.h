#ifndef NEBULAE_TEST_MOCKWINDOW_H__
#define NEBULAE_TEST_MOCKWINDOW_H__

#include <Nebulae/Common/Window/Window.h>

namespace Nebulae {

class MockWindow : public Window {
  public:
    MockWindow() : Window() {}

    virtual bool Initiate( void* ) { return true; }
    virtual void Destroy()         {};
    virtual void Show()            {};
};

}

#endif // NEBULAE_TEST_MOCKWINDOW_H__