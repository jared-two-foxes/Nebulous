--
--  lua_sample v0.0.1 build script
--

local utils = require('build.utils')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )
local projectLocation = path.join(solutionLocation, "Lua") 

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "lua_sample_lib"  
  kind "StaticLib"
  language "C++"
  location( projectLocation )

  if platform == 'ios' then
    utils.addXCodeBuildSettings()
  end

  includedirs {
    "./Source"
  }

  files {
    "Source/**.cpp",
    "Source/**.h"
  }
   
  -- exclude entry files.
  excludes {
    "Source/main.cpp",
    "Source/main.m"
  }

  configuration 'debug'
    defines { "DEBUG" }
    flags   { "Symbols" }
    targetdir ( path.join(baseLocation, "lib/debug" ) )

  configuration 'release'
    defines { "NDEBUG" }
    flags   { "Optimize" }      
    targetdir ( path.join(baseLocation, "lib/release") ) 


-----------------------------------------------------------------------------------------------------------------------------------------------------
if platform == 'win' then
project "lua_sample_test"  
  kind "ConsoleApp"
  language "C++"
  location( projectLocation )

  includedirs {
    "./Source",
    "./Test"
  }

  files {
    "Test/**.cpp",
    "Test/**.h"
  }

  libdirs {
    path.join(baseLocation, rendererLibraryLocation)
  }

  links {
    "Common",
    "Audio",
    "Alpha",
    "Beta",
    "lua_sample_lib",
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
end

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "lua_sample_app"  
  kind "WindowedApp"
  language "C++"
  location( projectLocation )

  includedirs {
    "./Source"
  }

if platform == 'ios' then 
  --  Official notes say that filelists are only supported at the project level thus this line cant go into a configuration 'xcode*'.
  files { 
    "Source/**.m*"
  }

  --utils.addXCodeInfoFile( projectLocation )
  utils.addXCodeBuildSettings()

  -- Add Resources    
  files {
    "Assets/**.anim",
    "Assets/**.atlas",
    "Assets/**.emitter",
    "Assets/**.glsl",
    "Assets/**.json",
    "Assets/**.material",
    "Assets/**.ogg",
    "Assets/**.png",
    "Assets/**.particle",
    "Assets/**.sprite",
    "Assets/**.storyboard",  
    "Assets/**.ttf"
  }
  resources {
    "Assets/**.anim",
    "Assets/**.atlas",
    "Assets/**.emitter",
    "Assets/**.glsl",
    "Assets/**.json",
    "Assets/**.material",
    "Assets/**.ogg",
    "Assets/**.png",
    "Assets/**.particle",
    "Assets/**.sprite",
    "Assets/**.storyboard",  
    "Assets/**.ttf"
  }

else
  files {
    "Source/main.cpp"
  }
end

  libdirs {
    path.join(baseLocation, rendererLibraryLocation)
  }

  links {
    "Common",
    "Audio",
    "Alpha",
    "Beta",
    "lua_sample_lib",
    rendererLibraryNames
  }

  utils.addLibsToCurrentProject( dependenciesRoot, platform );
  
  configuration 'debug'
    defines { "DEBUG" }
    flags   { "Symbols" }       
    targetdir ( path.join(baseLocation, "bin/debug") )

  configuration 'release'
    defines { "NDEBUG" }
    flags   { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release") )
