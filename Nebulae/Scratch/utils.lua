local libsDb = require('lib.libsDb')
local defaultAbi = 'armeabi-v7a'

-- config is a map from libName -> linkType

-- LinkTypes:
--   static             - autodetects static library for each configuration
--   staticForceDebug   - links debug library for all configurations (if possible)
--   staticForceRelease - links release library for all configurations (if possible)
--   staticForceRetail  - links retail library for all configurations (if possible)
--   project            - adds as a project

local function tableContainsKey(tb, key)
  for k, v in pairs(tb) do
    if key == k then
      return true
    end
  end
  return false
end

local function tableContainsValue(tb, value)
  for k, v in pairs(tb) do
    if value == v then
      return true
    end
  end
  return false
end

-- return list of keys and list of values
local function tableUnzip(tb)
  local keys = {}
  local values = {}
  for k, v in pairs(tb) do
    table.insert(keys, k)
    table.insert(values, k)
  end
  return keys, values
end

serializingObjects = {}
local function serialize(o, newlineOffset, depthLimit)
  local t = type(o)
  if t == 'string' then
    return "'" .. o .. "'"
  elseif t == 'number' then
    return o .. ''
  elseif t == 'boolean' then
    if t then
      return 'true'
    else
      return 'false'
    end
  else
    -- global serializingObjects contains all objects being serialized
    if tableContainsValue(serializingObjects, o) then
      return '<BACKREF>'
    end

    local typeStr = '<' .. t .. '> '
    if t == 'table' then
      typeStr = ''
    end

    
    if not next(o) then
      return typeStr .. '{}'
    end

    local nextDepthLimit = nil
    if depthLimit ~= nil then
      if depthLimit == 0 then
        return '{ ... }'
      end
      nextDepthLimit = depthLimit - 1
    end    

    table.insert(serializingObjects, o)    

    if not next(o, next(o)) then
      k, v = next(o)
      if k == 1 then
        return typeStr .. '{' .. serialize(v, newlineOffset, nextDepthLimit) .. '}'
      end
    end


    local res = typeStr .. '{'
    if newlineOffset ~= nil then
      res = res .. '\n' .. newlineOffset .. '  '
    end

    local first = true
    for k, v in pairs(o) do
      if first then
        first = false
      else
        if newlineOffset ~= nil then
          res = res .. ',\n' .. newlineOffset .. '  '
        else
          res = res .. ', '
        end
      end

      if newlineOffset ~= nil then
        res = res .. serialize(k) .. ' = ' .. serialize(v, newlineOffset .. '  ', nextDepthLimit)
      else
        res = res .. serialize(k) .. ' = ' .. serialize(v, nil, nextDepthLimit)
      end
    end

    if newlineOffset ~= nil then
      res = res .. ',\n' .. newlineOffset .. '}'
    else
      res = res .. '}'
    end

    table.remove(serializingObjects, #serializingObjects)

    return res
  end
end

local function printTable(tb, multiline, depth)
  if multiline then
    print(serialize(tb, '', depth))
  else
    print(serialize(tb, nil, depth))
  end
end

local function endsWith(s, suffix)
  return #s >= #suffix and s:find(suffix, #s - #suffix + 1, true)
end

local function fileExists(path)
   local f = io.open(path, 'r')
   if f ~= nil then
     io.close(f) 
     return true 
   else 
     return false 
   end
end

local function copyFile(from, to)
  src = io.open(from, 'rb')
  dst = io.open(to, 'wb')
  content = src:read('*a')
  dst:write(content)
  src:close()
  dst:close()
end

local devnull = 'NUL' -- windows way =(
if fileExists('/dev/null') then -- everyone else
  devnull = '/dev/null'
end

local function splitPathName(path)
  local nameStart, _, name = path:find('([^/\\]+)[/\\]?$')
  local dir = path:sub(0, nameStart - 2)
  return dir, name
end                                  

local function splitExt(path)
  --path:find('[^/\\]\\.([^/\\.]*)$')
  local extStart, _, ext = path:find('[^/\\.][.]([^/\\.]*)$')
  if extStart ~= nil then
    local name = path:sub(0, extStart)
    return name, ext
  else
    return path, nil
  end
end

local function getUuidByRegexp(path, pattern)
  assert(fileExists(path), 'Unable to find project file: ' .. path)
  local uuid = nil

  for line in io.lines(path) do    
    local _, _, foundUuid = line:find(pattern)
    if foundUuid ~= nil then
      assert(uuid == nil, 'Second UUID/target found in file: ' .. path)
      uuid = foundUuid
    end
  end

  assert(uuid ~= nil, 'Unable to find UUID/target in project: ' .. path)
  return uuid
end

local function getUuid(projectPath)
  if endsWith(projectPath, '.vcxproj') then
    return getUuidByRegexp(projectPath, '<ProjectGuid>{([%x-]+)}</ProjectGuid>')
  elseif endsWith(projectPath, '.vcproj') then
    return getUuidByRegexp(projectPath, 'ProjectGUID="{([%x-]+)}"')
  elseif endsWith(projectPath, '.xcodeproj') then
    return getUuidByRegexp(projectPath .. '/project.pbxproj', 'rootObject%s*=%s*(%x+)')
  elseif endsWith(projectPath, '.mk') then -- for android it returns target name
    return getUuidByRegexp(projectPath, 'LOCAL_MODULE%s*:=%s*([^%s]+)')
  else
    assert(false, 'Unsupported project type: ' .. projectPath)
  end
end

local function getPatchPath(libsRootPath, libName, platform)
  return libsRootPath .. '/PremakePatches/' .. libName .. '.' .. platform .. '.patch'
end

local function unpackLibPlatformInfo(libsRootPath, libPlatformInfo, abi)
  if abi == nil then
    abi = defaultAbi
  end

  local retailLib, releaseLib, debugLib, projectPath, patchProject = unpack(libPlatformInfo)

  if retailLib   ~= nil then retailLib   = libsRootPath .. '/' .. string.gsub(retailLib,  '%%ABI%%', abi) end
  if releaseLib  ~= nil then releaseLib  = libsRootPath .. '/' .. string.gsub(releaseLib, '%%ABI%%', abi) end
  if debugLib    ~= nil then debugLib    = libsRootPath .. '/' .. string.gsub(debugLib,   '%%ABI%%', abi) end
  if projectPath ~= nil then projectPath = libsRootPath .. '/' .. projectPath end

  return retailLib, releaseLib, debugLib, projectPath, patchProject
end

local function checkLibs(libsRootPath, libsConfig)
  if libsConfig == nil then
    libsConfig = {} -- create fake config consists of all libs
    for libName, libInfo in pairs(libsDb) do
      if not tableContainsKey(libsConfig, libName) then
      	libsConfig[libName] = 'dummy'
      end
    end
  end

  for libName, _ in pairs(libsConfig) do
    assert(tableContainsKey(libsDb, libName), 'Library not found in db: ' .. libName)
  end

  for libName, libInfo in pairs(libsDb) do
    for platformName, libPlatformInfo in pairs(libInfo) do
      if tableContainsKey(libsConfig, libName) then
        local retailLib, releaseLib, debugLib, projectPath, patchProject = unpackLibPlatformInfo(libsRootPath, libPlatformInfo)

        if debugLib ~= nil then
          assert(fileExists(debugLib), 'Unable to find debug library: ' .. debugLib)
        end
        if releaseLib ~= nil then
          assert(fileExists(releaseLib), 'Unable to find release library: ' .. releaseLib)
        end
        if retailLib ~= nil then
          assert(fileExists(retailLib), 'Unable to find retail library: ' .. retailLib)
        end
        -- not necessary a file, xcode projects are folders, it's not that easy to check dir existence
        --if projectPath ~= nil then
        --  assert(fileExists(projectPath), 'Unable to find project: ' .. projectPath)
        --end
        if patchProject then
          assert(projectPath ~= nil, 'No project specified, but patch enabled for: ' .. libName)
          local patchPath = getPatchPath(libsRootPath, libName, platformName)
          assert(fileExists(patchPath), 'Unable to find library patch: ' .. patchPath)
        end
      end
    end
  end
end

local function deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, preferredLinkType, configuration)
  local retailLib, releaseLib, debugLib = unpackLibPlatformInfo(libsRootPath, libPlatformInfo)

  assert(preferredType ~= 'project', 'Internal inconsistency, trying to deduce static library name for project link type')

  if preferredLinkType == 'staticForceDebug' then
    return deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, 'static', 'debug')
  elseif preferredLinkType == 'staticForceRelease' then
    return deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, 'static', 'release')
  elseif preferredLinkType == 'staticForceRetail' then
    return deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, 'static', 'retail')
  elseif preferredLinkType == 'static' then
    local orderedTable = nil
    if configuration == 'debug' then
      orderedTable = { debugLib, releaseLib, retailLib }
    elseif configuration == 'release' then
      orderedTable = { releaseLib, retailLib }
    elseif configuration == 'retail' then
      orderedTable = { retailLib, releaseLib }
    else
      assert(false, 'Unsupported configuration name: ' .. configuration)
    end    

    for i, libPath in pairs(orderedTable) do
      if libPath ~= nil then
        assert(fileExists(libPath), 'Unable to find library at path: ' .. libPath)
        return libPath
      end
    end

    assert(false, 'Unable to find suitable static library to link: ' .. libName .. ' for configuration: ' .. configuration)
  else
    assert(false, 'Unsupported link type: ' .. preferredLinkType)
  end
