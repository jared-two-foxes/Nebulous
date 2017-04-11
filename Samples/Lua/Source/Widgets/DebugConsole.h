#ifndef NEBULAE_DEBUGCONSOLE_H__
#define NEBULAE_DEBUGCONSOLE_H__

#include <Nebulae/Common/Common.h>

struct lua_State;

namespace Nebulae {

class Keyboard;
class LuaInterpreter;
class TextControl;

class DebugConsole : public InputListener
///
/// The Console class is an ingame control which displays a series of text like debug & log messages.
/// It wraps a TextControl object which is used to display the text and defines an interface for being
/// able to scroll through the text.
///
{
public:
  typedef std::shared_ptr<LuaInterpreter > ScriptInterpreterPtr;

private:
  ScriptInterpreterPtr      m_scriptInterpreter;
  std::vector<std::string > m_lines;              ///< A list of lines that has been writen to the console.
  std::string               m_currentLine;
  std::vector<std::string > m_history;
  int32                     m_currentHistoryLine;
  TextControl*              m_consoleText;        ///< The widget that is used to display the output.
  Keyboard&                 m_keyboard;

  public:
    DebugConsole( ScriptInterpreterPtr scriptInterpreter, TextControl* consoleElement, Keyboard& keyboard );
    ~DebugConsole();

    const std::vector<std::string >& Lines() const;

    virtual void KeyPressed(KeyCode keyCode, Flags<ModKey> modKeys);
    virtual void KeyReleased(KeyCode keyCode, Flags<ModKey> modKeys);

    void Clear();
    void AddLine( const std::string& line );
    void PushToGui();

  private:
    static int ClearConsoleFromLua( lua_State *L );
    static int InsertOutputFromLua( lua_State *L );

    void AddToHistory( const std::string& history );

}; // DebugConsole

}

#endif // NEBULAE_DEBUGCONSOLE_H__