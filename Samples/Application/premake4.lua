--
-- Application Sample build script
-- v0.0.1
--

local utils = require('build.utils')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )
local projectLocation = path.join(solutionLocation, "Application") 

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Application"
  kind "WindowedApp"
  language "C++"
  location( projectLocation )

  files {
    "**.h",
    "**.cpp"
  }

  links {
    "Common",
    "Alpha",
    "Beta"
  }

  if platform == 'ios' then
    -- ios specific code files.
    files {
      "**.m",
      "**.mm"
    }

    -- exclude win32 entry file.
    excludes {
      "main.cpp"
    }

    -- Add Resources    
    files {
      "Assets/**.fsh",
      "Assets/**.vsh",
      "Assets/**.storyboard",
      "Assets/**.png"
    }
    resources {
      "Assets/**.fsh",
      "Assets/**.vsh",
      "Assets/**.storyboard",
      "Assets/**.png"
    }
  end

  utils.addLibsToCurrentProject( dependenciesRoot, platform );

  configuration 'debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetdir ( path.join(baseLocation, "bin/debug") )

  configuration 'release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release") )
  