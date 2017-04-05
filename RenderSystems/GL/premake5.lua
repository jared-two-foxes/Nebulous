--
-- OpenGL Render Module v0.0.1 build script
--

local utils     = require('build.utils')
local libraries = require('build.libraries')
local tablex    = require('build.tablex')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "opengl_3"
  kind "SharedLib"
  language "C++"
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "GL3"))

  includedirs {
    "./",
    "./Include",
    "../../" -- Root Nebulae include path?
  }

  files {
    "main.cpp",
    "Config.h",
    "Sources/**.cpp",
    "Includes/**.hpp",
    "Includes/**.h"
  }

  links {
    "Common",
    "Alpha",
  }

  local var                = path.getdirectory( _SCRIPT ) .. '/libraries.lua'
  local project_libraries  = dofile( var )
  local current_libraries  = tablex.copy( libraries );
  local combined_libraries = tablex.union( current_libraries, project_libraries )

  utils.addLibrariesToCurrentProject( combined_libraries, dependenciesRoot, platform )

  filter "action:vs*"
    files {
      "Sources/Win32/**.cpp"
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