
--
-- Nebulae Audio Component build script
-- v0.0.1
--

local utils     = require('build.utils')
local libraries = require('build.libraries')
local tablex    = require('build.tablex')

-----------------------------------------------------------------------------------------------------------------------------------------------------

project "Audio"
  kind "StaticLib"
  language "C++"
  
  assert(solutionLocation, 'solutionLocation should be an absolute path set up in the main premake file')
  location( path.join(solutionLocation,"Audio") )
  
  
  includedirs {
    "./",
    "../../"
  }

  files {
    "**.h",
    "**.cpp"
  } 
  
  links {
    'Common'
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

  filter 'configurations:debug'
    defines { "DEBUG" }
    flags { "Symbols" }       
    targetsuffix '_d'
    targetdir ( path.join(baseLocation, "lib/debug") )

  filter 'configurations:release'
    defines { "NDEBUG" }
    flags { "Optimize" }    
    targetdir ( path.join(baseLocation, "lib/release") )