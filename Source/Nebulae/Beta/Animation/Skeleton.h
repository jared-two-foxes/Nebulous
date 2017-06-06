#ifndef SKELETON_H__
#define SKELETON_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{
	class Skeleton
	{
	protected:
		// A user name to aid in identifying the skeleton.
		std::string m_Name;
		// Parent relationship
		std::vector< short > m_ParentIndices;
		// The reference pose of this skeleton.  This pose is stored in local space?
    std::vector< Transform > m_ReferencePose;

	public:
		Skeleton();
		~Skeleton();

		void setName( const std::string& name ) { m_Name = name; }
		const std::string& getName() const { return m_Name; }

		void setReferencePose( Transform* pReferencePose, int iBoneCount );
		void getReferencePose(std::vector< Transform >& pose );

		void setParentIndices( short* pIndices, int iBoneCount );
		void getParentIndices(std::vector< short >& indices );

		void read( std::istream& is );
		void write( std::ostream& os ) const;

	}; //Skeleton

} //Nebulae

#endif //SKELETON_H__