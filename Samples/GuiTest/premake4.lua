--
--  Example v0.0.1 build script
--

local utils = require('build.utils')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )
local projectLocation = path.join(solutionLocation, "GuiTest") 

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "GuiTest"
	kind "WindowedApp"
  language "C++"  
  location( projectLocation )

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

  configuration 'debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetdir ( path.join(baseLocation, "bin/debug") )

  configuration 'release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release") )