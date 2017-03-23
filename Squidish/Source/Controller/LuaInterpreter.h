#ifndef __LUAINTERPRETER_H__
#define __LUAINTERPRETER_H__

#include <Nebulae/Common/Common.h>

#include <lua.hpp>

namespace Game {

class LuaInterpreter
{
private:
  lua_State*  m_L;
  std::string m_errorMessage;

  public:
    LuaInterpreter();
    ~LuaInterpreter();

    bool Initialize();
    void RegisterFunction( const char* name, lua_CFunction function );
    bool DoString( const char* line );

    bool HasErrorMessage() const;
    const std::string& GetErrorMessage() const;

    //Lua UserData
    void  RegisterUserData( const char* identifier, void* userdata );
    void* GetUserData( const char* identifier ) const;

};

}

#endif // __LUAINTERPRETER_H__