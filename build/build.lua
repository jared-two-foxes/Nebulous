
local utils = require 'build.utils'


local build = {}

local function SetupEnvironment()
  ok, err = os.chdir( "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build" )
  if ( not ok ) then
    print( "err: " .. err )
  end
  print( os.getcwd() );
  os.execute( "vcvarsall amd64" )
end

function build.compile( libraries, libsRootPath )
  for libName, libDef in pairs( libraries ) do
    local rootPath, projectPath, includePath, libPath, buildEngine, libNameRelease, libNameDebug = utils.unpackLibPlatformInfo( libsRootPath, libDef )

    if ( buildEngine ~= nil ) then
      print( "===========================================================" )
      print( "Building " .. libName )

      SetupEnvironment()

      -- Enter the Root directory for this dependency
      ok, err = os.chdir( rootPath )
      if( not ok ) then
        print( "err:" .. err )
      else
        print( os.getcwd() )
        if( buildEngine == "cmake" ) then
          -- Delete any existing build directory to make sure that we have a "clean" build
          if os.isdir( "build" ) then
            ok, err = os.rmdir( rootPath .. "/build" )
            -- if( not ok ) then
            --   print( "err: " .. err )
            -- end
          end

          -- create the directory and enter it
          os.mkdir( "build" )
          os.chdir( "build" )

          -- Run the solution builder process
          local cmd = buildEngine .. ' -G"Visual Studio 15 2017 Win64"' .. ' -DCMAKE_INSTALL_PREFIX="' .. rootPath .. '/build" -DCMAKE_DEBUG_POSTFIX="d"'
  
          -- @todo - Seperate the Preload the CMakeCache file. (Configure step). ??
          -- Generate solution
          print( cmd .. " .." )
          os.execute( cmd .. " .." )

          local buildCmd = buildEngine .. " --build . --target install"
          
          -- Build the project debug and release configurations
          print( buildCmd .. " --config debug" )
          os.execute( buildCmd .. " --config debug" )
          print( buildCmd .. " --config release" )
          os.execute( buildCmd .. " --config release" )
        elseif ( buildEngine == "boost.build" ) then
          -- Modular boost via git requires that we call this to copy all them headers to the right location.
          os.execute( "b2 headers" )
          
          -- Build and install!
          os.execute( "b2 --toolset=msvc-14.1 --build-type=complete --address-model=64 --link=static --threading=multi --prefix=built -j8 install" )
          --os.execute( "b2 --toolset=msvc-14.1 --build-type=complete address-model=64 threading=multi link=static runtime-link=static" )
        end 
      end
      print("")
    end
  end
end

return build