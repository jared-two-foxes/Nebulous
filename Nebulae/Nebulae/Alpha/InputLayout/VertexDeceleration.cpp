#include "VertexDeceleration.h"

using namespace Nebulae;

bool Nebulae::operator ==( const VertexDeceleration& lhs, const VertexDeceleration& rhs )
{
  if( lhs.GetElementCount() != rhs.GetElementCount() ) return false;
  
  for( int32 i = 0; i < lhs.GetElementCount(); ++i )
  {
    if( lhs.GetVertexElement(i)->AlignedByteOffset != rhs.GetVertexElement(i)->AlignedByteOffset || 
        lhs.GetVertexElement(i)->Format            != rhs.GetVertexElement(i)->Format || 
        lhs.GetVertexElement(i)->InputSlot         != rhs.GetVertexElement(i)->InputSlot ||
        lhs.GetVertexElement(i)->Semantic          != rhs.GetVertexElement(i)->Semantic ||
        lhs.GetVertexElement(i)->SemanticIndex     != rhs.GetVertexElement(i)->SemanticIndex )
    { 
      return false; 
    }  
  }

  return true;
}

//constructor
VertexDeceleration::VertexDeceleration( int elementCount )
	: m_iElementCount( elementCount )
	, m_iInternalCount( 0 )
{
	NE_ASSERT( elementCount > 0, "Element count must be greater than zero" )( elementCount );

	m_Elements = new VertexElement[ elementCount ];
}

//destructor
VertexDeceleration::~VertexDeceleration()
{
	delete [] m_Elements;
}

//SetElement
//@todo Should do a _DEBUG check that iSemanticIndex is correct (ie not repeated and consistent).
const VertexElement& VertexDeceleration::AddElement( VertexElementType format, VertexElementSemantic semantic, int semanticIndex, unsigned short index )
{	
  std::size_t iAlignedByteOffset = GetVertexSize();
  int32  idx                = m_iInternalCount++;

  NE_ASSERT( idx <= m_iElementCount, "idx is out of bounds" )( idx, m_iElementCount );

  //m_Elements[idx].iInputSlot = 0;
  m_Elements[idx].AlignedByteOffset = iAlignedByteOffset;
  m_Elements[idx].Format = format;
	m_Elements[idx].Semantic = semantic;
	m_Elements[idx].SemanticIndex = semanticIndex;

  return m_Elements[idx];
}

VertexElement* VertexDeceleration::GetVertexElement( uint32 idx ) const
{
  NE_ASSERT( idx <= m_iInternalCount, "idx is out of bounds" )( idx, m_iInternalCount );

	return &m_Elements[ idx ];
}

VertexDeceleration* 
VertexDeceleration::Clone() const
{
  VertexDeceleration* out = new VertexDeceleration( m_iElementCount );
  std::copy( m_Elements, m_Elements + m_iInternalCount, out->m_Elements ); // Copy the elements.
  out->m_iInternalCount = m_iInternalCount; // Store the internal count.
  return out; // return
}

size_t GetTypeSize(VertexElementType etype )
{
  switch(etype)
  {
  case VET_FLOAT1:
    return sizeof(float);
  case VET_FLOAT2:
    return sizeof(float)*2;
  case VET_FLOAT3:
    return sizeof(float)*3;
  case VET_FLOAT4:
    return sizeof(float)*4;
  case VET_SHORT1:
    return sizeof(short);
  case VET_SHORT2:
    return sizeof(short)*2;
  case VET_SHORT3:
    return sizeof(short)*3;
  case VET_SHORT4:
    return sizeof(short)*4;
  case VET_UBYTE4:
    return sizeof(unsigned char)*4;
  }
  return 0;
}

//GetVertexSize
std::size_t 
VertexDeceleration::GetVertexSize() const
{
	std::size_t size = 0;
	for( std::size_t i = 0; i < m_iInternalCount; ++i )
	{
		size += GetTypeSize( m_Elements[i].Format );
	}
	return size;
}