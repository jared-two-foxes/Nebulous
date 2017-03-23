#include "Animation.h"

namespace Nebulae
{
	//constructor
	Animation::Animation()
		: m_Duration( 0 )
		, m_iNumberOfTransformTracks( 0 )
		, m_iTransformsPerFrame( 0 )
	{

	}

	//read
	void Animation::read( std::istream& is )
	{
		is.read((char*)&m_Duration, sizeof(Real));
		is.read((char*)&m_iNumberOfTransformTracks, sizeof(int));
		is.read((char*)&m_iTransformsPerFrame, sizeof(int));
	}
	
	//write
	void Animation::write( std::ostream& os ) const
	{
		os.write((char*)&m_Duration, sizeof(Real));
		os.write((char*)&m_iNumberOfTransformTracks, sizeof(int));
		os.write((char*)&m_iTransformsPerFrame, sizeof(int));
	}
}
