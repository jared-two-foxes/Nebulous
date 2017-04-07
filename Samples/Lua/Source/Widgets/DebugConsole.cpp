
#include "DebugConsole.h"

#include "Lua/LuaInterpreter.h"

#include <Nebulae/Common/Platform/Keyboard.h>

#include <Nebulae/Beta/Gui/Control/TextControl.h>

using namespace Nebulae;


/// This lua snippet will replace the internal lua print implementation redirecting it to call our implementation
/// of print which the Console object can then "listen" to and thus enable it to be able to display lua print 
/// statements.  The original print function can still be invoked by calling oldprint instead.
const char* lua_function_myprint =
  "function myprint(...) \n"
  "  local str \n"
  "  str = '' \n"
  " \n"
  "  local args = table.pack(...) \n"
  "  for i = 1, args.n do \n"
  "   if str ~= '' then str = str .. '\t' end \n"
  "   str = str .. tostring( args[i] ) \n"
  "  end \n"
  "   \n"
  "  str = str .. '\\n' \n"
  " \n"
  "  ConsoleOut( str ) \n"
  "end \n"
  " \n"
  "oldprint = print \n"
  "print = myprint \n";


DebugConsole::DebugConsole( ScriptInterpreterPtr scriptInterpreter, TextControl* consoleText, Keyboard& keyboard )
: m_scriptInterpreter( scriptInterpreter ),
  m_currentHistoryLine( -2 ),
  m_consoleText( consoleText ),
  m_keyboard( keyboard )
{
  // Register this as a lua table userdata type.
  m_scriptInterpreter->RegisterUserData( "console", this );

  // Register the Console C++ functions to lua.
  m_scriptInterpreter->RegisterFunction( "ConsoleOut", InsertOutputFromLua );
  m_scriptInterpreter->RegisterFunction( "Clear", ClearConsoleFromLua );

  // Rebind the print lua function to our function.
  m_scriptInterpreter->DoString( lua_function_myprint );
}

DebugConsole::~DebugConsole()
{}

const std::vector<std::string >&
DebugConsole::Lines() const
{ return m_lines; }

void 
DebugConsole::Clear()
{
  m_lines.clear();
  m_history.clear();
  m_currentHistoryLine = -2;
}

void
DebugConsole::AddLine( const std::string& line )
{
  m_lines.push_back( rtrim(line) );
}

void 
DebugConsole::PushToGui()
{
  const int32 MAX_ROWS_TO_SHOW = 16;
  const int32 rows_to_show     = std::min<int32>( m_lines.size(), MAX_ROWS_TO_SHOW - 1 );

  std::string textToPush;
  std::for_each( m_lines.end() - rows_to_show, m_lines.end(), [&](std::string& str){
    textToPush += str;
    textToPush += "\n";
  });

  textToPush += m_currentLine;

  m_consoleText->SetText( textToPush );
}

void 
DebugConsole::KeyPressed(KeyCode keyCode, Flags<ModKey> modKeys)
{
  if( keyCode == VKC_ESCAPE )
  {
    //@todo close the debugConsole?
    int32 breakpoint = 12;
  }
  else if( keyCode == VKC_UP )
  {
    if( m_currentHistoryLine == -2 )
    {
      m_currentHistoryLine = m_history.size() - 1;
    }
    else
    {
      m_currentHistoryLine = std::min<int32>( ++m_currentHistoryLine, m_history.size() - 1 );
    }
    
    if( m_currentHistoryLine == -1 )
    {
      m_currentLine.clear();
    }
    else
    {
      m_currentLine = m_history[m_currentHistoryLine]; 
    }
  }
  else if( keyCode == VKC_DOWN )
  {
    if( m_currentHistoryLine == -2 )
    {
      m_currentHistoryLine = m_history.size() - 1;
    }
    else
    {
      m_currentHistoryLine = std::max<int32>( --m_currentHistoryLine, -1 );
    }

    if( m_currentHistoryLine == -1 )
    {
      m_currentLine.clear();
    }
    else
    {
      m_currentLine = m_history[m_currentHistoryLine];
    }
  }
  else
  {
    uint8 asciiValue[2];
    int32 index     = 0; 
    int32 converted = m_keyboard.ConvertKeycodeToAscii( keyCode, modKeys, asciiValue );

    while( index < converted )
    {
      uint8 character = asciiValue[index];

      if( std::isprint(character) )
      {
        m_currentLine += character; //asciiValue[index];
      }
      else if( character == 0x08 ) //< Backspace
      {
        if( m_currentLine.size() > 0 )
        {
          m_currentLine.pop_back();
        }
      }
      else if( character == 0x09 ) //< Tab
      {
        const uint8 TAB_LENGTH  = 4;
        const uint8 WHITE_SPACE = 0x20; 

        // Check if we are currently at a tab stop and if so force atleast one space.
        if( m_currentLine.size() % TAB_LENGTH == 0 )
        {
          m_currentLine.push_back( WHITE_SPACE );
        }

        // Fill the current position to the next tab stop.
        while( m_currentLine.size() % TAB_LENGTH != 0 )
        {
          m_currentLine.push_back( WHITE_SPACE );
        }
      }
      else if( character == 0x0A ) //< Line Feed
      {
        m_currentLine.push_back( '\n' );
      }
      else if( character == 0x0D ) //< Carriage Return
      {
        if( modKeys & MOD_KEY_SHIFT ) // multi-line support
        {
          m_currentLine.push_back( '\n' );
        }
        else
        {
          AddToHistory( m_currentLine );
          
          if( !m_scriptInterpreter->DoString(m_currentLine.c_str()) )
          {
            std::string errorMessage = m_scriptInterpreter->GetErrorMessage();
            AddLine( errorMessage );
          }

          m_currentLine.clear();
        }
      }

      index++;
    }
  }
}

void 
DebugConsole::KeyReleased(KeyCode keyCode, Flags<ModKey> modKeys)
{}

int 
DebugConsole::InsertOutputFromLua( lua_State *L )
{
  // The interpreter is not accessable at this point so duplicating code from "LuaInterpreter::GetUserData" is
  // unfortunately unavoidable.
  lua_pushstring( L, "console" );
  lua_gettable( L, LUA_REGISTRYINDEX );                                         // pops the key value, pushes the value.
  DebugConsole* console = static_cast<DebugConsole* >(lua_touserdata( L, -1 )); // retrieve from the stack.
  lua_pop( L, 1 ); // pop value.

  // Attempt to add the message to the console.
  if( console != NULL )
  {
    const char* message = lua_tostring( L, -1 );
    NE_ASSERT( message != NULL, "Tried to print a null message." )();
    if( message != nullptr )
    { 
      console->AddLine( message );
    }
  }

  return 0;
}

int 
DebugConsole::ClearConsoleFromLua( lua_State *L )
{
  // The interpreter is not accessable at this point so duplicating code from "LuaInterpreter::GetUserData" is
  // unfortunately unavoidable.
  lua_pushstring( L, "console" );
  lua_gettable( L, LUA_REGISTRYINDEX );                                         // pops the key value, pushes the value.
  DebugConsole* console = static_cast<DebugConsole* >(lua_touserdata( L, -1 )); // retrieve from the stack.
  lua_pop( L, 1 ); // pop value.

  // Attempt to clear the console.
  if( console != NULL )
  {
    console->Clear();
  }

  return 0;
}

void 
DebugConsole::AddToHistory( const std::string& currentLine )
{
  m_history.push_back( currentLine );
  
  //@todo split current line on "\n" and push all the individual lines into the list.
  m_lines.push_back( currentLine );
 
  m_currentHistoryLine = -2;
}