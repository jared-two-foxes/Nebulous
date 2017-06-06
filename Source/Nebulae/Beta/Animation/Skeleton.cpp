#include "Skeleton.h"

namespace Nebulae
{
	//constructor
	Skeleton::Skeleton()
	{

	}

	//destructor
	Skeleton::~Skeleton()
	{

	}

	//setReferencePose
	void Skeleton::setReferencePose( Transform* pReferencePose, int iBoneCount )
	{
		//m_ReferencePose.initializeFromBuffer( pReferencePose, iBoneCount*sizeof(Transform), iBoneCount );
	}

	//setParentIndices
	void Skeleton::setParentIndices( short* pIndices, int iBoneCount )
	{
		//m_ReferencePose.initializeFromBuffer( pIndices, iBoneCount*sizeof(short), iBoneCount );
	}

	//read
	void Skeleton::read( std::istream& is )
	{
		int size;
		char* szBuf;

		//TODO: Read the skeleton name.

		// Read the parent indices.
		is.read((char*)&size, sizeof(int));
		szBuf = new char[size*sizeof(short)];
		is.read((char*)szBuf[0], size*sizeof(short));
		//m_ParentIndices.initializeFromBuffer(szBuf, size*sizeof(short), size);
		delete [] szBuf;

		// Read the reference pose data.
		is.read((char*)&size, sizeof(int));
		szBuf = new char[size*sizeof(Transform)];
		is.read((char*)szBuf[0], size*sizeof(Transform));
		//m_ReferencePose.initializeFromBuffer(szBuf, size*sizeof(Transform), size);
		delete [] szBuf;
	}

	//write
	void Skeleton::write( std::ostream& os ) const
	{
		std::size_t size;
		
		//TODO: Write the skeleton name.

		// Write the parent indices.
		size = m_ParentIndices.size();
		os.write((char*)&size, sizeof(int));
		os.write((char*)&m_ParentIndices[0], size*sizeof(short));

		// Write the pose data.
		size = m_ReferencePose.size();
		os.write((char*)&size, sizeof(int));
		os.write((char*)&m_ReferencePose[0], size*sizeof(Transform));
	}
} //Nebulae