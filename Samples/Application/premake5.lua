--
-- Application Sample build script
-- v0.0.1
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Application"
  kind "WindowedApp"

  includedirs {
    "./",
  }

  files {
    "**.h",
    "**.cpp"
  }

  links {
    "Common",
    "Alpha",
    "Beta"
  }

  filter "action:vs*"
    flags{ "WinMain" }
  filter {}


  utils.addLibrariesToCurrentProject( desc.dependencies );

