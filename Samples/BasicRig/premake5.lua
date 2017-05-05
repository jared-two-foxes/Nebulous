--
--  Example v0.0.1 build script
--

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "BasicRig"
  kind "WindowedApp"

  includedirs {
    "./",
    "../../"
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
