-- Format: { projectPath, includePath, libPath, libname } 

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

  -- CMake installed!
  zlib = {
    nil,                                                               
    'C:/Program Files/zlib/include',                      
    'C:/Program Files/zlib/lib',                 
    'zlibstatic.lib'
  },

  libpng = {
    nil, 
    'C:/Program Files/libpng/include',
    'C:/Program Files/libpng/lib',
    'libpng16.lib'
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