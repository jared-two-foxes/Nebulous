
#include "PrefabFactory.h"

#include <Nebulae/Common/Window/Window.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include <Nebulae/Beta/Mesh/Mesh.h>
#include <Nebulae/Beta/Mesh/SubMesh.h>
#include <Nebulae/Beta/Scene/Geometry.h>

using namespace Nebulae;


///
/// PlaneLoader
///

PlaneLoader::PlaneLoader( RenderSystem& renderDevice )
: m_renderDevice( renderDevice )
{}

PlaneLoader::~PlaneLoader()
{}

Geometry* 
PlaneLoader::Create( float width, float depth, int rows, int columns )
{
  int32     vertexCount = rows*columns;
  int32     indexCount  = (rows*2)*(columns-1);
  Real      dx          = width / (columns-1);
  Real      dz          = depth / (rows-1);

  VertexDeceleration* vertexDeceleration = new VertexDeceleration( 2 );
  vertexDeceleration->AddElement( VET_FLOAT3, VES_POSITION, 0 );
  vertexDeceleration->AddElement( VET_FLOAT3, VES_NORMAL, 0 );
  //vertexDeceleration->AddElement( VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );

	std::size_t vertexSize = vertexDeceleration->GetVertexSize();

  HardwareBuffer* vbuf = m_renderDevice.CreateBuffer( "", HBU_STATIC, 
    vertexCount*vertexSize, HardwareBufferBinding::HBB_VERTEX, NULL );

  HardwareBuffer* ibuf = m_renderDevice.CreateBuffer( "", HBU_STATIC, 
    indexCount*sizeof(uint16), HardwareBufferBinding::HBB_INDEX, NULL );

  UpdateIndexBuffer( ibuf, rows, columns );
  UpdateVertexBuffer( vbuf, rows, columns, dx, dz );

  Geometry* geometry = new Geometry();
  geometry->m_vertexBuffer       = vbuf;
  geometry->m_vertexDeceleration = vertexDeceleration;
  geometry->m_vertexCount        = vertexCount;
  geometry->m_indexBuffer        = ibuf;
  geometry->m_indexCount         = indexCount;
  geometry->m_primitiveTopology  = OT_TRIANGLELIST;

  return geometry;
}

void 
PlaneLoader::UpdateVertexBuffer( HardwareBuffer* vb, int numRows, int numColumns, float dx, float dz  )
{
	NE_ASSERT( vb != nullptr, "UpdateVertexBuffer recieved a null buffer parameter" )();

	// Create the vertex scratch.
	int32 vertexCount   = numRows*numColumns;
	Real* vertexScratch = new Real[vertexCount*6]; //6 elements; 3-pos, 3-norm
	
	int32 idx = 0;

	for( int32 i = 0; i < numRows; ++i )
	{
		Real z = -0.5f*(numColumns-1)*dz + i*dz;

		for( int32 j = 0; j < numColumns; ++j )
		{
			Real x = -0.5f*(numColumns-1)*dx + j*dx;

			// Calculate position.
			vertexScratch[idx++] = x;
			vertexScratch[idx++] = 0;
			vertexScratch[idx++] = z;

			// Calculate normal
			vertexScratch[idx++] = 0;
			vertexScratch[idx++] = 1.0f;
			vertexScratch[idx++] = 0;
		}
	}

  vb->WriteData( 0, vertexCount*6*sizeof(Real), vertexScratch );
	
	delete [] vertexScratch;
}

void 
PlaneLoader::UpdateIndexBuffer( HardwareBuffer* ib, int rows, int columns )
{
	NE_ASSERT( ib != nullptr, "UpdateVertexBuffer recieved a null buffer parameter" )();

	// Create the index scratch
	int32   indexCount   = (rows*2)*(columns-1);
	uint16* indexScratch = new uint16[indexCount];

	int idx = 0;
	for( int32 i = 0; i < (rows-1); ++i )
	{
		for( int32 j = 0; j < columns; ++j )
		{
			indexScratch[idx++] = i*columns + j;
			indexScratch[idx++] = i*columns + j + columns;
		}
	}

	ib->WriteData( 0, indexCount*sizeof(uint16), indexScratch );

	delete [] indexScratch;
}



///
/// CubeLoader
///

CubeLoader::CubeLoader( RenderSystem& renderDevice )
: m_renderDevice( renderDevice )
{}

CubeLoader::~CubeLoader()
{}

Geometry*
CubeLoader::Create( float width, float height, float depth, int rows, int columns, int slices )
{
	const int NUM_VERTICES           = 4 * 6; // 4 vertices per side * 6 sides
	const int NUM_ENTRIES_PER_VERTEX = 6;
	const int NUM_VERTEX_ENTRIES     = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
	const int NUM_INDICES            = 3 * 2 * 6; // 3 indices per face * 2 faces per side * 6 sides

	const Real CUBE_SIZE      = 2.0f;
	const Real CUBE_HALF_SIZE = CUBE_SIZE / 2.0f;

  const Real dx = width / (rows - 1);
	const Real dy = height / (columns - 1);
	const Real dz = depth / (slices - 1);

  Geometry* geometry = new Geometry();

	geometry->m_vertexDeceleration = new VertexDeceleration( 2 );
  geometry->m_vertexDeceleration->AddElement( VET_FLOAT3, VES_POSITION, 0 );
  geometry->m_vertexDeceleration->AddElement( VET_FLOAT3, VES_NORMAL, 0 );
  geometry->m_vertexDeceleration->AddElement( VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );

	geometry->m_vertexBuffer = m_renderDevice.CreateBuffer( "", HBU_STATIC, 
		NUM_VERTEX_ENTRIES*sizeof(Real), HardwareBufferBinding::HBB_VERTEX, NULL );
	geometry->m_indexBuffer = m_renderDevice.CreateBuffer( "", HBU_STATIC, 
		NUM_INDICES*sizeof(uint16), HardwareBufferBinding::HBB_INDEX, NULL );
			
  UpdateVertexBuffer( geometry->m_vertexBuffer, rows, columns, slices, dx, dy, dz );
	UpdateIndexBuffer( geometry->m_indexBuffer, rows, columns, slices );

  geometry->m_vertexCount = NUM_VERTICES;
	//geometry->m_iVertexSize  = NUM_ENTRIES_PER_VERTEX*sizeof(float);
	geometry->m_indexCount = NUM_INDICES;
	geometry->m_primitiveTopology = OT_TRIANGLES;

	return geometry;
}

void 
CubeLoader::UpdateVertexBuffer( HardwareBuffer* vb, int rows, int columns, int slices, float dx, float dy, float dz )
{
	const int NUM_VERTICES           = 4 * 6; // 4 vertices per side * 6 sides
	const int NUM_ENTRIES_PER_VERTEX = 6;
	const int NUM_VERTEX_ENTRIES     = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;

	Real vertexScratch[NUM_VERTEX_ENTRIES] = 
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

	vb->WriteData( 0, NUM_VERTEX_ENTRIES*sizeof(Real), vertexScratch );
}

void 
CubeLoader::UpdateIndexBuffer( HardwareBuffer* ibuf, int rows, int columns, int slices )
{
	const int NUM_INDICES = 3 * 2 * 6; // 3 indices per face * 2 faces per side * 6 sides

	// Create index buffer
	uint16 indexScratch[NUM_INDICES] =
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

	ibuf->WriteData( 0, NUM_INDICES*sizeof(uint16), indexScratch );
}
