#ifndef NEBULAE_COMMON_WINDOW_H__
#define NEBULAE_COMMON_WINDOW_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

/** Window.
	*/
class Window
{
private:
  std::wstring               m_caption;
  Point                      m_position;
  Point                      m_size;
  std::list<InputListener* > m_listeners;

  public:
    Window();
    virtual ~Window();

    int32 GetWidth() const;
    int32 GetHeight() const;
    const Point GetSize() const;
    const Point GetPosition() const;
    const wchar_t* GetCaption() const;

    bool IsPointInWindow( const Point& point ) const;

    virtual bool Initiate( void* creationData ) = 0;
    virtual void Destroy() = 0;
    virtual void Show() = 0;
    virtual void MoveAndResize( int x, int y, int width, int height );
    virtual void SetCaption( const wchar_t* szCaption );

    void AddInputListener( InputListener* inputListener );
    void RemoveInputListener( InputListener* inputListener );
    bool HasInputListener( InputListener* inputListener );

    void ButtonPressed( int32 x, int32 y, MouseButton button, Flags<ModKey> modKeys );
    void ButtonReleased( int32 x, int32 y, MouseButton button, Flags<ModKey> modKeys );
    void ButtonDragged( int32 x, int32 y, MouseButton button, Flags<ModKey> modKeys );
    void KeyPressed( KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys );
    void KeyReleased( KeyCode keyCode, uint32 key_code_point, Flags<ModKey> modKeys );

}; //Window

} //Nebulae

#endif // NEBULAE_COMMON_