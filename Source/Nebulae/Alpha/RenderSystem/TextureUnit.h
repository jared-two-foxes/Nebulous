#ifndef NEBULAE_ALPHA_RENDERSYSTEM_TEXTUREUNIT_H_
#define NEBULAE_ALPHA_RENDERSYSTEM_TEXTUREUNIT_H_

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{

// Forward Decleration
class RenderSystem;
class Texture;

class TextureUnit
{
private:
  RenderSystem* m_renderer{ nullptr };
  std::string m_name;
  Texture* m_texture{ nullptr };

public:
  TextureUnit( RenderSystem* renderer );

  std::string GetName() const;
  Texture* GetTexture() const;

  bool Load();
  void SetName( const std::string& name );
  void SetTexture( Texture* texture );

}; // TextureUnit

} // namespace Nebulae

#endif // NEBULAE_ALPHA_RENDERSYSTEM_TEXTUREUNIT_H_