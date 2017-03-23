--
-- Alpha System Module v0.0.1 build script
--

local utils = require('build.utils')
local libaries = require('build.libraries')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Alpha"
  kind "StaticLib"
  language "C++"
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "Alpha"))
  
  if platform == 'ios' then
    utils.addXCodeBuildSettings()
  end

  includedirs {
    "./",
    "../../"
  }

  files {
    "**.cpp",
    "**.h"
  }

  utils.addLibrariesToCurrentProject( libaries, dependenciesRoot, platform )

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetsuffix '_d'
    targetdir ( path.join(baseLocation, "lib/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "lib/release") )
