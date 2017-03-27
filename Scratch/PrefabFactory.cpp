
#include "PrefabFactory.h"

#include <Nebulae/Common/Window/Window.h>
#include <Nebulae/Alpha/Shared/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/Shared/Mesh/Mesh.h>
#include <Nebulae/Alpha/Shared/Mesh/SubMesh.h>
#include <Nebulae/Alpha/Shared/RenderSystem/RenderSystem.h>

using namespace Nebulae;


//constructor
PlaneLoader::PlaneLoader( RenderSystem& renderDevice, const std::string& materialName, float width, float depth, int rows, int columns )
	: m_renderDevice( renderDevice ),
    m_MaterialName( materialName ),
		m_Width( width ),
		m_Depth( depth ),
		m_Rows( rows ),
		m_Columns( columns )
{

}

//destructor
PlaneLoader::~PlaneLoader()
{

}

//LoadResource
void PlaneLoader::LoadResource( Resource* resource )
{
	Mesh* mesh = static_cast<Mesh*>(resource);

	SubMesh* sub = mesh->CreateSubMesh(0);

	// Create vertex buffer
	int vertexCount = m_Rows*m_Columns;
		
	HardwareBuffer* vbuf = m_renderDevice.GetBufferManager()->Create( "", HardwareBufferUsage::HBU_STATIC, 
		vertexCount*6*sizeof(float), HardwareBufferBinding::HBB_VERTEX, NULL );

	float dx = m_Width / (m_Columns-1);
	float dz = m_Depth / (m_Rows-1);
	UpdateVertexBuffer( vbuf, m_Rows, m_Columns, dx, dz );

	sub->m_pVertexBuffer = vbuf;
	sub->m_iVertexCount = vertexCount;
	sub->m_iVertexSize = 6*sizeof(float);
		
	sub->m_pVertexElements = new VertexDeceleration( 2 );
	sub->m_pVertexElements->AddElement( VET_FLOAT3, VES_POSITION, 0 );
	sub->m_pVertexElements->AddElement( VET_FLOAT3, VES_NORMAL, 0 );
	sub->m_pVertexElements->AddElement( VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );

	sub->m_DeleteDecleration = true;

	int indexCount = (m_Rows*2)*(m_Columns-1);

	HardwareBuffer* ibuf = m_renderDevice.GetBufferManager()->Create( "", HardwareBufferUsage::HBU_STATIC, 
		indexCount*sizeof(unsigned short), HardwareBufferBinding::HBB_INDEX, NULL );
			
	UpdateIndexBuffer( ibuf, m_Rows, m_Columns );

	sub->m_pIndexBuffer      = ibuf;
	sub->m_iIndexCount       = indexCount;
	sub->m_PrimitiveTopology = OT_TRIANGLES;

	sub->SetMaterialName( m_MaterialName );
}

//UpdateVertexBuffer
void PlaneLoader::UpdateVertexBuffer( HardwareBuffer* vb, int numRows, int numColumns, float dx, float dz  )
{
	// Create vertex scratch.
	/*int vertexCount = numRows*numColumns;
	Real* vertexScratch = new Real[vertexCount*6]; //6 elements; 3-pos, 3-norm
	assert(vertexScratch && "PlaneLoader::UpdateVertexBuffer - Failed to create scratch.");

	int idx = 0;

	for (int i = 0; i<numRows; ++i)
	{
		Real z = -0.5f*(numColumns-1)*dz + i*dz;

		for (int j = 0; j<numColumns; ++j)
		{
			Real x = -0.5f*(numColumns-1)*dx + j*dx;

			// Calc position.
			vertexScratch[idx++] = x;
			vertexScratch[idx++] = 0;
			vertexScratch[idx++] = z;

			// Calc normal
			vertexScratch[idx++] = 0;
			vertexScratch[idx++] = 1.0f;
			vertexScratch[idx++] = 0;
		}
	}

	ID3D11DeviceContext* pImmediateContext = NULL;
	Window::ms_pD3D11Device->GetImmediateContext( &pImmediateContext );
	assert( vb );
	pImmediateContext->UpdateSubresource( vb->GetHardwarePointer(), 0, NULL, vertexScratch, 0, 0 );

	delete [] vertexScratch;*/
}

//UpdateIndexBuffer
void PlaneLoader::UpdateIndexBuffer( HardwareBuffer* ibuf, int rows, int columns )
{
	// Create index buffer
	/*int indexCount = (rows*2)*(columns-1);
	unsigned short *indexScratch = new unsigned short[indexCount];

	int idx = 0;
	for (int i = 0; i<(rows-1); ++i)
		for (int j = 0; j<columns; ++j)
		{
			indexScratch[idx++] = i*columns + j;
			indexScratch[idx++] = i*columns + j + columns;
		}

	ID3D11DeviceContext* pImmediateContext = NULL;
	Window::ms_pD3D11Device->GetImmediateContext( &pImmediateContext );
	assert( ibuf );
	pImmediateContext->UpdateSubresource( ibuf->GetHardwarePointer(), 0, NULL, indexScratch, 0, 0 );*/
}

//constructor
CubeLoader::CubeLoader( RenderSystem& renderDevice, const std::string& materialName, float width, float height, float depth, int rows, int columns, int slices )
  : m_renderDevice( renderDevice )
{
	m_MaterialName = materialName;
	m_Width        = width;
	m_Height       = height;
	m_Depth        = depth;
	m_Rows         = rows;
	m_Columns      = columns;
	m_Slices       = slices;
}
	
//destructor
CubeLoader::~CubeLoader()
{

}

