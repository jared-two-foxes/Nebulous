#ifndef NEBULAE_ANIMATIONCONTAINER_H__
#define NEBULAE_ANIMATIONCONTAINER_H__

#include <Nebulae/Common/Common.h>
//#include <Nebulae/Common/Object/Serializable.h>
//#include <Nebulae/Common/Object/SerializableFactory.h>

namespace Nebulae
{
	class Animation;
	class Skeleton;

	class AnimationContainer// : public Serializable
	{
	protected:
		std::vector<Animation* > m_Animations;
    std::vector<Skeleton* >  m_Skeletons;

	public:
		AnimationContainer();
		~AnimationContainer();

		// Animations
		void        addAnimation( Animation* animation );
		std::size_t getAnimationCount() const { return m_Animations.size(); }
		Animation*  getAnimation( int idx ) const;

		// Skeletons
		void addSkeleton( Skeleton* skel );
		std::size_t getSkeletonCount() const { return m_Skeletons.size(); }
		Skeleton* getSkeleton( int idx ) const;

		//Serializable impl.
		virtual void read( std::istream& is );
		virtual void write( std::ostream& os ) const;

	}; //AnimationContainer

	//class AnimationContainerFactory : public SerializableFactory
	//{
	//public:
	//	virtual Serializable* createObject() const { return new AnimationContainer(); }

	//}; //AnimationContainerFactory

} //Nebulae

#endif // NEBULAE_ANIMATIONCONTAINER_H__