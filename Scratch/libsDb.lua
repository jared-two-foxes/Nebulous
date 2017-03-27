-- Format: { projectPath, releaseStaticPath, debugStaticPath } 

return {
  freetype = {
    win = { 'freetype/builds/win32/vc2010/freetype.vcxproj', 'Win32/release/freetype.lib',      'Win32/debug/freetypeD.lib',     },
    ios = { nil,                                             'iOS/iphone/libfreetype.a',        nil,                             },
  },
  lua = {
    win = { 'lua-5.1.4/Lua.vcxproj'            ,             'Win32/release/lua.lib',           'Win32/debug/lua_d.lib',         },
    ios = { 'lua-5.1.4/maclua/maclua.xcodeproj',             'iOS/iphone/release/libmaclua.a',  'iOS/iphone/debug/libmaclua.a',  },
  },
  jsoncpp = {       
    win = { 'jsoncpp/project/msvc/jsoncpp.vcxproj',          'Win32/release/jsoncpp.lib',       'Win32/debug/jsoncpp.lib',        },
    ios = { 'jsoncpp/project/xcode/ios/jsoncpp.xcodeproj',   'iOS/iphone/release/libjsoncpp.a', 'iOS/iphone/debug/libjsoncpp.a',  },
  },
}
