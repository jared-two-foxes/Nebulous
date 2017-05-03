--
--  Nebulae v0.0.1 build script
--

root_dir = _SCRIPT_DIR

utils = require( 'utils' )
desc = require( 'project' )

-- Options & Actions -------------------------------------------------------------------------------------------------------------------------

newoption {
  trigger     = "plugins",
  description = "Add's renderer plugin projects to solution."
}

newoption {
  trigger     = "samples",
  description = "Add's to the solution the project files for the engine samples.",
}

newoption {
  trigger     = "tests",
  description = "Add's to the solution the unit test projects.",
}



-- Setup ------------------------------------------------------------------------------------------------------------------------------------------

local workspaceName = ""
if desc.solution ~= nil then
  workspaceName = desc.solution
else
  workspaceName = desc.name
end

baseLocation     = path.getabsolute( "./" )
solutionLocation = path.getabsolute( "Projects/" .. workspaceName )

if( false == os.isdir(solutionLocation) ) then
  os.mkdir( solutionLocation )
end



-- Workspace -------------------------------------------------------------------------------------------------------------------------------------

workspace( workspaceName )
  configurations { "Debug", "Release" }
  architecture "x86_64"
  language "C++"
  location( solutionLocation )  
  flags { "FloatFast" }

  defines {
    "PREMAKE5",
    "NEBULAE_INCLUDES_PNG"
  }

  includedirs {
    "./",
    --"./Source",
    "./include"
  }

  libdirs {
    "./lib"
  }

  filter "configurations:Debug"
    defines { "DEBUG", "_DEBUG" }
    targetsuffix 'd'
    symbols "On"
  
  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On" 

  filter "action:vs*"
    systemversion "10.0.14393.0"
    defines{
      "NOMINMAX",
      "WIN32",
      "_WIN32" 
    }

    buildoptions { "/EHsc", "/MP" }
    --flags        { 'FatalWarnings' }

  filter {}

  -- Engine --
  include "Nebulae"

  -- -- Build plugins --
  -- if _OPTIONS["plugins"] then
  --   include "RenderSystems" -- RenderSystem Dynamic Libraries
  -- end
  
  -- -- Build tests --
  -- if _OPTIONS["tests"] then
  --   include "tests"
  -- end

  -- -- Samples --
  -- if _OPTIONS["Samples"] then
  --   include "Samples"
  -- end
