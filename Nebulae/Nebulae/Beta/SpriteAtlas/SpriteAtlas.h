#ifndef __SPRITEATLAS_H__
#define __SPRITEATLAS_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

// Forward Decelerations.
class RenderSystem;
class SubTexture;
class Texture;

/** SpriteAtlasModule.
  */
struct SpriteAtlasModule
{
	std::size_t  m_identifier;
  std::string  m_name;
  SubTexture*  m_subTexture;
}; 

/** SpriteAtlas.
  */
class SpriteAtlas
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

private:
  std::string                      m_name;
  RenderSystemPtr                  m_renderDevice;
  std::vector<SpriteAtlasModule* > m_modules;

  public:
    SpriteAtlas( const std::string& name, RenderSystemPtr renderDevice );
    virtual ~SpriteAtlas();

    virtual bool Load( File* is );

    const std::string& GetName() const;
		std::size_t        GetModuleCount() const;
    SpriteAtlasModule* GetModule( int idx ) const;
    SubTexture*        FindModuleSubTexture( const std::string& strModuleName ) const;

  private:
    void ProcessFrame( Json::Value& frame_data, Texture* texture );

}; //SpriteAtlas

} // Nebulae

#endif // __SPRITEATLAS_H__