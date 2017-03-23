#ifndef NE_GLES2_CONFIG_H__
#define NE_GLES2_CONFIG_H__

#include <Nebulae/Common/Common.h>

#if NE_PLATFORM == NE_PLATFORM_APPLE_IOS
  #import <OpenGLES/ES2/gl.h>
  #import <OpenGLES/ES2/glext.h>
#elif NE_PLATFORM == NE_PLATFORM_WIN32
  #include <EGL/egl.h>
  #include <GLES2/gl2.h>
#endif

bool CheckForGLESError(); 

#endif // NE_GLES2_CONFIG_H__
