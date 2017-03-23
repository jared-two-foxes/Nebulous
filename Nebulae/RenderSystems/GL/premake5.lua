--
-- OpenGL Render Module v0.0.1 build script
--

local utils     = require('build.utils')
local tablex    = require('pl.tablex')
local libraries = require('build.libraries')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "opengl_3"
  kind "SharedLib"
  language "C++"
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "GL3"))
  
  -- if platform == 'ios' then
  --   utils.addXCodeBuildSettings()
  -- end

  defines {
    "FREEIMAGE_LIB"
  }

  includedirs {
    "./",
    "../",
    "../../../"
  }

  files {
    "config.h",
    "../Shared/**.cpp",
    "../Shared/**.h"
  }

  links {
    "Common",
    "Alpha",
    --"opengl32.lib",
  }



  local var               = path.getdirectory(_SCRIPT) .. '/libraries.lua'
  local project_libraries = dofile( var )
  local current_libraries = tablex.copy( libraries );
  
  utils.merge( current_libraries, project_libraries )

  -- print( "Adding Libraries to project - " .. dependenciesRoot )

  utils.addLibrariesToCurrentProject( current_libraries, dependenciesRoot, platform )

  filter "action:xcode*"
    files {
      "EAGL/**.h",
      "EAGL/**.mm"
    }

  filter "action:vs*"
    files {
      "Win32/**.h",
      "Win32/**.cpp"
    }

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetsuffix '_d'
    targetdir ( path.join(baseLocation, "bin/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release") )