//LoadResource
void CubeLoader::LoadResource( Resource* resource )
{
	Mesh* mesh = static_cast<Mesh*>(resource);

	SubMesh* sub = mesh->CreateSubMesh(0);

	const int NUM_VERTICES = 4 * 6; // 4 vertices per side * 6 sides
	const int NUM_ENTRIES_PER_VERTEX = 6;
	const int NUM_VERTEX_ENTRIES = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
	const int NUM_INDICES = 3 * 2 * 6; // 3 indices per face * 2 faces per side * 6 sides

	const float CUBE_SIZE = 2.0f;
	const float CUBE_HALF_SIZE = CUBE_SIZE / 2.0f;

	sub->m_pVertexElements = new VertexDeceleration( 2 );
  sub->m_pVertexElements->AddElement( VET_FLOAT3, VES_POSITION, 0 );
  sub->m_pVertexElements->AddElement( VET_FLOAT3, VES_NORMAL, 0 );
  sub->m_pVertexElements->AddElement( VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );

	sub->m_DeleteDecleration = true;

	sub->m_pVertexBuffer = m_renderDevice.GetBufferManager()->Create( "", HardwareBufferUsage::HBU_STATIC, 
		NUM_VERTEX_ENTRIES*sizeof(float), HardwareBufferBinding::HBB_VERTEX, NULL );

	float dx = m_Width / (m_Rows - 1);
	float dy = m_Height / (m_Columns - 1);
	float dz = m_Depth / (m_Slices - 1);
	UpdateVertexBuffer( sub->m_pVertexBuffer, m_Rows, m_Columns, m_Slices, dx, dy, dz );

	sub->m_iVertexCount = NUM_VERTICES;
	sub->m_iVertexSize = NUM_ENTRIES_PER_VERTEX*sizeof(float);

	sub->m_pIndexBuffer = m_renderDevice.GetBufferManager()->Create( "", HardwareBufferUsage::HBU_STATIC, 
		NUM_INDICES*sizeof(unsigned short), HardwareBufferBinding::HBB_INDEX, NULL );
			
	UpdateIndexBuffer( sub->m_pIndexBuffer, m_Rows, m_Columns, m_Slices );

	sub->m_iIndexCount = NUM_INDICES;
	sub->m_PrimitiveTopology = OT_TRIANGLES;

	sub->SetMaterialName("Default");
}

//UpdateVertexBuffer
void CubeLoader::UpdateVertexBuffer( HardwareBuffer* vb, int rows, int columns, int slices, float dx, float dy, float dz )
{
	/*const int NUM_VERTICES = 4 * 6; // 4 vertices per side * 6 sides
	const int NUM_ENTRIES_PER_VERTEX = 6;
	const int NUM_VERTEX_ENTRIES = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
	const int NUM_INDICES = 3 * 2 * 6; // 3 indices per face * 2 faces per side * 6 sides

	float vertexScratch[NUM_VERTEX_ENTRIES] = 
	{
		// up side
		-1.0f, 1.0f, -1.0f, 
		0,1,0,
		1.0f, 1.0f, -1.0f,
		0,1,0,
		1.0f, 1.0f, 1.0f,
		0,1,0,
		-1.0f, 1.0f, 1.0f, 
		0,1,0,

		// down side
		-1.0f, -1.0f, -1.0f,
		0,-1,0,
		1.0f, -1.0f, -1.0f,
		0,-1,0,
		1.0f, -1.0f, 1.0f,
		0,-1,0,
		-1.0f, -1.0f, 1.0f,
		0,-1,0,

		// left side
		-1.0f, -1.0f, 1.0f,
		-1,0,0,
		-1.0f, -1.0f, -1.0f,
		-1,0,0,
		-1.0f, 1.0f, -1.0f,
		-1,0,0,
		-1.0f, 1.0f, 1.0f,
		-1,0,0,

		// right side
		1.0f, -1.0f, 1.0f,
		1,0,0,
		1.0f, -1.0f, -1.0f,
		1,0,0,
		1.0f, 1.0f, -1.0f,
		1,0,0,
		1.0f, 1.0f, 1.0f,
		1,0,0,

		// back side
		-1.0f, -1.0f, -1.0f,
		0,0,-1,
		1.0f, -1.0f, -1.0f,
		0,0,-1,
		1.0f, 1.0f, -1.0f,
		0,0,-1,
		-1.0f, 1.0f, -1.0f,
		0,0,-1,

		// front side
		-1.0f, -1.0f, 1.0f,
		0,0,1,
		1.0f, -1.0f, 1.0f,
		0,0,1,
		1.0f, 1.0f, 1.0f,
		0,0,1,
		-1.0f, 1.0f, 1.0f,
		0,0,1,
	};

	ID3D11DeviceContext* pImmediateContext = NULL;
	Window::ms_pD3D11Device->GetImmediateContext( &pImmediateContext );
	assert( vb );
	pImmediateContext->UpdateSubresource( vb->GetHardwarePointer(), 0, NULL, vertexScratch, 0, 0 );

//		delete [] vertexScratch;*/
}

//UpdateIndexBuffer
void CubeLoader::UpdateIndexBuffer( HardwareBuffer* ibuf, int rows, int columns, int slices )
{
	/*const int NUM_VERTICES = 4 * 6; // 4 vertices per side * 6 sides
	const int NUM_ENTRIES_PER_VERTEX = 6;
	const int NUM_VERTEX_ENTRIES = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
	const int NUM_INDICES = 3 * 2 * 6; // 3 indices per face * 2 faces per side * 6 sides

	// Create index buffer
	unsigned short indexScratch[NUM_INDICES] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	ID3D11DeviceContext* pImmediateContext = NULL;
	Window::ms_pD3D11Device->GetImmediateContext( &pImmediateContext );
	assert( ibuf );
	pImmediateContext->UpdateSubresource( ibuf->GetHardwarePointer(), 0, NULL, indexScratch, 0, 0 );

//		delete [] indexScratch;*/
}
