#ifndef ANIMATIONCONTROL_H__
#define ANIMATIONCONTROL_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
	class Animation;

	//TODO: Havok uses a hkAnimationBinding to handle binding an animation to a skeleton rather than directly referencing the animation
	//		in the hkAnimationControl class.  Implement something similar?
	class AnimationControl
	{
	protected:
		Real m_LocalTime;
    Real m_Weight;
		const Animation* m_pAnimation;

	public:
		AnimationControl( const Animation* pAnimation );
		~AnimationControl();

			/// Advance the local clock by the specified delta time and return the new state
		void update(Real stepDelta );

			/// Return the local time for the control
		inline Real getLocalTime() const;

			/// Set the local time for the animation
		inline void setLocalTime(Real lTime );

			/// Get the current weight this control
		inline Real getWeight() const;

			/// Get the pose at the control local time
		void sampleTracks( Transform* transformTracksOut/*, btScalar* floatTracksOut*/ ) const;

		//TODO: Add an animation listener??

	}; //AnimationControl

} //Nebulae

#endif // ANIMATIONCONTROL_H__