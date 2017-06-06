#include "KeyframedAnimation.h"

namespace Nebulae
{
	//constructor
	KeyframedAnimation::KeyframedAnimation()
	{

	}

	//destructor
	KeyframedAnimation::~KeyframedAnimation()
	{

	}

	//sampleTracks
	void KeyframedAnimation::sampleTracks( Real time, Transform* transformTracksOut ) const
	{
		// Determine which frame to return
		int first = static_cast<int>(floorf(time/getNumOriginalFrames()));

		//TODO: Lerp between frames to output a smooth set.

		// Copy transforms into buffer.
		memcpy(transformTracksOut, &m_Transforms[first*m_iTransformsPerFrame], sizeof(Transform)*m_iTransformsPerFrame);
	}

	//getNumOriginalFrames
	std::size_t KeyframedAnimation::getNumOriginalFrames() const
	{
		return (m_Transforms.size() / m_iTransformsPerFrame);
	}

	//addKeyFrame
	void KeyframedAnimation::addKeyFrame( Transform* transformTracks )
	{
		for (int i = 0; i<m_iTransformsPerFrame; ++i)
		{
			m_Transforms.push_back( transformTracks[i] );
		}

		//Increase the number of transform tracks.
		m_iNumberOfTransformTracks += m_iTransformsPerFrame;
	}

	//read
	void KeyframedAnimation::read( std::istream& is )
	{
		//TODO: Do Object Header?

		// Do common Animation values first.
		Animation::read( is );

		char* szBuf = new char[m_iNumberOfTransformTracks*sizeof(Transform)];
		is.read((char*)&szBuf[0], m_iNumberOfTransformTracks*sizeof(Transform));

		//m_Transforms.initializeFromBuffer( szBuf, m_iNumberOfTransformTracks*sizeof(Transform), m_iNumberOfTransformTracks );

    delete [] szBuf;
	}

	//write
	void KeyframedAnimation::write( std::ostream& os ) const
	{
		//TODO: Do Object Header?

		// Do common Animation values first.
		Animation::write( os );

		os.write((char*)&m_Transforms[0], m_iNumberOfTransformTracks*sizeof(Transform));
	}
}