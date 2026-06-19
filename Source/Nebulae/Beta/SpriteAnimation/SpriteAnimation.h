#ifndef NEBULAE_BETA_SPRITEANIMATION_SPRITEANIMATION_H_
#define NEBULAE_BETA_SPRITEANIMATION_SPRITEANIMATION_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
// Forward Decleration.
class SpriteAtlas;
class SpriteAtlasManager;

/// SpriteAnimationFrame.
struct SpriteAnimationFrame
{
  int iId;
  SpriteAtlas* pSpriteAtlas;
  std::string strFrameName;
};


class SpriteAnimation
/**
 *  A resource type that defines a series of SpriteAltas frames that are to be rendered
 *  in sequence to simulate 'Animation'.
 */
{
public:
  typedef std::shared_ptr<SpriteAtlasManager> AtlasManagerPtr;

private:
  std::string m_name;
  AtlasManagerPtr m_atlasManager;              ///< The sprite atlas manager.
  std::vector<SpriteAnimationFrame*> m_Frames; ///< A sequence of 'frames'

public:
  /** \name Structors */ ///@{
  SpriteAnimation( const std::string& strName, const AtlasManagerPtr& atlasManager );
  virtual ~SpriteAnimation();
  //@}

  /** \name Accessors */ ///@{
  const std::string& GetName() const;
  std::size_t GetFrameCount() const;
  SpriteAnimationFrame* GetFrame( std::size_t idx ) const;
  //@}

  /** \name Mutators */ ///@{
  virtual bool Load( File* is );
  //@}

}; // SpriteAnimation

} // namespace Nebulae

#endif // NEBULAE_BETA_SPRITEANIMATION_SPRITEANIMATION_H_