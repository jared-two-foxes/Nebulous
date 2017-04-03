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
    'jsoncpp/lib/release',              
    'jsoncpp.lib'
  },

  libvorbis = {
    nil,                                                               
    'vorbis/include',         
    'vorbis/lib/release',            
    'vorbis.lib'
  },
  libogg = {
    nil,                                   
    'ogg/include',           
    'ogg/lib/release',               
    'ogg.lib'
  },
  -- openal = {
  --   nil,                                                               
  --   'openal-soft/include',       
  --   'openal-soft/libs/Win64',           
  --   'OpenAL32.lib'
  -- },

  zlib = {
    nil,                                                               
    'zlib',                      
    'zlib/lib/release',                 
    'zlib.lib'
  },
  minizip = {
    nil,                                                               
    'zlib/contrib',              
    'zlib/lib/release',                 
    'minizip.lib'
  },

  gli = {   
    nil,                                                               
    'gli/include',          
    nil,             
    nil
  },

  freetype = {   
    nil,                                                               
    'freetype2/include',          
    'freetype2/lib/release',             
    'freetype2411mt.lib'
  },

  utf8 = {   
    nil,                                                               
    'utf8/source',                      
    nil,                                
    nil 
  },

  lua = {   
    nil,                                                               
    'lua/src',                   
    'lua/lib/release',                  
    'lualib51.lib'
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
  --   'gtest/lib/debug',                        
  --   'gtest.lib'
  -- },  

}