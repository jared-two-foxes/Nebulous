-- Format: { projectPath, includePath, libPath, libname, patch } 

return {
  boost = {       
    nil,                                                                 
    'C:/boost',                     
    'C:/boost/lib',                  
    nil 
  },
  jsoncpp = {                                                                                 
    nil,                                                               
    'jsoncpp/include',           
    'jsoncpp/makefiles/msvc2010/x64/$(Configuration)',              
    'lib_json.lib'
  },
  brofiler = {       
    nil,                                                                 
    'Brofiler-1.1.1',           
    'Brofiler-1.1.1',              
    'ProfilerCore64.lib'
  },

  libvorbis = {
    nil,                                                               
    'libvorbis/include',         
    'libvorbis/win32/VS2010/x64/$(Configuration)',            
    'libvorbis_static.lib'
  },
  libogg = {
    nil,                                   
    'libogg/include',           
    'libogg/win32/VS2015/x64/$(Configuration)',               
    'libvorbis_static.lib'
  },

  -- CMake installed!
  openal = { 
    nil,                                                               
    'C:/Program Files/OpenAL/include',       
    'C:/Program Files/OpenAL/lib',           
    'OpenAL32.lib'
  },

  -- CMake installed!
  zlib = {
    nil,                                                               
    'C:/Program Files/zlib/include',                      
    'C:/Program Files/zlib/lib',                 
    'zlibstatic.lib'
  },

  -- minizip = {
  --   nil,                                                               
  --   'zlib/contrib',              
  --   'zlib/build/$(Configuration)',                 
  --   'minizip.lib'
  -- },

  gli = {   
    nil,                                                               
    'gli/include',          
    nil,             
    nil
  },

  freetype = {   
    nil,                                                               
    'freetype2/include',          
    'freetype2/objs/vc2010/x64',             
    'freetype271MT.lib'
  },

  utf8 = {   
    nil,                                                               
    'utf8/source',                      
    nil,                                
    nil 
  },

  -- CMake installed!
  lua = {   
    nil,                                                               
    'C:/Program Files/lua/include',                   
    'C:/Program Files/lua/lib',                  
    'lua.lib'
  },

  -- lua = {   
  --   nil,                                                               
  --   'luaDist/etc;luaDist/src',                   
  --   'luaDist/build/$(Configuration)',
  --   'liblua_static.lib'
  -- },

  -- gmock = {                  
  --   nil,                                                               
  --   'mock/include',              
  --   'gmock/lib',                        
  --   'gmock.lib'
  -- },
  -- gtest = {                  
  --   nil,                                                               
  --   'gtest/include',             
  --   'gtest/lib/$(Configuration)',                        
  --   'gtest.lib'
  -- },  

}