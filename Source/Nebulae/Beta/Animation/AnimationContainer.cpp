#include <Nebulae/Beta/Animation/AnimationContainer.h>
#include <Nebulae/Beta/Animation/KeyframedAnimation.h>
#include <Nebulae/Beta/Animation/Skeleton.h>

namespace Nebulae
{
	//constructor
	AnimationContainer::AnimationContainer()
	{

	}

	//destructor
	AnimationContainer::~AnimationContainer()
	{
		for ( std::size_t i = 0, n = m_Animations.size(); i<n; i++ )
		{
			delete m_Animations[i];
		}

		for ( std::size_t i = 0, n = m_Skeletons.size(); i<n; ++i )
		{
			delete m_Skeletons[i];
		}
	}

	//addAnimation
	void AnimationContainer::addAnimation( Animation* animation )
	{
		m_Animations.push_back(animation);
	}

	//getAnimation
	Animation* AnimationContainer::getAnimation( int idx ) const
	{
		return m_Animations[idx];
	}

	//addSkeleton
	void AnimationContainer::addSkeleton( Skeleton* skel )
	{
		m_Skeletons.push_back(skel);
	}

	//getSkeleton
	Skeleton* AnimationContainer::getSkeleton( int idx ) const
	{
		return m_Skeletons[idx];
	}

	//read
	void AnimationContainer::read( std::istream& is )
	{
		int size;

		// Read the Animations.
		is.read((char*)&size, sizeof(int));
		m_Animations.reserve(size);
		for (int i = 0; i<size; i++)
		{
			// For now all these will be KeyframedAnimation's
			KeyframedAnimation* pAnim = new KeyframedAnimation();
			// Stream/Fix the animation.
			pAnim->read( is );

			m_Animations.push_back( pAnim );
		}

		// Read the skeletons.
		is.read((char*)&size, sizeof(int));
		m_Skeletons.reserve(size);
		for (int i = 0; i<size; i++)
		{
			// Create the skeleton class.
			Skeleton* pSkel = new Skeleton();
			// Stream the skeleton data.
			pSkel->read( is );

			m_Skeletons.push_back( pSkel );
	 	}
	}

	//write
	void AnimationContainer::write( std::ostream& os ) const
	{
		std::size_t size;
		
		// Write the animation data.
		size = m_Animations.size();
		os.write((char*)&size, sizeof(int));
		for ( std::size_t i = 0, n = m_Animations.size(); i<n; i++ )
		{
			m_Animations[i]->write( os );
	 	}

		// Write the skeleton data.
		size = m_Skeletons.size();
		os.write((char*)&size, sizeof(int));
		for ( std::size_t i = 0, n = m_Skeletons.size(); i<n; i++ )
		{
			m_Skeletons[i]->write( os );
	 	}
	}
}