--
--  Example v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Entity"
  kind "WindowedApp"
  
  includedirs {
    "./",
    "../../"
  }

  files {
    "**.cpp",
    "**.h"
  }

  links {
    "Common",
    "Alpha",
    "Beta"
  }

  utils.addLibrariesToCurrentProject( desc.dependencies );

  filter "action:vs*"
    flags{ "WinMain" }
  filter {}
