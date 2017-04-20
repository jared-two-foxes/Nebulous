-- Format: { rootPath, projectPath, includePath, libPath, libname (release), libname (debug)? } 

return {
  boost = {   
    'boost',    
    nil,                                                                 
    'boost/built/include/boost-1_64',                     
    'boost/built/lib',                  
    'boost.build',
    nil,
    nil,
  },

  jsoncpp = {  
    'jsoncpp',
    nil,                                                               
    'jsoncpp/build/include',           
    'jsoncpp/build/lib',              
    'cmake',
    'jsoncpp.lib',
    'jsoncppd.lib',
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
    'zlib/build/include',                      
    'zlib/build/lib',                 
    'cmake',
    'zlib.lib',
    'zlibd.lib',
  },

  libpng = {
    'libpng',  
    nil, 
    'libpng/build/include',
    'libpng/build/lib',
    'cmake',
    'libpng16.lib',
    'libpng16d.lib',
  },

  freetype = {   
    'freetype2',  
    nil,                                                               
    'freetype2/build/include/freetype2',          
    'freetype2/build/lib',             
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
    'luaDist/build/include',                   
    'luaDist/build/lib',     
    'cmake',             
    'lua.lib',
    'luad.lib',
  },

}