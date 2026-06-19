#ifndef NEBULAE_LUA_SOURCE_LUA_LUAINTERPRETER_H_
#define NEBULAE_LUA_SOURCE_LUA_LUAINTERPRETER_H_

#include <Nebulae/Common/Common.h>

#include <lua.hpp>

namespace Nebulae
{

class LuaInterpreter
{
private:
  lua_State* m_L;
  std::string m_errorMessage;

public:
  LuaInterpreter();
  ~LuaInterpreter();

  bool Initialize();

  bool HasErrorMessage() const;
  const std::string& GetErrorMessage() const;

  void RegisterFunction( const char* name, lua_CFunction function );

  // Lua UserData
  void RegisterUserData( const char* identifier, void* userdata );
  void* GetUserData( const char* identifier ) const;

  void LoadScript();

  bool DoString( const char* line );
};

} // namespace Nebulae

#endif // NEBULAE_LUA_SOURCE_LUA_LUAINTERPRETER_H_