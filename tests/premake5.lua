--
--  Nebulae UnitTests v0.0.1 build script
--

local utils     = require('build.utils')
local libraries = require('build.libraries')
local tablex    = require('build.tablex')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )

-----------------------------------------------------------------------------------------------------------------------------------------------------

group "Tests"

project "UnitTests"
  kind "ConsoleApp"
  language "C++"
  location( path.join(solutionLocation, "UnitTests") )

  files {
    "**.cpp",
    "**.h"
  }

  includedirs {
    "./",
    rendererIncludeLocations
  }

  libdirs {
    path.join(baseLocation, rendererLibraryLocation )
  }

  links {
    "Common",
    "Audio",
    "Alpha",
    "Beta",
    rendererLibraryNames
  }

  local combined_libraries = {}
  local var                = path.getdirectory( _SCRIPT ) .. '/libraries.lua'
  local is_file            = os.isfile( var )
  if is_file then 
    local project_libraries  = dofile( var )
    local current_libraries  = tablex.copy( libraries );
    combined_libraries = tablex.union( current_libraries, project_libraries )
  else
    combined_libraries = libraries
  end

  utils.addLibrariesToCurrentProject( combined_libraries, dependenciesRoot, platform )

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetdir ( path.join(baseLocation, "bin/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release" ) )





group ""