end

local function upgradeProject(projectPath)
  assert(endsWith(projectPath, '.vcproj'), "Don't know how to upgrade project: " .. projectPath)

  local projectName, projectExt = splitExt(projectPath)
  local upgradedProjectPath = projectName .. '.vcxproj'

  if fileExists(upgradedProjectPath) then
    os.remove(upgradedProjectPath)
  end

  local rc = os.execute('@call "%VS100COMNTOOLS%/vcupgrade.exe" -nologo -nocolor -overwrite ' .. projectPath .. ' > ' .. devnull)
  assert(rc == 0, 'Unable to upgrade lib project: ' .. projectPath)
  assert(fileExists(upgradedProjectPath), 'Unable to upgrade visual studio project: ' .. projectPath)

  return upgradedProjectPath
end

local function getFilesAffectedByPatch(patchPath)
  local results = {}
  for line in io.lines(patchPath) do
    local _, _, foundFilePath = line:find('^Index: (.*)$')
    if foundFilePath ~= nil then
      table.insert(results, foundFilePath)
    end
  end
  return results
end

local function patchProjectInplace(libsRootPath, libName, platform, projectPath)
  local patchPath = getPatchPath(libsRootPath, libName, platform)

  print('Patching ' .. libName .. ' using ' .. patchPath)
  -- trying to apply reversed patch first, to revert possible changes from previous run
  os.execute('patch --force -p0 -R -d ' .. libsRootPath .. ' < ' .. patchPath .. ' > ' .. devnull)

  local rc = os.execute('patch -p0 -N -d ' .. libsRootPath .. ' < ' .. patchPath .. ' > ' .. devnull)
  if (rc ~= 0) then -- patch might be already applied, if so trying to reverse and reapply patch
    -- we might have svn conflict after update, try to revert
    print('Unable to patch ' .. libName .. ' using ' .. patchPath .. ', maybe svn has unresolved conflict, trying to revert local changes and apply patch once more')

    for _, affectedFile in pairs(getFilesAffectedByPatch(patchPath)) do
      affectedFile = libsRootPath .. '/' .. affectedFile
      if fileExists(affectedFile) then
        print('Trying to revert ' .. affectedFile)
        rc = os.execute('svn --non-interactive revert ' .. affectedFile)
        assert(rc == 0, 'Revert failed. Unable to patch lib: ' .. libName)
      end
    end

    rc =     os.execute('patch -p0 -N -d ' .. libsRootPath .. ' < ' .. patchPath .. ' > ' .. devnull)
    assert(rc == 0, 'Still unable to patch lib: ' .. libName)
  end
