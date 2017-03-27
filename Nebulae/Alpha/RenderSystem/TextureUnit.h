#ifndef NEBULAE_ALPHA_TEXTUREUNIT_H__
#define NEBULAE_ALPHA_TEXTUREUNIT_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{

//Forward Decleration
class RenderSystem;
class Texture;

class TextureUnit
{
private:
  RenderSystem* m_renderer;
  std::string   m_name;
  Texture*      m_texture;

  public:
    TextureUnit( RenderSystem* renderer );

    std::string GetName() const;
    Texture*    GetTexture() const;

    bool        Load();
    void        SetName( const std::string& name );
    void        SetTexture( Texture* texture );

}; //TextureUnit

} //Nebulae

#endif // NEBULAE_ALPHA_TEXTUREUNIT_H__