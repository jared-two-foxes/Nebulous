--
-- OpenGL Render Module v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "opengl_3"
  kind "SharedLib"
  language "C++"
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location(path.join(solutionLocation, "GL3"))

  includedirs {
    "./",
    "./Include",
    "../../" -- Root Nebulae include path?
  }

  files {
    "main.cpp",
    "Config.h",
    "Sources/**.cpp",
    "Includes/**.hpp",
    "Includes/**.h"
  }

  links {
    "Common",
    "Alpha",
  }

  filter "action:vs*"
    files {
      "Sources/Win32/**.cpp"
    }

  filter {}
  
  utils.addLibrariesToCurrentProject( desc.dependencies )
