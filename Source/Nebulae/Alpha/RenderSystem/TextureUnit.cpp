
#include "TextureUnit.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Texture/Texture.h>

using namespace Nebulae;


TextureUnit::TextureUnit( RenderSystem* renderer ) {}


bool TextureUnit::Load()
{
  bool ret = true;
  if ( !m_texture->IsLoaded() )
  {
    // ret = m_texture->Load();
  }
  return ret;
}


void TextureUnit::SetName( const std::string& name ) { m_name = name; }


void TextureUnit::SetTexture( Texture* texture ) { m_texture = texture; }


Texture* TextureUnit::GetTexture() const { return m_texture; }
