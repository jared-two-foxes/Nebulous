#ifndef NEBULAE_SUBMESH_H__
#define NEBULAE_SUBMESH_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/RenderSystem/OperationType.h>

namespace Nebulae
{

//Forward Decleration
class HardwareBuffer;
class Mesh;
class RenderSystem;
class VertexDeceleration;


/** SubMesh
	*/
class SubMesh	
{
protected:
	// Owning mesh
	Mesh* m_pParent;

  public:
	  explicit SubMesh( Mesh* pParent );
	  ~SubMesh();

	  void SetMaterialName( const std::string& strMaterialName );

	  //Vertex Layout
	  VertexDeceleration* m_pVertexElements;
	  //flag indicating if m_vertexElements should be deleted on deconstruction.
	  bool m_DeleteDecleration;

	  //Vertex Data
	  HardwareBuffer* m_pVertexBuffer;
	  unsigned int m_iVertexCount;
	  unsigned int m_iVertexSize;
		
	  //Index Data
	  HardwareBuffer* m_pIndexBuffer;
	  unsigned int m_iIndexCount;

	  //Primitive topology
	  OperationType m_PrimitiveTopology;	

	  //Name of material to use to render this SubMesh
	  std::string m_strMaterialName;

	  //! Tmp.
	  bool m_Skinned;
	  std::vector< std::string > m_BoneOrder;

}; //SubMesh

} //Nebulae

#endif // NEBULAE_SUBMESH_H__