#ifndef __NEBULAE_ALPHA_GLES2_RENDERBUFFERIMPL_H__
#define __NEBULAE_ALPHA_GLES2_RENDERBUFFERIMPL_H__

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

}

#endif // __NEBULAE_ALPHA_GLES2_RENDERBUFFERIMPL_H__