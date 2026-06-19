#ifndef NEBULAE_BETA_MESH_SUBMESH_H_
#define NEBULAE_BETA_MESH_SUBMESH_H_

#include <Nebulae/Alpha/RenderSystem/OperationType.h>
#include <Nebulae/Common/Common.h>

namespace Nebulae
{

// Forward Decleration
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

  // Vertex Layout
  VertexDeceleration* m_pVertexElements;
  // flag indicating if m_vertexElements should be deleted on deconstruction.
  bool m_DeleteDecleration;

  // Vertex Data
  HardwareBuffer* m_pVertexBuffer;
  unsigned int m_iVertexCount;
  unsigned int m_iVertexSize;

  // Index Data
  HardwareBuffer* m_pIndexBuffer;
  unsigned int m_iIndexCount;

  // Primitive topology
  OperationType m_PrimitiveTopology;

  // Name of material to use to render this SubMesh
  std::string m_strMaterialName;

  //! Tmp.
  bool m_Skinned;
  std::vector<std::string> m_BoneOrder;

}; // SubMesh

} // namespace Nebulae

#endif // NEBULAE_BETA_MESH_SUBMESH_H_