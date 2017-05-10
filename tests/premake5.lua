--
--  Nebulae UnitTests v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

group "Tests"

project "UnitTests"
  kind "ConsoleApp"

  defines {
    "GTEST_LINKED_AS_SHARED_LIBRARY"
  }

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