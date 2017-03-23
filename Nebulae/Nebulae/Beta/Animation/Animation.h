#ifndef ANIAMTION_H__
#define ANIAMTION_H__

#include <Nebulae/Common/Common.h>
//#include <Nebulae/Common/Object/Serializable.h>

namespace Nebulae
{

	class Animation //: public Serializable
	{
	protected:
			/// The length of the animation cycle in seconds.
		Real m_Duration;

		int m_iNumberOfTransformTracks;
		//int m_iNumberOfFloatTracks;

			/// The number of bone tracks to be animated.
		int m_iTransformsPerFrame;

	public:
		Animation();
		virtual ~Animation() {}

		void setDuration( Real duration ) { m_Duration = duration; }
		Real getDuration() const { return m_Duration; }

		void setTransformsPerFrame( int iTransformsPerFrame ) { m_iTransformsPerFrame = iTransformsPerFrame; }
		int getTransformsPerFrame() const { return m_iTransformsPerFrame; }

		virtual void sampleTracks( Real time, Transform* pTransformTracksOut ) const = 0;

		/** Returns the number of original samples / frames of animation.
		 */
		virtual std::size_t getNumOriginalFrames() const = 0;

		/** Get the frame/sample index and delta between frames. So 0 <= frameOut < getNumOriginalFrames() and 0 <= delta < 1.0
		 */
		void getFrameAndDelta( Real time, unsigned& frameOut, unsigned& deltaOut ) const;

		virtual void read( std::istream& is );
		virtual void write( std::ostream& os ) const;

	}; //Animation

} //Nebulae

#endif // ANIMATION_H__
