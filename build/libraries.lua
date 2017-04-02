-- Format: { projectPath, includePath, libPath, libname, patch } 

return {
  boost = {       
    nil,                                                                 'boost',                     'boost/stage/lib',                  nil 
  },
  jsoncpp = {                                                                                 
    nil,                                                                 'jsoncpp/include',           'jsoncpp/lib/release',              'jsoncpp.lib'
  },
  libvorbis = {                                                                         
    nil,                                                                 'libvorbis/include',         'libvorbis/lib/release',            'vorbis.lib'
  },
  libogg = {                                                                         
    nil,                                                                 'libogg/include',            'libogg/lib/release',               'ogg.lib'
  },
  openal = {                                                                         
    nil,                                                                 'openal-soft/include',       'openal-soft/libs/Win64',           'OpenAL32.lib'
  },
  zlib = {
    nil,                                                                 'zlib',                      'zlib/lib/release',                 'zlib.lib'
  },
  minizip = {
    nil,                                                                 'zlib/contrib',              'zlib/lib/release',                 'minizip.lib'
  },
  freeimage = {
    nil,                                                                 'FreeImage/Source',          'FreeImage/lib/release',            'freeimage.lib'
  },
  openexr = {
    nil,                                                                 nil,                         'FreeImage/lib/release',            'openexr.lib'
  },
  libtiff4 = {
    nil,                                                                 nil,                         'FreeImage/lib/release',            'libtiff.lib'
  },
  libpng = {
    nil,                                                                 nil,                         'FreeImage/lib/release',            'libpng.lib'
  },   
  libopenjpeg = {
    nil,                                                                 nil,                         'FreeImage/lib/release',            'libopenjpeg.lib'
  },
  libmng = {
    nil,                                                                 nil,                         'FreeImage/lib/release',            'libmng.lib'
  },   
  libjpeg = {   
    nil,                                                                 nil,                         'FreeImage/lib/release',            'libjpeg.lib'
  },   
  libraw = {   
    nil,                                                                 nil,                         'FreeImage/lib/release',            'libraw.lib'
  },   
  freetype = {   
    nil,                                                                 'freetype/include',          'freetype/lib/release',             'freetype2411mt.lib'
  },
  utf8 = {   
    nil,                                                                 'utf8',                      nil,                                nil 
  },
  lua = {   
    nil,                                                                 'lua/src',                   'lua/lib/release',                  'lualib51.lib'
  },

  gmock = {                  
    nil,                                                                'gmock/include',              'gmock/lib',                        'gmock.lib'
  },
  gtest = {                  
    nil,                                                                'gtest/include',              'gtest/lib/debug',                        'gtest.lib'
  },  

}