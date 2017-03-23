-----------------------------------------------------------------------------------------------------------------------------------------------------
if platform == 'win' then
project "Squidish_test"  
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
    "Squidish_lib",
    rendererLibraryNames
  }

  utils.addLibsToCurrentProject( dependenciesRoot, platform );

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }     
    targetdir ( path.join(baseLocation, "bin/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "bin/release") )
end
