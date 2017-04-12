-- Format: { projectPath, includePath, libPath, libname (release), libname (debug)? } 

return {

libvorbis = {
  nil,                                                               
  'libvorbis/include',         
  'libvorbis/win32/VS2010/x64/$(Configuration)',            
  'libvorbis_static.lib;libvorbisfile_static.lib'
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

}