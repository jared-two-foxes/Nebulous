
#include "SubMesh.h"

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

namespace Nebulae
{
// constructor
SubMesh::SubMesh( Mesh* parent )
  : m_pParent( parent ),
    m_pVertexElements( nullptr ),
    m_DeleteDecleration( false ),
    m_pVertexBuffer( nullptr ),
    m_iVertexCount( 0 ),
    m_iVertexSize( 0 ),
    m_pIndexBuffer( nullptr ),
    m_iIndexCount( 0 ),
    m_PrimitiveTopology( OT_UNKNOWN ),
    m_Skinned( false )
{
}

// destructor
SubMesh::~SubMesh()
{
  if ( m_DeleteDecleration )
  {
    delete m_pVertexElements;
  }
}

// SetMaterialName
void SubMesh::SetMaterialName( const std::string& materialName ) { m_strMaterialName = materialName; }
} // namespace Nebulae