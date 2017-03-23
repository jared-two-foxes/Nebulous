#ifndef __LUAINTERPRETER_H__
#define __LUAINTERPRETER_H__

#include <Nebulae/Common/Common.h>

#include <lua.hpp>

namespace Nebulae {

class LuaInterpreter
{
private:
  lua_State*  m_L;
  std::string m_errorMessage;

  public:
    LuaInterpreter();
    ~LuaInterpreter();

    bool Initialize();

    bool HasErrorMessage() const;
    const std::string& GetErrorMessage() const;

    void RegisterFunction( const char* name, lua_CFunction function );

    //Lua UserData
    void RegisterUserData( const char* identifier, void* userdata );
    void* GetUserData( const char* identifier ) const;

    void LoadScript();

    bool DoString( const char* line );

};

}

#endif // __LUAINTERPRETER_H__