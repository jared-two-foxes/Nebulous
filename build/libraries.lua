-- Format: { projectPath, includePath, libPath, libname (release), libname (debug)? } 

return {
  boost = {       
    nil,                                                                 
    'C:/boost',                     
    'C:/boost/lib',                  
    nil,
    nil
  },

  jsoncpp = {                                                                                 
    nil,                                                               
    'jsoncpp/include',           
    'jsoncpp/makefiles/msvc2010/x64/$(Configuration)',              
    'lib_json.lib',
    'lib_json.lib',
  },

  brofiler = {       
    nil,                                                                 
    'Brofiler-1.1.1',           
    'Brofiler-1.1.1',              
    'ProfilerCore64.lib',
    'ProfilerCore64.lib',
  },

  -- CMake installed!
  zlib = {
    nil,                                                               
    'C:/Program Files/zlib/include',                      
    'C:/Program Files/zlib/lib',                 
    'zlibstatic.lib',
    'zlibstaticd.lib',
  },

  libpng = {
    nil, 
    'C:/Program Files/libpng/include',
    'C:/Program Files/libpng/lib',
    'libpng16.lib',
    'libpng16d.lib',
  },

  freetype = {   
    nil,                                                               
    'freetype2/include',          
    'freetype2/objs/vc2010/x64',             
    'freetype271MT.lib',
    'freetype271MTd.lib',
  },

  utf8 = {   
    nil,                                                               
    'utf8/source',                      
    nil,                                
    nil,
    nil 
  },

  -- CMake installed!
  lua = {   
    nil,                                                               
    'C:/Program Files/lua/include',                   
    'C:/Program Files/lua/lib',                  
    'lua.lib',
    'lua.lib',
  },


  -- gmock = {                  
  --   nil,                                                               
  --   'mock/include',              
  --   'gmock/lib',                        
  --   'gmock.lib',
  --   nil
  -- },

  -- gtest = {                  
  --   nil,                                                               
  --   'gtest/include',             
  --   'gtest/lib/$(Configuration)',                        
  --   'gtest.lib',
  --   nil
  -- },  

}