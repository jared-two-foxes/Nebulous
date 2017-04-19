
local utils = require 'build.utils'


local build = {}

local function SetupEnvironment()
  ok, err = os.chdir( "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build" )
  if ( not ok ) then
    print( "err: " .. err )
  end
  print( os.getcwd() );
  os.execute( "vcvarsall x86_amd64" )
end

function build.compile( libraries, libsRootPath )
  SetupEnvironment()

  for libName, libDef in pairs( libraries ) do
    local rootPath, projectPath, includePath, libPath, buildEngine, libNameRelease, libNameDebug = utils.unpackLibPlatformInfo( libsRootPath, libDef )

    if ( buildEngine ~= nil ) then
      print( "===========================================================" )
      print( "Building " .. libName )

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
          local cmd = buildEngine .. ' -G"Visual Studio 15 2017 Win64" ..'
          
          -- Preload the CMake Cache file. (Configure step).
          print( cmd )
          os.execute( cmd ) 

          -- Generate solution
          cmd = cmd .. " -DCMAKE_INSTALL_PREFIX=" .. rootPath .. '/build'
          print( cmd )
          os.execute( cmd ) -- generate

          -- Build the project debug and release configurations
          os.execute( buildEngine .. " --build . --target install --config debug" )
          os.execute( buildEngine .. " --build . --target install --config release" )
        elseif ( buildEngine == "boost.build" ) then
          -- build both debug and release variants 
          os.execute( "b2 address-model=64 architecture=x86 variant=debug,release link=static threading=multi --prefix=. headers" )
          os.execute( "b2 address-model=64 architecture=x86 variant=debug,release link=static threading=multi stage" )
          os.execute( "b2 address-model=64 architecture=x86 variant=debug,release link=static threading=multi --prefix=. install" )
        end 
      end
      print("")
    end
  end
end

return build