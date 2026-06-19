#ifndef NEBULAE_BETA_ANIMATION_KEYFRAMEDANIMATION_H_
#define NEBULAE_BETA_ANIMATION_KEYFRAMEDANIMATION_H_

#include "Animation.h"

namespace Nebulae
{

class KeyframedAnimation : public Animation
{
protected:
  std::vector<Transform> m_Transforms;

public:
  KeyframedAnimation();
  ~KeyframedAnimation() override;

  void sampleTracks( Real time, Transform* transformTracksOut ) const override;
  std::size_t getNumOriginalFrames() const override;
  void read( std::istream& is ) override;
  void write( std::ostream& os ) const override;

  void addKeyFrame( Transform* transformTracks );

}; // KeyframedAnimation

} // namespace Nebulae

#endif // NEBULAE_BETA_ANIMATION_KEYFRAMEDANIMATION_H_