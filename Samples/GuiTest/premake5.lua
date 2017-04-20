--
--  Example v0.0.1 build script
--

local utils     = require('build.utils')
local libraries = require('build.libraries')
local tablex    = require('build.tablex')

assert( solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file' )
local projectLocation = path.join(solutionLocation, "GuiTest") 

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "GuiTest"
	kind "WindowedApp"
  language "C++"  
  location( projectLocation )

  includedirs {
    "./",
    "../../"
  }

  files {
  	"**.cpp",
  	"**.h"
  }

  links {
    "Common",
    "Alpha",
    "Beta"
  }

  local combined_libraries = {}
  local var                = path.getdirectory( _SCRIPT ) .. '/libraries.lua'
  local is_file            = os.isfile( var )
  if is_file then 
    local project_libraries  = dofile( var )
    local current_libraries  = tablex.copy( libraries );
    combined_libraries = tablex.union( current_libraries, project_libraries )
  else
    combined_libraries = libraries
  end

  utils.addLibrariesToCurrentProject( combined_libraries, dependenciesRoot, platform )

  filter "action:vs*"
    flags { "WinMain" }

  filter 'configurations:Debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetsuffix '_d'
    targetdir ( path.join(baseLocation, "Bin/Debug") )

  filter 'configurations:Release'
    defines { "NDEBUG" }
    flags { "Optimize" }      
    targetdir ( path.join(baseLocation, "Bin/Release") )