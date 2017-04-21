-- Format: { rootPath, projectPath, includePath, libPath, 'buildSystem', libname (release), libname (debug)? } 

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
    'jsoncpp/built/include',           
    'jsoncpp/built/lib',              
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
    'zlib/built/include',                      
    'zlib/built/lib',                 
    'cmake',
    'zlib.lib',
    'zlibd.lib',
  },

  libpng = {
    'libpng',  
    nil, 
    'libpng/built/include',
    'libpng/built/lib',
    'cmake',
    'libpng16.lib',
    'libpng16d.lib',
  },

  freetype = {   
    'freetype2',  
    nil,                                                               
    'freetype2/built/include/freetype2',          
    'freetype2/built/lib',             
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
    'luaDist/built/include',                   
    'luaDist/built/lib',     
    'cmake',             
    'lua.lib',
    'luad.lib',
  },

  libogg = {  
    'libogg',   
    nil,                                                               
    'libogg/built/include',                   
    'libogg/built/lib',     
    'cmake',             
    'ogg.lib',
    'oggd.lib',
  },

  libvorbis = {  
    'libvorbis',   
    nil,                                                               
    'libvorbis/built/include',                   
    'libvorbis/built/lib',     
    'cmake',             
    'libvorbis.lib',
    'libvorbisd.lib',
  },

  openal = {  
    'openal-soft',   
    nil,                                                               
    'openal-soft/built/include',                   
    'openal-soft/built/lib',     
    'cmake',             
    'OpenAL32.lib',
    'OpenAL32d.lib',
  },
}