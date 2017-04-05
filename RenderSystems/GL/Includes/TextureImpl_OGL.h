#ifndef __TEXTUREIMPL_OGL_H__
#define __TEXTUREIMPL_OGL_H__

#include <Config.h>
#include <Nebulae/Alpha/Texture/TextureImpl.h>

namespace Nebulae
{

/** TextureImpl_OGL.
	*/
class TextureImpl_OGL : public TextureImpl
{
protected:
	GLuint m_iHandle;

  public:
    TextureImpl_OGL( const std::string& strFileName );
    virtual ~TextureImpl_OGL();

    //functions
    virtual bool Load( const Image& image ) override;
    virtual bool LoadFromMemory( unsigned char* szBuffer, uint8 srcDataType, uint8 channels, int iWidth, int iHeight ) override;

    //getters
    GLuint GetHandle() const { return m_iHandle; }

};

} 

#endif // __TEXTUREIMPL_OGL_H__