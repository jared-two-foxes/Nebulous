#include <Nebulae/Beta/Animation/AnimatedSkeleton.h>
#include <Nebulae/Beta/Animation/AnimationControl.h>

namespace Nebulae
{
	//constructor
	AnimatedSkeleton::AnimatedSkeleton( const Skeleton* pSkeleton )
		: m_pSkeleton( pSkeleton )
	{

	}

	//destructor
	AnimatedSkeleton::~AnimatedSkeleton()
	{
		// Delete all the remaining AnimationControl
		for ( std::size_t i = 0, n = m_AnimationControls.size(); i<n; ++i )
		{
			assert( m_AnimationControls[i] );
			delete m_AnimationControls[i];
		}
	}

	//update
	void AnimatedSkeleton::update( float deltaTimeStep )
	{
		// Update the animation controls.
		for ( std::size_t i = 0, n = m_AnimationControls.size(); i<n; ++i )
		{
			m_AnimationControls[i]->update( deltaTimeStep );
		}
	}
} //Nebulae