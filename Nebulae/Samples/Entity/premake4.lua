--
--  Example v0.0.1 build script
--

local utils = require('build.utils')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Entity"
  kind "WindowedApp"
  language "C++"
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "Entity"))
  
  includedirs {
    "."
  }

  files {
    "**.cpp",
    "**.h"
  }

  libdirs {
    path.join(baseLocation, rendererLibraryLocation)
  }

  links {
    "Common",
    "Alpha",
    "Beta",
    rendererLibraryNames
  }

  utils.addLibsToCurrentProject( dependenciesRoot, platform );

  filter 'action:xcode*'
    utils.addXCodeBuildSettings()

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetdir ( path.join(baseLocation, "bin/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release") )