#ifndef NEBULAE_PRIMITIVEBATCH_H__
#define NEBULAE_PRIMITIVEBATCH_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>

namespace Nebulae
{

class Effect;


template <class VertexType >
class PrimitiveBatch
///
/// Helper class to help with rendering standard shapes and structures, is responsible for
/// setting the vertex layout and submitting vertex & index data.
///
{
public:
  static const std::size_t DefaultBatchSize = 2048;

public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

private:
  RenderSystemPtr     m_renderSystem;
  HardwareBuffer*     m_vertexBuffer;
  char*               m_scratchBuffer;     
  std::size_t         m_vertexBufferSize;
  std::size_t         m_count;
  Effect*             m_boundEffect;
  VertexDeceleration* m_decleration;
  InputLayout*        m_inputLayout;

  public:

    PrimitiveBatch( RenderSystemPtr renderSystem, size_t maxIndices = DefaultBatchSize * 3, size_t maxVertices = DefaultBatchSize )
    : m_renderSystem( renderSystem ),
      m_vertexBuffer( nullptr ), 
      m_scratchBuffer( nullptr ),
      m_vertexBufferSize( 0 ),
      m_count( 0 ),
      m_boundEffect( nullptr ),
      m_decleration( nullptr ),
      m_inputLayout( nullptr )
    {
      m_vertexBufferSize = maxVertices * sizeof(VertexType);
      m_vertexBuffer     = renderSystem->CreateBuffer( "", HBU_DYNAMIC_WRITE_ONLY, m_vertexBufferSize, HBB_VERTEX, nullptr );
      m_scratchBuffer    = (char*)_aligned_malloc( DefaultBatchSize * sizeof(VertexType), 16 );

      //m_indexBuffer = renderSystem->CreateBuffer( "", HBU_DYNAMIC_WRITE_ONLY, maxIndices * sizeof(uint16_t), HBB_INDEX, nullptr );

      // Create a vertex decleration for this vertex type.
      m_decleration = new VertexDeceleration( 2 );
      m_decleration->AddElement( VET_FLOAT4, VES_POSITION, 0 );
      m_decleration->AddElement( VET_FLOAT4, VES_DIFFUSE, 0 );
    }

    ~PrimitiveBatch()
    {
      if ( m_scratchBuffer )
        _aligned_free( m_scratchBuffer );

      if ( m_vertexBuffer )
        delete m_vertexBuffer;

      if( m_decleration )
        delete m_decleration;
    }

    void Begin( Effect* effect )
    {
      if( m_boundEffect != effect )
      {
        m_boundEffect = effect;

        m_inputLayout = m_renderSystem->CreateInputLayout( "batch_layout", m_decleration, m_boundEffect->GetVertexShader() );
      }

      m_count  = 0;
    }

    void End()
    {
      if( m_count > 0 )
        Flush();
    }

    void Flush()
    {
      NE_ASSERT( m_count > 0, "" )();
      
      m_boundEffect->Apply( m_renderSystem );

      int size = sizeof(VertexType);

      // Map the buffer for writing.
      m_vertexBuffer->WriteData( 0, m_count*sizeof(VertexType), m_scratchBuffer, true );
      //void* pMappedMemory = m_vertexBuffer->Lock( 0, m_count*sizeof(VertexType), HBL_DISCARD );
      //memcpy( pMappedMemory, m_scratchBuffer, m_count*sizeof(VertexType) );
      //m_vertexBuffer->Unlock();
    
      // Set the Vertex Buffer.
      std::size_t iOffset = 0;
      std::size_t iStride = 0; //sizeof(VertexType);
      m_renderSystem->SetVertexBuffers( 0, m_vertexBuffer, iStride, iOffset );
        
      // Set the Vertex input layout.
      m_renderSystem->SetInputLayout( m_inputLayout );

      // @todo get and reset depth test?
      m_renderSystem->SetDepthTest( false );

      // Draw to backbuffer.
      m_renderSystem->SetOperationType( OT_LINES );
      m_renderSystem->Draw( m_count, 0 );

      // @todo get and reset depth test?
      m_renderSystem->SetDepthTest( true );

      m_count = 0;
    }

    void DrawLine( VertexType v1, VertexType v2 )
    {
      VertexType* mappedVertices;

      Draw( OT_LINELIST, false, nullptr, 0, 2, reinterpret_cast<void**>(&mappedVertices) );

      mappedVertices[0] = v1;
      mappedVertices[1] = v2;
    }

    void DrawTriangle( VertexType v1, VertexType v2, VertexType v3 );
    void DrawQuad( VertexType v1, VertexType v2, VertexType v3, VertexType v4 );
    
    //void Draw( topology, uint32 count, VertexType* vertices );
    //void DrawIndexed( topology, uint32 indexCount, uint16* indices, uint32 vertexCount, VertexType* vertices );

  private:
    // Internal, untyped drawing method.
    void Draw( OperationType topology, bool isIndexed, uint16_t const* indices, size_t indexCount, size_t vertexCount, void** ppMappedVertices )
    {
      std::size_t size = ( m_count + vertexCount ) * sizeof(VertexType);
      if ( size > m_vertexBufferSize )
        Flush();

      *ppMappedVertices = m_scratchBuffer + m_count * sizeof(VertexType);
      m_count += vertexCount;
    }

};

}

#endif // NEBULAE_PRIMITIVEBATCH_H__