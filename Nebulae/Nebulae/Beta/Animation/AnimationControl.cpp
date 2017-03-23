#include <Nebulae/Beta/Animation/AnimationControl.h>

namespace Nebulae
{
	//constructor
	AnimationControl::AnimationControl( const Animation* pAnimation )
		: m_LocalTime( 0 )
		, m_Weight( 1.0 )
		, m_pAnimation( pAnimation )
	{

	}

	//destructor
	AnimationControl::~AnimationControl()
	{

	}

	//update
	void AnimationControl::update( Real stepDelta )
	{
		// Increase the local time.
		m_LocalTime += stepDelta;

		//TODO: Loop the local time if it extends past the animations duration?
		//TODO: Fire the animation listener if the animation gets to the end?
	}
} //Nebulae