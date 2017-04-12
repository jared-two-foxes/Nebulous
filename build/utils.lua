
local utils = {}

local function endsWith(s, suffix)
  return #s >= #suffix and s:find(suffix, #s - #suffix + 1, true)
end

local function splitPathName(path)
  local nameStart, _, name = path:find('([^/\\]+)[/\\]?$')
  local dir = path:sub(0, nameStart - 2)
  return dir, name
end                                  

local function splitExt(path)
  local extStart, _, ext = path:find('[^/\\.][.]([^/\\.]*)$')
  if extStart ~= nil then
    local name = path:sub(0, extStart)
    return name, ext
  else
    return path, nil
  end
end

local function unpackLibPlatformInfo( libsRootPath, libPlatformInfo )
  local projectPath, includePath, libPath, libNameRelease, libNameDebug = unpack( libPlatformInfo )

  if projectPath ~= nil and not path.isabsolute( projectPath ) then 
    projectPath = libsRootPath .. '/' .. projectPath 
  end
  
  if includePath ~= nil then
    -- could be a list of objects, split on ';', and recombine
    parts = string.explode( includePath, ';' ) 
    includePath = ""
    for _,v in pairs(parts) do
      if not path.isabsolute( v ) then 
        v = libsRootPath .. '/' .. v
      end
      includePath = includePath .. v .. ';'
    end
  end
  
  if libPath ~= nil and not path.isabsolute( libPath ) then 
    libPath = libsRootPath .. '/' .. libPath 
  end

  return projectPath, includePath, libPath, libNameRelease, libNameDebug
end

function utils.addLibrariesToCurrentProject( libraries, libsRootPath, platform )
  for libName, libDef in pairs( libraries ) do
    local projectPath, includePath, libPath, libNameRelease, libNameDebug = unpackLibPlatformInfo( libsRootPath, libDef )

    if projectPath ~= nil then
      local fileDir, fileName     = splitPathName( projectPath )
      local fileBaseName, fileExt = splitExt( fileName )
      links { fileBaseName } 
    else
      if includePath ~= nil then includedirs { includePath } end
      if libPath ~= nil then     libdirs { libPath } end
      if libNameRelease ~= nil then     
        filter 'configurations:release'
          links { libNameRelease } 
        filter {}
      end
      if libNameDebug ~= nil then
        filter 'configurations:debug'
          links { libNameDebug } 
        filter {}
      end
    end
  end
end


return utils