end

local function addProjectsToCurrentSolution(libsRootPath, platform, config)
  for libName, linkType in pairs(config) do
    local libDef = libsDb[libName]
    assert(libDef ~= nil, 'Unable to find library in database: ' .. libName)
    local libPlatformInfo = libDef[platform]

    if libPlatformInfo ~= nil and linkType == 'project' then
      local retailLib, releaseLib, debugLib, projectPath, patchProject = unpackLibPlatformInfo(libsRootPath, libPlatformInfo)
      assert(projectPath ~= nil, 'Project link type required, but no project found for lib: ' .. libName)

      if endsWith(projectPath, '.vcproj') then
        projectPath = upgradeProject(projectPath)
      end

      local fileDir, fileName = splitPathName(projectPath)
      local fileBaseName, fileExt = splitExt(fileName)
      local uuidValue = getUuid(projectPath)
      assert(uuidValue ~= nil, 'Unable to get UUID from project: ' .. projectPath)
              
      -- premake calls
      external(fileBaseName)
    	  location(fileDir)
  	    uuid(uuidValue)
  	    kind('StaticLib')
  	    language('C++')

  	  print('Added external lib: ' .. libName .. ' at ' .. projectPath)
  	end
  end
end

local function addLibsToCurrentProject(libsRootPath, platform, config)
	local linksAbsolute = nil
  if platform == 'ios' then  -- for xcode linker we can't specify library by absolute path like -l/path/to/lib
		linksAbsolute = function (libs) -- we have to specify library name without -l instead, to do so we use linkoptions instead of links
			for _, l in pairs(libs) do
				linkoptions(path.getabsolute(l))
      end
    end
	else
    linksAbsolute = links
  end

  for libName, linkType in pairs(config) do
    local libDef = libsDb[libName]
    assert(libDef ~= nil, 'Unable to find library in database: ' .. libName)
    local libPlatformInfo = libDef[platform]

    if libPlatformInfo ~= nil then
      local retailLib, releaseLib, debugLib, projectPath, patchProject = unpackLibPlatformInfo(libsRootPath, libPlatformInfo)
      if linkType == 'project' then
				local fileDir, fileName = splitPathName(projectPath)
        local fileBaseName, fileExt = splitExt(fileName)
        local projectTarget = fileBaseName
        
        -- premake calls
				links { projectTarget } 
      else
        local debugLib   = deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, linkType, 'debug')
        local releaseLib = deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, linkType, 'release')
        local retailLib  = deduceLinkLibrary(libsRootPath, libName, libPlatformInfo, linkType, 'retail')

        -- premake calls
        configuration 'Debug'
          linksAbsolute { debugLib }
        configuration 'Release'
          linksAbsolute { releaseLib }
        configuration 'Retail'
          linksAbsolute { retailLib }
        configuration {} -- reset configuration
      end
    end
  end
