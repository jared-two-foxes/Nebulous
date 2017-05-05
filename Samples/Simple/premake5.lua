--
-- Simple example build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Simple"
  kind "WindowedApp"
  
  includedirs 
  {
    "./",
    "../../"
  }


  files 
  {
    "**.cpp",
    "**.hpp",
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
