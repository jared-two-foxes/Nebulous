
#ifndef _UTIL_SCREENCONTROLLER_H_
#define _UTIL_SCREENCONTROLLER_H_

#include <Nebulae/Nebulae.h>
#include <Nebulae/Common/Window/KeyCodes.h>

namespace Game {
    
class ScreenController;

    
class Screen
{
public:
  friend class ScreenController;
  friend class ScreenHandle;

private:
  std::string       m_name;  ///< Unique identifying name of the screen.
  ScreenController* m_owner; ///< Reverse pointer to the ScreenController.

  public:
    Screen( const std::string& name, ScreenController *owner );
    ~Screen();

    virtual void Init();
    virtual void Enter( const std::vector<std::string>& args );
    virtual void Update( Real dt_s );
    virtual void Exit();
    
    virtual bool OnKeyPressed(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnKeyReleased(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnTouchDown(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnTouchUp(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);
    virtual bool OnTouchDrag(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys);

    virtual bool Goto( const std::string& action, const std::vector<std::string>& args );
    
    const std::string& GetName() const { return m_name; }
    ScreenController*  GetOwner()      { return m_owner; }
};

class ScreenHandle
///
/// Simple screen wrapper class.
///
{
private:
  Screen* m_ptr; ///< Pointer to the wrapped screen.

  public:
    ScreenHandle(Screen *ptr) : m_ptr(ptr) {}

    operator bool ()                                   { return m_ptr != NULL; }
    void Enter( const std::vector<std::string> &args ) { if (m_ptr != NULL) m_ptr->Enter(args); }
    void Update( float dt_s )                          { if (m_ptr != NULL) m_ptr->Update(dt_s); }
    void Exit()                                        { if (m_ptr != NULL) m_ptr->Exit(); }
    
    virtual void OnKeyPressed(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys)  { if (m_ptr != NULL) m_ptr->OnKeyPressed(keyCode, modKeys); }
    virtual void OnKeyReleased(Nebulae::KeyCode keyCode, Nebulae::Flags<Nebulae::ModKey> modKeys) { if (m_ptr != NULL) m_ptr->OnKeyReleased(keyCode, modKeys); }
    virtual void OnTouchDown(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys)           { if (m_ptr != NULL) m_ptr->OnTouchDown(x, y, modKeys); }
    virtual void OnTouchUp(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys)             { if (m_ptr != NULL) m_ptr->OnTouchUp(x, y, modKeys); }
    virtual void OnTouchDrag(int32 x, int32 y, Nebulae::Flags<Nebulae::ModKey> modKeys)           { if (m_ptr != NULL) m_ptr->OnTouchDrag(x, y, modKeys); }

    bool Goto( const std::string& action, const std::vector<std::string>& args )
    {
      return (m_ptr != NULL) ? m_ptr->Goto(action, args) : false;
    }
};

class ScreenController
{
private:
  struct Edge
  {
    short                    a;
    short                    b;
    std::string              condition;
    std::vector<std::string> args;

    Edge() : a(0), b(0) {}
    Edge(int a, int b, const std::string& condition, const std::vector<std::string>& args )
      : a(a), b(b), condition(condition), args(args) {}
  };

  class Graph : public std::vector<Edge>
  {
  public:
    const Edge* FindEdge(int start, const std::string& condition) const
    {
      for (size_t i = 0; i < size(); i++)
      {
        if ((*this)[i].a == start && (*this)[i].condition == condition)
          return &(*this)[i];
      }
      return NULL;
    }
  };

public:
  static const int EXIT_ID = -2;// Ask for termination
  
protected:
  friend class Screen;
    
private:
  std::vector<Screen*> m_screens;        ///< Graph vertices list.
  Graph                m_graph;          ///< Graph edge list.
  int                  m_current_screen; ///< Current screen vertex.

  public:
    ScreenController();

    int GetScreenCount() const;
    int GetEdgeCount() const;
    int GetCurrentScreenId() const;
    int GetScreenId( const std::string& name ) const;

    void Clear();
    void AddScreen( Screen* screen );
    void AddEdge( const std::string& screen_from, const std::string& screen_to, const std::string& condition, const std::string& arg0 );
    void AddEdge( const std::string& screen_from, int b, const std::string& condition, const std::string& arg0 );
    bool HasEdge( const std::string& action ) const;
    ScreenHandle CurrentScreen( const std::string& name );
    ScreenHandle CurrentScreen( int set_current = -1 );
    bool GotoScreen( const std::string& action, const std::vector<std::string> &args );
  
};

}

#endif  // _UTIL_SCREENCONTROLLER_H_
