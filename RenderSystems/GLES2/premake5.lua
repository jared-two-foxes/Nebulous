--
-- OpenGL ES 2.0 Module v0.0.1 build script
--

local utils     = require('build.utils')
local libraries = require('build.libraries')
local tablex    = require('build.tablex')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "gles_2_0"
  kind "SharedLib"
  language "C++"
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "GLES2"))
  
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
    "Alpha"
  }

  local current_libraries = tablex.copy( libraries );
  local var               = path.getdirectory(_SCRIPT) .. '/libraries.lua'
  local project_libraries = dofile( var )
  local libraries         = tablex.union( current_libraries, project_libraries )

  utils.addLibrariesToCurrentProject( libraries, dependenciesRoot, platform )

  filter "action:vs*"
    files {
      "Win32/**.h",
      "Win32/**.cpp"
    }

  filter 'configurations:Debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetsuffix '_d'
    targetdir ( path.join(baseLocation, "Bin/Debug") )

  filter 'configurations:Release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "Bin/Release") )    