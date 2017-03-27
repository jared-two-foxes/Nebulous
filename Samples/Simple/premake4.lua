--
-- Simple example build script
--

local utils = require('build.utils')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )
local projectLocation = path.join(solutionLocation, "Simple") 

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Simple"
  kind "WindowedApp"
  language "C++"
  location( projectLocation )

  files 
  {
    "**.mm",
    "**.cpp",
    "**.h"
  }

  excludes {
    "main.cpp"
  }

  utils.addLibsToCurrentProject(dependenciesRoot, platform)

  -- adds files and _folders_ as resources
  -- if platform == 'ios' then
  --   utils.setResourcesDir("../Media")
  -- end

  configuration 'debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetdir ( path.join(baseLocation, "lib/debug" ) )

  configuration 'release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "lib/release" ) )


  

	

