#ifndef NEBULAE_ALPHA_GL_CONFIG_H__
#define NEBULAE_ALPHA_GL_CONFIG_H__

#include <Nebulae/Common/Common.h>

// include OpenGL headers
#if NE_PLATFORM == NE_PLATFORM_APPLE_IOS
  #include <OpenGLES/ES2/gl.h>
  //#include <OpenGLES/ES2/glext.h>
#elif NE_PLATFORM == NE_PLATFORM_WIN32
  #include <gl/gl.h>
#endif

#include <glext.h>

#define GLCHECK \

bool CheckForGLError();

#endif // NEBULAE_ALPHA_CONFIG_H__