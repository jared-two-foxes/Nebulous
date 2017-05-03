--
-- Beta System Module v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Beta"
  kind "StaticLib"

  files {
    "**.cpp",
    "**.h"
  }

  utils.addLibrariesToCurrentProject( desc.dependencies );
