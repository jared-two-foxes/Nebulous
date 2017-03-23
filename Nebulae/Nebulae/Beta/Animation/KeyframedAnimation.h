#ifndef KEYFRAMEDANIMATION_H__
#define KEYFRAMEDANIMATION_H__

#include "Animation.h"

namespace Nebulae
{

	class KeyframedAnimation : public Animation
	{
	protected:
		std::vector< Transform > m_Transforms;

	public:
		KeyframedAnimation();
		virtual ~KeyframedAnimation();

		virtual void sampleTracks( Real time, Transform* transformTracksOut ) const;
		virtual std::size_t getNumOriginalFrames() const;
		virtual void read( std::istream& is );
		virtual void write( std::ostream& os ) const;

		void addKeyFrame( Transform* transformTracks );

	}; //KeyframedAnimation

} //Nebulae

#endif // KEYFRAMEDANIMATION_H__