
--
-- Nebulae Audio Component build script
-- v0.0.1
--

local utils = require('build.utils')
local libaries = require('build.libraries')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Audio"
  kind "StaticLib"
  language "C++"
  
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location( path.join(solutionLocation,"Audio") )
  
  
  includedirs {
    "./",
    "../../"
  }

  files {
    "**.h",
    "**.cpp"
  } 
  
  links {
    'Common'
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