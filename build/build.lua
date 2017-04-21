
local utils = require 'build.utils'


local build = {}

local function SetupEnvironment()
  ok, err = os.chdir( "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build" )
  if ( not ok ) then
    print( "err: " .. err )
  end
  os.execute( "vcvarsall x64" )
  print()
end

-- @todo - Seperate the Preload the CMakeCache file. (Configure step). ??
-- @todo - Allow for clean building of dependencies by deleting cached files ??
-- @todo - Seperate the build list from the library list...
function build.compile( libraries, libsRootPath )
  
  -- Setup the build environment
  SetupEnvironment()

  for libName, libDef in pairs( libraries ) do
    local rootPath, projectPath, includePath, libPath, buildEngine, libNameRelease, libNameDebug = utils.unpackLibPlatformInfo( libsRootPath, libDef )

    if ( buildEngine ~= nil ) then
      print( "===========================================================" )
      print( "Building " .. libName )
      print()


      -- Enter the Root directory for this dependency
      ok, err = os.chdir( rootPath )
      if( not ok ) then
        print( "err:" .. err )
      else

        if( buildEngine == "cmake" ) then
          -- Only delete the file if we are attempting to do a clean build? 
          -- Delete any existing build directory to make sure that we have a "clean" build
          -- if os.isdir( "build" ) then
          --   ok, err = os.rmdir( rootPath .. "/build" )
          -- end

          -- create the directory and enter it
          if not os.isdir( "build" ) then 
            os.mkdir( "build" )
          end

          -- Navigate to the build directory
          os.chdir( "build" )

          -- Run the solution builder process
          -- Attempt to build with the specified compilier
          -- Set the output directory to install the files.
          local cmd = buildEngine .. ' -G"Visual Studio 15 2017 Win64"' .. ' -DCMAKE_INSTALL_PREFIX="' .. rootPath .. '/built" -DCMAKE_DEBUG_POSTFIX="d"'
  
          -- Generate solution
          os.execute( cmd .. " .." )
          print()

          local buildCmd = buildEngine .. " --build . --target install"
          
          -- Build the project debug and release configurations
          os.execute( "msbuild install.vcxproj /p:configuration=debug;platform=x64;WarningLevel=0 /v:q" )
          print()
          os.execute( "msbuild install.vcxproj /p:configuration=release;platform=x64;WarningLevel=0 /v:q" )
          print()
          print( "done" )
        elseif ( buildEngine == "boost.build" ) then
          -- Modular boost via git requires that we call this to copy all them headers to the right location.
          os.execute( "b2 headers" )
          
          -- Build and install!
          os.execute( "b2 --toolset=msvc-14.1 --build-type=complete address-model=64 --architecture=ia64 --threading=multi --link=static --prefix=built -j8 install" )
          print()
          print( "done" )
        end 
      end
      print()
    end
  end
end

return build