end

local function getPossibleRebuildTargets()
  local libNames = tableUnzip(libsDb)
  local res = {}

  for _, libName in pairs(libNames) do
    table.insert(res, {libName, libName})
  end
  table.insert(res, {'all', 'Rebuild all known libraries'})
  table.insert(res, {'linked', 'Rebuild linked to current project'})
  return res
end

local libNotFoundExplanation = '' ..
  "Build reported to be successfull, but premake is unable to find built library at expected path.\n" ..
  "Most likely that's becuase project creates library with other name than expected.\n" ..
  "For various reasons (see comments in source) premake can't change output library name, it can only change output library dir.\n" ..
  "To fix this issue, please change library name in DB to match actually generated file name.\n" ..
  "If project has the same name for debug and release configurations, please put libraries into different subfolders\n" ..
  "Expected location: "

local function rebuildLibMsvc(libName, releaseLib, debugLib, projectPath, buildDefines)
  local debugLibDir, debugLibName         = splitPathName(debugLib)
  local debugLibBaseName, debugLibExt     = splitExt(debugLibName)
  local releaseLibDir, releaseLibName     = splitPathName(releaseLib)
  local releaseLibBaseName, releaseLibExt = splitExt(releaseLibName)

  local msbuildPath = '%SystemRoot%/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe'

  -- Overriding TargetName/TargetExt doesn't work for projects that override property Lib.OutputFile
  -- Lib.OutputFile takes precedence and cannot be overriden from cmdline
  --   Following projects known to suffer from that: glsocial, slimxml, zlib, tracking, etc

  -- local rebuildDebugCmdline   = msbuildPath .. ' "' .. projectPath .. '" /p:Configuration=Debug   /property:OutDir="' .. path.getabsolute(debugLibDir)   .. '/" /property:TargetName=' .. debugLibBaseName   .. ' /property:TargetExt=.' .. debugLibExt
  -- local rebuildReleaseCmdline = msbuildPath .. ' "' .. projectPath .. '" /p:Configuration=Release /property:OutDir="' .. path.getabsolute(releaseLibDir) .. '/" /property:TargetName=' .. releaseLibBaseName .. ' /property:TargetExt=.' .. releaseLibExt
  local setCl = "SET CL="
  for i, defineOption in ipairs(buildDefines) do
  		setCl = setCl .. ' /D' .. defineOption
  end
  local rebuildDebugCmdline   = setCl .. ' && ' .. msbuildPath .. ' "' .. projectPath .. '" /p:Configuration=Debug   /property:OutDir="' .. path.getabsolute(debugLibDir)   .. '/"'
  local rebuildReleaseCmdline = setCl .. ' && ' .. msbuildPath .. ' "' .. projectPath .. '" /p:Configuration=Release /property:OutDir="' .. path.getabsolute(releaseLibDir) .. '/"'

  local rc
  print('Debug Command to Execute :' .. rebuildDebugCmdline )
  rc = os.execute(rebuildDebugCmdline)
  assert(rc == 0, 'Debug library build failed: ' .. libName)

  print('Release Command to Execute :' .. rebuildReleaseCmdline )
  rc = os.execute(rebuildReleaseCmdline)
  assert(rc == 0, 'Release library build failed: ' .. libName)
