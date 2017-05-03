--
-- Common System Module v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Common"
  kind "StaticLib"

  files {
   "**.cpp",
   "**.h"
  }

  removefiles {
    "UnitTests/**"
  }

  utils.addLibrariesToCurrentProject( desc.dependencies )

