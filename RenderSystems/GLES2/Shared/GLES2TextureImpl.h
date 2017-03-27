#ifndef NEBULAE_GLES2_TEXTUREIMPL_H__
#define NEBULAE_GLES2_TEXTUREIMPL_H__

#include <GLES2/config.h>

#include <Nebulae/Alpha/Texture/TextureImpl.h>

namespace Nebulae
{

/** 
 *	GLES2TextureImpl.
 */
class GLES2TextureImpl : public TextureImpl
{
protected:
  GLuint m_iHandle;

  public:
    GLES2TextureImpl( const std::string& strFileName );
    virtual ~GLES2TextureImpl();

    //functions
    virtual bool Load( const Image& image ) override;
    virtual bool LoadFromMemory( unsigned char* szBuffer, uint8 srcDataType, uint8 channels, int iWidth, int iHeight ) override;

    //getters
    GLuint GetHandle() const { return m_iHandle; }

}; //GLES2TextureImpl

} //Nebulae

#endif // NEBULAE_GLES2_TEXTUREIMPL_H__