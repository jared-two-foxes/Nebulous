-- Format: { projectPath, includePath, libPath, libname, patch } 

return {
  nebulae_common = {       
    win = { 'D:/Develop/nebulous/Nebulae/trunk/Project/Common/Common.vcxproj',   'D:/Develop/nebulous/Nebulae/trunk/',                      'D:/Develop/nebulous/Nebulae/trunk/lib',                       'common.lib'        },
    ios = { nil,                                       'Nebulae',                            nil,                                 nil                 }, 
  },
  nebulae_audio = {       
    win = { 'D:/Develop/nebulous/Nebulae/trunk/Project/Audio/Audio.vcxproj',     'D:/Develop/nebulous/Nebulae/trunk/',                      'D:/Develop/nebulous/Nebulae/trunk/lib',                       'audio.lib'        },
    ios = { nil,                                       'Nebulae',                            nil,                                 nil                 }, 
  },  
  nebulae_alpha = {       
    win = { 'D:/Develop/nebulous/Nebulae/trunk/Project/Alpha/Alpha.vcxproj',     'D:/Develop/nebulous/Nebulae/trunk/',                      'D:/Develop/nebulous/Nebulae/trunk/lib',                       'alpha.lib'        },
    ios = { nil,                                       'Nebulae',                            nil,                                 nil                 }, 
  },  
  nebulae_beta = {       
    win = { 'D:/Develop/nebulous/Nebulae/trunk/Project/Beta/Beta.vcxproj',       'D:/Develop/nebulous/Nebulae/trunk/',                      'D:/Develop/nebulous/Nebulae/trunk/lib',                       'beta.lib'        },
    ios = { nil,                                       'Nebulae',                            nil,                                 nil                 }, 
  },

  boost = {       
    win = { nil,                                         'D:/Develop/nebulous/Dependencies/boost',                         'D:/Develop/nebulous/Dependencies/boost/stage/lib',                  nil                 },
    ios = { nil,                                         'D:/Develop/nebulous/Dependencies/boost_1_55_0/src/boost_1_55_0', nil,                                nil                 }, 
  },
  jsoncpp = {                  
    win = { 'D:/Develop/nebulous/Dependencies/jsoncpp/jsoncpp.vcxproj',                   'D:/Develop/nebulous/Dependencies/jsoncpp/include',            'D:/Develop/nebulous/Dependencies/jsoncpp/lib',                      'jsoncpp.lib'       },
    ios = { 'jsoncpp/jsoncpp.xcodeproj',                 'jsoncpp/include',            'jsoncpp/lib',                      'libjsoncpp.a'      },
  },           
  pugixml = {                  
    win = { 'D:/Develop/nebulous/Dependencies/pugixml/pugixml.vcxproj',                   'D:/Develop/nebulous/Dependencies/pugixml/src',                'D:/Develop/nebulous/Dependencies/pugixml/lib',                      'D:/Develop/nebulous/Dependencies/pugixml.lib'       },
    ios = { 'pugixml/pugixml.xcodeproj',                 'pugixml/src',                'pugixml/lib',                      'libpugixml.a'      },
  },
  libvorbis = {          
    win = { 'D:/Develop/nebulous/Dependencies/libvorbis/libvorbis.vcxproj',               'D:/Develop/nebulous/Dependencies/libvorbis/include',          'D:/Develop/nebulous/Dependencies/libvorbis/lib',                    'vorbis.lib'        },
    ios = { 'libvorbis/libvorbis.xcodeproj',             'libvorbis/include',          'libvorbis/lib',                    'libvorbis.a'       },
  },
  libogg = {          
    win = { 'D:/Develop/nebulous/Dependencies/libogg/libogg.vcxproj',                     'D:/Develop/nebulous/Dependencies/libogg/include',             'D:/Develop/nebulous/Dependencies/libogg/lib',                       'ogg.lib'           },
    ios = { 'libogg/libogg.xcodeproj',                   'libogg/include',             'libogg/lib',                       'libogg.a'          },
  },
  openal = {          
    win = { nil,                                         'D:/Develop/nebulous/Dependencies/openal-soft-1.15.1/include', 'D:/Develop/nebulous/Dependencies/openal-soft-1.15.1/build/Debug',   'OpenAL32.lib'      },
    ios = { nil,                                         nil,                          nil,                                nil                 },
  },
  zlib = {
    win = { 'D:/Develop/nebulous/Dependencies/zlib/zlib.vcxproj',                          'D:/Develop/nebulous/Dependencies/zlib',                      'D:/Develop/nebulous/Dependencies/zlib/lib',                         'zlib.lib'          },
    ios = { 'zlib/zlib.xcodeproj',                        'zlib',                      'zlib/lib',                         'zlib.a'            },
  },
  minizip = {
    win = { 'D:/Develop/nebulous/Dependencies/zlib/minizip.vcxproj',                       'D:/Develop/nebulous/Dependencies/zlib/contrib',              'D:/Develop/nebulous/Dependencies/zlib/lib',                         'minizip.lib'       },
    ios = { 'zlib/minizip.xcodeproj',                     'zlib/contrib',              'zlib/lib',                         'minizip.a'         },
  },
  freeimage = {
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/freeimagelib.vcxproj',            'D:/Develop/nebulous/Dependencies/FreeImage/Source',           'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'freeimage.lib'     },
    ios = { 'FreeImage/freeimagelib.xcodeproj',          'FreeImage/Source',           'FreeImage/lib',                    'libfreeimage.a'    },
  },
  openexr = {
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/openexr.vcxproj',                  nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'openexr.lib'       },
    ios = { 'FreeImage/openexr.xcodeproj',                nil,                         'FreeImage/lib',                    'libopenexr.a'      },
  },
  libtiff4 = {
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/libtiff4.vcxproj',                 nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'tiff4.lib'         },
    ios = { 'FreeImage/libtiff4.xcodeproj',               nil,                         'FreeImage/lib',                    'libtiff4.a'        },
  },
  libpng = {
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/libpng.vcxproj',                   nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'png.lib'           },
    ios = { 'FreeImage/libpng.xcodeproj',                 nil,                         'FreeImage/lib',                    'libpng.a'          },
  },   
  libopenjpeg = {
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/libopenjpeg.vcxproj',              nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'openjpeg.lib'      },
    ios = { 'FreeImage/libopenjpeg.xcodeproj',            nil,                         'FreeImage/lib',                    'libopenjpeg.a'     },
  },
  libmng = {
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/libmng.vcxproj',                   nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'mng.lib'           },
    ios = { 'FreeImage/libmng.xcodeproj',                 nil,                         'FreeImage/lib',                    'libmng.a'          },
  },   
  libjpeg = {   
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/libjpeg.vcxproj',                  nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'jpeg.lib'          },
    ios = { 'FreeImage/libjpeg.xcodeproj',                nil,                         'FreeImage/lib',                    'libjpeg.a'         },
  },   
  libraw = {   
    win = { 'D:/Develop/nebulous/Dependencies/FreeImage/libraw.vcxproj',                   nil,                         'D:/Develop/nebulous/Dependencies/FreeImage/lib',                    'raw.lib'           },
    ios = { 'FreeImage/libraw.xcodeproj',                 nil,                         'FreeImage/lib',                    'libraw.a'          },
  },   
  freetype = {   
    win = { 'D:/Develop/nebulous/Dependencies/freetype/freetype.vcxproj',                  'D:/Develop/nebulous/Dependencies/freetype/include',          'D:/Develop/nebulous/Dependencies/freetype/lib',                     'freetype.lib'      },
    ios = { 'freetype/freetype.xcodeproj',                'freetype/include',          'freetype/lib',                     'libfreetype.a'     },
  },
  utf8 = {   
    win = { nil,                                          'utf8',                      nil,                                nil                 },
    ios = { nil,                                          'utf8',                      nil,                                nil                 },
  },
  lua = {   
    win = { 'D:/Develop/nebulous/Dependencies/lua/lualib51.vcxproj',                       'D:/Develop/nebulous/Dependencies/lua/src',                   'D:/Develop/nebulous/Dependencies/lua/lib',                          'lualib51.lib'      },
    ios = { 'lua/lualib51.xcodeproj',                     'lua/src',                   'lua/lib',                          'lualib51.a'        },
  },

  -- gmock = {                  
  --   win = { 'gmock/msvc/2010/gmock.vcxproj',             'gmock/include',              'gmock/lib',                        'gmock.lib'         },
  --   ios = { 'gmock/gmock.xcodeproj',                     'gmock/include',              'gmock/lib',                        'libgtest.a'        },
  -- },
  -- gtest = {                  
  --   win = { 'gtest/gtest.vcxproj',                       'gtest/include',              'gtest/lib',                        'gtest.lib'         },
  --   ios = { 'gtest/gtest.xcodeproj',                     'gtest/include',              'gtest/lib',                        'libgtest.a'        },
  -- },           
}
