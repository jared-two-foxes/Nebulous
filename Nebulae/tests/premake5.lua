--
--  Nebulae UnitTests v0.0.1 build script
--

local utils = require('build.utils')
local libraries = require('build.libraries')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )

-----------------------------------------------------------------------------------------------------------------------------------------------------

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

  utils.addLibrariesToCurrentProject( libraries, dependenciesRoot, platform );

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetdir ( path.join(baseLocation, "bin/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release" ) )