end

-- local function getXcodeProductName(libName)
--   local baseName, ext = splitExt(libName)
--   local prefix = baseName:sub(1, 3)
--   local suffix = baseName:sub(4)
--   assert((prefix .. suffix) == baseName, "That's just paranoia")
--   assert(prefix == 'lib', 'XCode library name should start with "lib" prefix: ' .. libName)
--   return suffix
-- end

local function rebuildLibXcode(libName, releaseLib, debugLib, projectPath, buildDefines)
  local projectDir, projectName = splitPathName(projectPath)
  local debugLibDir, debugLibName = splitPathName(debugLib)
  local releaseLibDir, releaseLibName = splitPathName(releaseLib)

  -- Overriding PRODUCT_NAME doesn't work for projects that has more than one target.
  -- In such case PRODUCT_NAME would be overriden for all targets, resulting in
  -- multiple libraries sharing the same name. It breaks compilation of course.
  --   Following projects known to suffer from that: glsocial

  --local rebuildDebugCmdline   = 'cd ' ..  projectDir .. ' && xcodebuild -project ' .. projectName .. ' -configuration Debug   DEPLOYMENT_LOCATION=NO CONFIGURATION_BUILD_DIR=' .. path.getabsolute(debugLibDir) .. ' PRODUCT_NAME=' .. getXcodeProductName(debugLibName)
  --local rebuildReleaseCmdline = 'cd ' ..  projectDir .. ' && xcodebuild -project ' .. projectName .. ' -configuration Release DEPLOYMENT_LOCATION=NO CONFIGURATION_BUILD_DIR=' .. path.getabsolute(releaseLibDir) .. ' PRODUCT_NAME=' .. getXcodeProductName(releaseLibName)
  local setCFlags = ''
  for i, defineOption in ipairs(buildDefines) do
		setCFlags = setCFlags .. ' -D' .. defineOption
  end
  local setCf = ' OTHER_CFLAGS="' .. setCFlags .. '"'
  local setCpp = ' OTHER_CPLUSPLUSFLAGS="' .. setCFlags .. '"'
  
  local rebuildDebugCmdline   = 'cd ' ..  projectDir .. ' && xcodebuild -project ' .. projectName .. ' -configuration Debug   DEPLOYMENT_LOCATION=NO CONFIGURATION_BUILD_DIR=' .. path.getabsolute(debugLibDir) .. setCf .. setCpp
  local rebuildReleaseCmdline = 'cd ' ..  projectDir .. ' && xcodebuild -project ' .. projectName .. ' -configuration Release DEPLOYMENT_LOCATION=NO CONFIGURATION_BUILD_DIR=' .. path.getabsolute(releaseLibDir) .. setCf .. setCpp
  
  local rc
  print('Debug Command to Execute :' .. rebuildDebugCmdline )
  rc = os.execute(rebuildDebugCmdline)
  assert(rc == 0, 'Debug library build failed: ' .. libName)
  print('Debug Command to Execute :' .. rebuildReleaseCmdline )
  rc = os.execute(rebuildReleaseCmdline)
  assert(rc == 0, 'Release library build failed: ' .. libName)
