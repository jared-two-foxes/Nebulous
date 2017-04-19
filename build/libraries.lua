-- Format: { rootPath, projectPath, includePath, libPath, libname (release), libname (debug)? } 

return {
  boost = {   
    'boost',    
    nil,                                                                 
    'boost/include/boost-1_64',                     
    'boost/stage/lib',                  
    'boost.build',
    nil,
    nil,
  },

  jsoncpp = {  
    'jsoncpp',
    nil,                                                               
    'jsoncpp/include',           
    'jsoncpp/build/src/lib_json/$(Configuration)',              
    'cmake',
    'jsoncpp.lib',
    'jsoncpp.lib',
  },

  brofiler = {
    'Brofiler-1.1.1',       
    nil,                                                                 
    'Brofiler-1.1.1',           
    'Brofiler-1.1.1',              
    nil,
    'ProfilerCore64.lib',
    'ProfilerCore64.lib',
  },

  zlib = {
    'zlib',  
    nil,                                                               
    'zlib',                      
    'zlib/build/$(Configuration)',                 
    'cmake',
    'zlib.lib',
    'zlibd.lib',
  },

  libpng = {
    'libpng',  
    nil, 
    'libpng',
    'libpng/build/$(Configuration)',
    'cmake',
    'libpng16.lib',
    'libpng16d.lib',
  },

  freetype = {   
    'freetype2',  
    nil,                                                               
    'freetype2/include',          
    'freetype2/build/$(Configuration)',             
    'cmake',
    'freetype.lib',
    'freetyped.lib',
  },

  utf8 = {   
    'utf8',  
    nil,                                                               
    'utf8/source',                      
    nil,                                
    nil,
    nil,
    nil 
  },

  lua = {  
    'luaDist',   
    nil,                                                               
    'luaDist/src',                   
    'luaDist/build/$(Configuration)',     
    'cmake',             
    'lua.lib',
    'lua.lib',
  },

}