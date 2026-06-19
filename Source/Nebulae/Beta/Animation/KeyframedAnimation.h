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
  virtual ~KeyframedAnimation();

  virtual void sampleTracks( Real time, Transform* transformTracksOut ) const;
  virtual std::size_t getNumOriginalFrames() const;
  virtual void read( std::istream& is );
  virtual void write( std::ostream& os ) const;

  void addKeyFrame( Transform* transformTracks );

}; // KeyframedAnimation

} // namespace Nebulae

#endif // NEBULAE_BETA_ANIMATION_KEYFRAMEDANIMATION_H_