end

local function rebuildLibNdkBuild(libName, releaseLib, debugLib, projectPath, buildDefinesAndroid)
  local jniDir, androidMk = splitPathName(projectPath)
  assert(androidMk == 'Android.mk', 'Path to Android.mk expected, got: ' .. projectPath)

  local targetName = getUuid(projectPath)
  local applicationMkName = 'Application.premakeautobuild.mk'
  local applicationMk = jniDir .. '/' .. applicationMkName

  local configurations = {
    debug = debugLib,
    release = releaseLib,
  }

  for configName, libPath in pairs(configurations) do
    local libName = targetName
    if string.sub(targetName, 0, 3) ~= 'lib' then
      libName = 'lib' .. targetName
    end

    local expectedLocation = jniDir .. '/../obj/local/' .. defaultAbi .. '/' .. libName .. '.a'
    -- cleaning old build
    os.remove(expectedLocation)

    assert (not fileExists(applicationMk), 'Autogenerated Application.mk is already presented, if it is a remain of previous build, just remove it and retry: ' .. applicationMk)
    local amkFile = io.open(applicationMk, 'w')
    amkFile:write('APP_MODULES  := ' .. targetName .. '\n')
    amkFile:write('APP_OPTIM    := ' .. configName .. '\n')
    amkFile:write('APP_ABI      := ' .. defaultAbi .. '\n')
    amkFile:write('APP_STL      := gnustl_static\n')
    --amkFile:write('APP_PLATFORM := android-9\n')
    amkFile:close()
    
    local cmdline = 'ndk-build -C "' .. jniDir .. '/../" NDK_APPLICATION_MK="jni/' .. applicationMkName .. '"'
    print (cmdline)
    local rc = os.execute(cmdline)

    os.remove(applicationMk)

    assert (rc == 0, 'ndk-build call failed')
    assert (fileExists(expectedLocation), 'Unable to find generated library at path: ' .. expectedLocation)

    -- sorry, couldn't make fs.copy work, I'm kind of bad at lua
    copyFile(expectedLocation, libPath)
    assert (fileExists(libPath), 'Unable to copy generated library to: ' .. libPath)
  end
end

local function patchAllLibs(libsRootPath, platform)
	print('Patching all libraries for: ' .. platform .. libsRootPath)
    for name, _ in pairs(libsDb) do
		local libDef = libsDb[name]
		assert(libDef ~= nil, 'Unknown library name: ' .. name)
		local libPlatformInfo = libDef[platform]
		if libPlatformInfo == nil then
			print('skipping patching lib: ' .. name .. ' because it has no platform specified')
		else
			local retailLib, releaseLib, debugLib, projectPath, patchProject = unpackLibPlatformInfo(libsRootPath, libPlatformInfo)
			
			if projectPath == nil then
				print('skipping patching lib: ' .. name .. ' because it has no project specified')
			else
				if endsWith(projectPath, '.vcproj') then
					projectPath = upgradeProject(projectPath)
				end

				if(patchProject) then
					patchProjectInplace(libsRootPath, name, platform, projectPath)
				end
			end -- if projectPath == nil  else
		end -- if libPlatformInfo == nil  else
    end -- for
