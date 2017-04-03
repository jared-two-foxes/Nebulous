--
-- Common System Module v0.0.1 build script
--

local utils     = require('build.utils')
local libraries = require('build.libraries')

assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Common"
  kind "StaticLib"
  language "C++"
  location(path.join(solutionLocation, "Common"))

  -- if platform == 'ios' then
  --   utils.addXCodeBuildSettings()
  -- end

  files {
   "**.cpp",
   "**.h"
  }

  includedirs {
    "./",
    "../../"
  }

  removefiles {
    "UnitTests/**"
  }

  utils.addLibrariesToCurrentProject( libraries, dependenciesRoot, platform  )

  filter "action:xcode*"
    removefiles
    {
      "Profile/**.h",
      "Profile/**.cpp",
      "Window/Win32/**.h",
      "Window/Win32/**.cpp"
    }
    files {
      "**.mm"
    }

  filter "configurations:debug"
    flags { "Symbols" }
    targetsuffix '_d' 
    targetdir ( path.join(baseLocation, "lib/debug") )

  filter "configurations:release"
    flags { "Optimize" }
    targetdir ( path.join(baseLocation, "lib/release") )
