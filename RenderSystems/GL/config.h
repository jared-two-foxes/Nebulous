#ifndef NEBULAE_RENDERSYSTEMS_GL_CONFIG_H_
#define NEBULAE_RENDERSYSTEMS_GL_CONFIG_H_

#include <Nebulae/Common/Common.h>

// include OpenGL headers
#if NE_PLATFORM == NE_PLATFORM_APPLE_IOS
#include <OpenGLES/ES2/gl.h>
// #include <OpenGLES/ES2/glext.h>
#elif NE_PLATFORM == NE_PLATFORM_WIN32
#include <gl/gl.h>
#endif

#include <glext.h>

#define GLCHECK

bool CheckForGLError();

#endif // NEBULAE_RENDERSYSTEMS_GL_CONFIG_H_