--
--  lua_sample v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "lua_sample_lib"  
  kind "StaticLib"

  includedirs {
    "./",
    "../../"
  }

  includedirs {
    "./Source"
  }

  files {
    "Source/**.cpp",
    "Source/**.h"
  }
   
  -- exclude entry files.
  excludes {
    "Source/main.cpp",
  }

  utils.addLibrariesToCurrentProject( desc.dependencies );



-- -----------------------------------------------------------------------------------------------------------------------------------------------------
-- project "lua_sample_test"  
--   kind "ConsoleApp"

--   includedirs {
--     "./",
--     "../../",
--     "./Source",
--     "./Test"
--   }

--   files {
--     "Test/**.cpp",
--     "Test/**.h"
--   }

--   links {
--     "Common",
--     "Alpha",
--     "Beta",
--     "lua_sample_lib"
--   }


--   utils.addLibrariesToCurrentProject( desc.dependencies );


-----------------------------------------------------------------------------------------------------------------------------------------------------

project "lua_sample_app"  
  kind "WindowedApp"

  includedirs {
    "./",
    "../../",
    "./Source",
  }

  files {
    "Source/main.cpp"
  }

  links {
    "Common",
    "Alpha",
    "Beta",
    "lua_sample_lib"
  }

  filter "action:vs*"
    flags{ "WinMain" }
  filter {}

  utils.addLibrariesToCurrentProject( desc.dependencies );
