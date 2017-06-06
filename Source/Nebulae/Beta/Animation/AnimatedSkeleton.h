#ifndef ANIMATEDSKELETON_H__
#define ANIMATEDSKELETON_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
	class AnimationControl;
	class Skeleton;

	class AnimatedSkeleton
	{
	protected:
		const Skeleton* m_pSkeleton;
		std::vector< AnimationControl* > m_AnimationControls;

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

	}; //AnimatedSkeleton

} //Nebulae

#endif // ANIMATEDSKELETON_H__