end -- function

local function rebuildLib(libsRootPath, platform, libsConfig, libName, buildDefinesMsvc, buildDefinesXCode, buildDefinesAndroid)
  if libName == 'all' then
    for name, _ in pairs(libsDb) do
      rebuildLib(libsRootPath, platform, libsConfig, name, buildDefinesMsvc, buildDefinesXCode)
    end
    return
  end

  if libName == 'linked' then
    printTable(libsConfig)
    for name, _ in pairs(libsConfig) do
      rebuildLib(libsRootPath, platform, libsConfig, name, buildDefinesMsvc, buildDefinesXCode)
    end
    return
  end

  local libDef = libsDb[libName]
  assert(libDef ~= nil, 'Unable to rebuild, unknown library name: ' .. libName)
  local libPlatformInfo = libDef[platform]

  if libPlatformInfo == nil then
    print('Unable to rebuild lib: ' .. libName .. ' because it is not presented on platform: ' .. platform)
    return 
  end

  local retailLib, releaseLib, debugLib, projectPath, patchProject = unpackLibPlatformInfo(libsRootPath, libPlatformInfo)

  if projectPath == nil then
    print('Unable to rebuild lib: ' .. libName .. ' because it has no project specified, skipping')
    return 
  end

  if endsWith(projectPath, '.vcproj') then
    projectPath = upgradeProject(projectPath)
  end


  assert(retailLib == nil, 'Retail libs not supported for external projects')
  if fileExists(debugLib)   then os.remove(debugLib)   end
  if fileExists(releaseLib) then os.remove(releaseLib) end

  print('Building static libraries for: ' .. libName)
  if platform == 'win' then
    rebuildLibMsvc(libName, releaseLib, debugLib, projectPath, buildDefinesMsvc)
  elseif platform == 'ios' then
    rebuildLibXcode(libName, releaseLib, debugLib, projectPath, buildDefinesXCode)
  elseif platform == 'andr' then
    rebuildLibNdkBuild(libName, releaseLib, debugLib, projectPath, buildDefinesAndroid)
  else
    assert(false, 'Unsupported platform: ' .. platform)
  end

  assert (fileExists(debugLib),   libNotFoundExplanation .. debugLib)
  assert (fileExists(releaseLib), libNotFoundExplanation .. releaseLib)
end

function setResourcesDir(path)
  local items = {}

  -- adding both files and directories
  local f = io.popen("ls -1 " .. path)
  for line in f:lines() do
    if line.sub(1, 1) ~= '.' then
      table.insert(items, path .. '/' .. line)
    end
  end

  -- premake call
  files(items)
  resources(items)
end

function addFrameworksToCurrentSolutionConfiguration(strongFrameworks, weakFrameworks)
  local linkOptionsString = ''
  for i, name in ipairs(strongFrameworks) do
    linkOptionsString = linkOptionsString .. " -framework " .. name 
  end
  for i, name in ipairs(weakFrameworks) do
    linkOptionsString = linkOptionsString .. " -weak_framework " .. name 
  end

  -- premake call
  linkoptions {linkOptionsString}
end

return {
  checkLibs = checkLibs,
  addProjectsToCurrentSolution = addProjectsToCurrentSolution,
  addLibsToCurrentProject = addLibsToCurrentProject,
  rebuildLib = rebuildLib,
  getPossibleRebuildTargets = getPossibleRebuildTargets,
  setResourcesDir = setResourcesDir,
  addFrameworksToCurrentSolutionConfiguration = addFrameworksToCurrentSolutionConfiguration,
  patchAllLibs = patchAllLibs,
}

