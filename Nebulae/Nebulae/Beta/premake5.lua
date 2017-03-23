--
-- Beta System Module v0.0.1 build script
--

local utils = require('build.utils')
local libaries = require('build.libraries')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Beta"
  kind "StaticLib"
  language "C++"

  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "Beta"))

  -- if platform == 'ios' then
  --   utils.addXCodeBuildSettings()
  -- end

  files {
    "**.cpp",
    "**.h"
  }

  includedirs {
    "../",
    "../../"
  }

  removefiles {
    "UnitTests/**"
  }

  utils.addLibrariesToCurrentProject( libaries, dependenciesRoot, platform )

  configuration "xcode*"
    files {
      "**.mm"
    }
    excludes {
      "Platform/Win32/**.h",
      "Platform/Win32/**.cpp"
    }

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetsuffix '_d'
    targetdir ( path.join(baseLocation, "lib/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "lib/release") )
