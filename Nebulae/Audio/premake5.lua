
--
-- Nebulae Audio Component build script
-- v0.0.1
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Audio"
  kind "StaticLib"

  files {
    "**.cpp",
    "**.h"
  }

  utils.addLibrariesToCurrentProject( desc.dependencies );