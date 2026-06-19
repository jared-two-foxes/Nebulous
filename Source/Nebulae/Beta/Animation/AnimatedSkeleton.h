#ifndef NEBULAE_BETA_ANIMATION_ANIMATEDSKELETON_H_
#define NEBULAE_BETA_ANIMATION_ANIMATEDSKELETON_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
class AnimationControl;
class Skeleton;

class AnimatedSkeleton
{
protected:
  const Skeleton* m_pSkeleton;
  std::vector<AnimationControl*> m_AnimationControls;

public:
  explicit AnimatedSkeleton( const Skeleton* skeleton );
  ~AnimatedSkeleton();

  void update( float stepDeltaTime );
  void sampleAndCombineAnimations( float time, Transform* transformTracksOut );
  const Skeleton* getSkeleton() const { return m_pSkeleton; }


  void addAnimationControl( AnimationControl* control );
  void removeAnimationControl( AnimationControl* control );
  int getNumAnimationCotnrolCount() const;
  AnimationControl* getAnimationControl( int idx ) const;

}; // AnimatedSkeleton

} // namespace Nebulae

#endif // NEBULAE_BETA_ANIMATION_ANIMATEDSKELETON_H_