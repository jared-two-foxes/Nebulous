#ifndef NEBULAE_BETA_FONT_FONTMANAGER_H_
#define NEBULAE_BETA_FONT_FONTMANAGER_H_

// #include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Font/Font.h>

namespace Nebulae
{

// Forward decleration
class RenderSystem;

/** FontManager
 */
class FontManager
{
private:
  std::vector<Font*> m_Resources;

public:
  FontManager();
  virtual ~FontManager();

  Font* Create( const std::string& name, int iFontSize = 16 );

  Font* GetByName( const std::string& name ) const;

private:
  virtual void addImpl( Font* res );

}; // FontManager

} // namespace Nebulae

#endif // NEBULAE_BETA_FONT_FONTMANAGER_H_