--
--  Nebulae v0.0.1 build script
--

dependenciesRoot = 'C:/Users/Jared Watt/Documents/Develop/Nebulous/Externals'
-- package.path = package.path .. ";" .. dependenciesRoot

local utils    = require('build.utils')
local libaries = require('build.libraries')

-----------------------------------------------------------------------------------------------------------------------------------------------------

newoption {
  trigger     = "clean",
  description = "Will clean all project files."
}

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

newoption {
  trigger     = "build_dependencies",
  description = "Builds the dependency projects.",
}

-----------------------------------------------------------------------------------------------------------------------------------------------------


if _ACTION == "vs2010" then
  platform = 'win'
elseif _ACTION == "vs2013" then
  platform = 'win'
elseif _ACTION == "vs2015" then
  platform = 'win'
elseif _ACTION == "xcode4" then --todo.  Need to wildcard match this string to xcode*
  if _OPTIONS['ios'] then
    platform = 'ios'
  else
    platform = 'osx'
  end
else
  print('Unrecogonised action found')
end

if _OPTIONS["clean"] then
  os.rmdir( 'bin' )
  os.rmdir( 'lib' )
  os.rmdir( 'Project' )
end

baseLocation             = path.getabsolute("./")
solutionLocation         = path.getabsolute("Project")
rendererIncludeLocations = ""
rendererLibraryLocation  = ""
rendererLibraryNames     = {}

if( false == os.isdir(solutionLocation) ) then
  os.mkdir( solutionLocation )
end

if _OPTIONS["build_dependencies"] then
  local build = require 'build.build'
  build.compile( libaries, dependenciesRoot )
end

-----------------------------------------------------------------------------------------------------------------------------------------------------

workspace "Nebulae"
  configurations { "debug", "release" }
  language "C++"
  location( solutionLocation )  
  flags { "Symbols", "NoMinimalRebuild", "NoEditAndContinue", "FloatFast" }

  defines {
    "PREMAKE5",
    "NEBULAE_INCLUDES_PNG"
  }

  includedirs {
    "./"
  }

  filter "configurations:debug"
    defines { "DEBUG", "_DEBUG" }
    targetdir ( path.join(baseLocation, "bin/debug") )
  
  filter "configurations:release"
    defines { "NDEBUG" }
    flags   { "Optimize" } 
    targetdir ( path.join(baseLocation, "bin/debug") )

  filter "action:vs*"
    defines{
      "NOMINMAX",
      "WIN32",
      "_WIN32" 
    }

    architecture ( "x86_64" )
    buildoptions { "/EHsc", "/MP" }
    --flags        { 'FatalWarnings' }

    -- ignore some warnings
    linkoptions {
      '/ignore:4221', -- This object file does not define any previously undefined public symbols, so it will not be used by any link operation that consumes this library
      '/WX',          -- treat all other warnings as errors
    }

    buildoptions {
      -- completely useless warning
      '/wd4061', -- enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
      '/wd4100', -- 'identifier' : unreferenced formal parameter
      '/wd4121', -- 'symbol' : alignment of a member was sensitive to packing
      '/wd4127', -- conditional expression is constant
      '/wd4201', -- nonstandard extension used : nameless struct/union (NB: actively used, supported on all platforms)
      '/wd4324', -- 'struct_name' : structure was padded due to __declspec(align())
      '/wd4505', -- 'function' : unreferenced local function has been removed
      '/wd4510', -- 'class' : default constructor could not be generated
      '/wd4512', -- 'class' : assignment operator could not be generated
      '/wd4514', -- 'function' : unreferenced inline function has been removed    
      '/wd4571', -- Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
      '/wd4625', -- 'derived class' : copy constructor could not be generated because a base class copy constructor is inaccessible
      '/wd4626', -- 'derived class' : assignment operator could not be generated because a base class assignment operator is inaccessible
      '/wd4668', -- 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
      '/wd4710', -- 'function' : function not inlined
      '/wd4711', -- function 'function' selected for inline expansion
      '/wd4820', -- 'bytes' bytes padding added after construct 'member_name'
      '/wd4351', -- new behavior: elements of array will be default initialized
      '/wd4619', -- warning that does not exist was disabled, boost's fault!
      '/wd4555', -- expression has no effect; expected expression with side-effect [Boost]
      -- type system warnings    
      '/wd4242', -- 'identifier' : conversion from 'type1' to 'type2', possible loss of data
      '/wd4244', -- 'conversion' : conversion from 'type1' to 'type2', possible loss of data
      '/wd4245', -- 'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
      '/wd4826', -- 'conversion' : conversion from 'type1' to 'type2' is sign-extended.
      '/wd4345', -- 'conversion' : an object of POD type constructed with an initializer of the form () will be default initialized
      '/wd4365', -- 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
      '/wd4389', -- 'operator' : signed/unsigned mismatch
      -- candidates for enabling
      '/wd4917', -- 'declarator' : a GUID can only be associated with a class, interface or namespace (NB: triggers warnings in system ocidl.h)
      '/wd4548', -- expression before comma has no effect; expected expression with side-effect (NB: would like to have it enabled, but malloc.h triggers it =/ )
      '/wd4640', -- 'instance' : construction of local static object is not thread-safe
      '/wd4255', -- 'function' : no function prototype given: converting '()' to '(void)'
      '/wd4263', -- 'function' : does not override any virtual member function
      '/wd4264', -- 'function' : no override any virtual member function
      '/wd4265', -- 'class' : class has virtual functions, but destructor is not virtual
      '/wd4266', -- 'function' : no override available for virtual member function from base 'type'; function is hidden (NB: a derived class did not override all overloads of a virtual function)
      '/wd4191', -- 'type cast' : unsafe conversion : cause = boost, in all it's evil glory.
      '/wd4310', -- cast truncates constant value - see CasualCoreOnline/Internal/Source/MemoryMgrInternal.cpp line 34
      '/wd4005', -- for GLOT: USING_GLOTV3 redefined
      '/wd4350',
      '/wd4996',
      '/wd4062',
      '/wd4702',
      '/wd4189',
      '/wd4706', -- assignment within a conditional
      '/wd4987', -- assignment within a conditional

      '/wd4628', -- gmock
      '/wd4373', -- previous versions of the compiler did not override when parameters only differed by const/volatile qualifiers
      '/wd5031', -- #pragma warning(pop): likely mismatch, popping warning state pushed in different file
      '/wd4623', -- default constructor was implicitly defined as deleted
      '/wd5026', -- move constructor was implicitly defined as deleted.
      '/wd5027', -- move assignment operator was implicitly defined as deleted
      '/wd4459', -- declaration of 'set' hides global declaration
      '/wd4458', -- declaration of 'width' hides class member

      '/wd4371', -- layout of class may have changed from a previous version of the compiler due to better packing of member 
    }

    -- warning-related defines
    defines {
      '_SCL_SECURE_NO_WARNINGS',
      '_CRT_SECURE_NO_WARNINGS',
      '_CRT_SECURE_NO_DEPRECATE',
    }

  filter {}

  -- Engine --
  include "Nebulae"

  -- Build plugins --
  if _OPTIONS["plugins"] then
    include "RenderSystems" -- RenderSystem Dynamic Libraries
  end
  
  -- Build tests --
  if _OPTIONS["tests"] then
    include "tests"
  end

  -- Samples --
  if _OPTIONS["Samples"] then
    include "Samples"
  end
