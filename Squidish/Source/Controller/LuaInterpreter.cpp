
#include "LuaInterpreter.h"

static const char g_sLuaRegistryGUID = 0;

#if defined(_DEBUG)
#define LUA_STACK_SET   int __luastackcheck = lua_gettop(m_L);
#define LUA_STACK_CHECK assert(lua_gettop(m_L) == __luastackcheck && "Lua Stack is not normalized, it is %d instead of %d"); //, lua_gettop(m_L), __luastackcheck);
#else
#define LUA_STACK_SET
#define LUA_STACK_CHECK
#endif

using namespace Nebulae;
using namespace Game;

LuaInterpreter::LuaInterpreter() 
  : m_L( NULL )
{}

LuaInterpreter::~LuaInterpreter()
{
  //release lua?
  lua_close(m_L); //< should we be doing this stuff in constructors and destructor's?
}

bool 
LuaInterpreter::HasErrorMessage() const
{ return (m_errorMessage.length() > 0); }

const std::string& 
LuaInterpreter::GetErrorMessage() const
{ return m_errorMessage; }

bool 
LuaInterpreter::Initialize()
{
  m_L = luaL_newstate();

  if( m_L != NULL )
  {
    luaL_openlibs(m_L);

  //#if defined(GAME_WIN32) && (defined(_DEBUG) || defined(DEBUG))
  //  LoadScript("socket.lua", false, false);
  //  LoadScript("mobdebug.lua", false, false);
  //#endif
  }

  return (m_L != NULL);
}

void 
LuaInterpreter::RegisterFunction( const char* name, lua_CFunction function )
{
  lua_register(m_L, name, function);
}

void 
LuaInterpreter::RegisterUserData( const char* identifier, void* userdata )
{
  LUA_STACK_SET

  lua_pushstring( m_L, identifier );
  lua_pushlightuserdata( m_L, userdata );
  lua_settable( m_L, LUA_REGISTRYINDEX ); // settable will pop off the top 2 elements in the stack so no need to clear them.

  LUA_STACK_CHECK
}

void* 
LuaInterpreter::GetUserData( const char* identifier ) const
{ 
  LUA_STACK_SET
  lua_pushstring( m_L, identifier );
  lua_gettable( m_L, LUA_REGISTRYINDEX ); // pops the key value, pushes the value.

  void* data = lua_touserdata( m_L, -1 ); // retrieve from the stack.

  lua_pop( m_L, 1 ); // pop value.
  LUA_STACK_CHECK

  return data; 
}

bool  
LuaInterpreter::DoString( const char* luacmd )
{
  LUA_STACK_SET

  if( luaL_dostring(m_L, luacmd) )
  {
    m_errorMessage = lua_tostring(m_L, -1);
    lua_pop(m_L, 1);
    
    LUA_STACK_CHECK

    return false;
  }

  LUA_STACK_CHECK

  return true;
}