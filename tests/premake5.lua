--
--  Nebulae UnitTests v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

group "Tests"

project "UnitTests"
  kind "ConsoleApp"

  files {
    "**.cpp",
    "**.h"
  }

  includedirs {
    "./"
  }

  links {
    "Common",
    "Audio",
    "Alpha",
    "Beta"
  }

  utils.addLibrariesToCurrentProject( desc.dependencies );

group ""