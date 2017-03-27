#ifndef MESH_H__
#define MESH_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

class Material;
class RenderSystem;
class SubMesh;

//defines the max number of different levels-of-detail that the mesh can have.
#define MAX_LOD_COUNT	3

class Mesh
{
public:  
	struct MeshLod
	{
		std::vector< SubMesh* > m_SubMeshes;
	};
		
private:
  std::string   m_name;
  RenderSystem* m_renderer;
  MeshLod       m_lods[MAX_LOD_COUNT];

	public:
		Mesh( const std::string& name );
		~Mesh();
		
		SubMesh*               CreateSubMesh( uint16 lod );
		SubMesh*               CreateNamedSubMesh( uint16 lod, std::string name );
		std::size_t            GetSubMeshCount( uint16 lod ) const;
		std::vector<SubMesh*>& GetSubMeshes( uint16 lod );
		SubMesh*               GetSubMesh( uint16 lod, uint16 index ) const;


	protected:
		virtual bool LoadImpl_( File* is );

}; //Mesh

} //Nebulae

#endif // MESH_H__