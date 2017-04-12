-- Format: { projectPath, includePath, libPath, libname, patch } 

return {
  opengl = {  
    nil, 
    'mali/include', 
    'mali',
    {"libEGL.lib","libGLESv2.lib","libMaliEmulator.lib"}
    {"libEGL.lib","libGLESv2.lib","libMaliEmulator.lib"}
  },
}