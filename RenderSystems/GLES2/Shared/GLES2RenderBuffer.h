#ifndef NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2RENDERBUFFER_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2RENDERBUFFER_H_

#include <GLES2/config.h>

namespace Nebulae
{

class GLES2RenderBuffer
{
public:
  GLuint m_handle;

public:
  GLES2RenderBuffer();
  ~GLES2RenderBuffer();

  GLuint GetHandle() const;

  void Bind();
  bool Load();
  bool Unload();
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2RENDERBUFFER_H_