#ifndef NEBULAE_ALPHA_VERTEXDECLERATION_H__
#define NEBULAE_ALPHA_VERTEXDECLERATION_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

/// Vertex element semantics, used to identify the meaning of vertex buffer contents
enum VertexElementSemantic 
{
  VES_POSITION,             ///< Position coordinates (local space)
  VES_BLEND_WEIGHTS,        ///< Blending weights
  VES_BLEND_INDICES,        ///< Blending indices
  VES_NORMAL,               ///< Normal, 3 reals per vertex
  VES_DIFFUSE,              ///< Diffuse colours
  VES_SPECULAR,             ///< Specular colours
  VES_TEXTURE_COORDINATES,  ///< Texture coordinates
  VES_BINORMAL,             ///< Binormal (Y axis if normal is Z)
  VES_TANGENT,              ///< Tangent (X axis if normal is Z)
};

/// Vertex element type, used to identify the base types of the vertex contents
enum VertexElementType
{
  VET_FLOAT1 = 0,
  VET_FLOAT2 = 1,
  VET_FLOAT3 = 2,
  VET_FLOAT4 = 3,
  VET_SHORT1 = 5,
  VET_SHORT2 = 6,
  VET_SHORT3 = 7,
  VET_SHORT4 = 8,
  VET_UBYTE4 = 9,
};

struct VertexElement
{
  size_t                InputSlot;         ///< The source vertex buffer, as bound to an index using VertexBufferBinding
  size_t                AlignedByteOffset; ///< The offset in the buffer that this element starts at
  VertexElementType     Format;            ///< The type of element
  VertexElementSemantic Semantic;          ///< The meaning of the element
  size_t                SemanticIndex;     ///< Index of the item, only applicable for some elements like texture coords
};

class VertexDeceleration
{
protected:
  int32          m_iElementCount;   ///< The size of the vertex deceleration array.
  uint32         m_iInternalCount;  ///< The number of elements currently in the array
  VertexElement* m_Elements;        ///< The vertex elements.

public:
  VertexDeceleration( int elementCount );
  ~VertexDeceleration();

  /** Get the number of elements in the declaration. */
  int GetElementCount() const { return m_iInternalCount; }

  /** Get a single element. */
  VertexElement* GetVertexElement( uint32 idx ) const;

  /** Creates a deep copy of this VertexDeceleration.  Ownership of the cloned object
   *  is passed to the receiver. 
   *
   *  @return
   *    Copied object.
   */
  VertexDeceleration* Clone() const;

  /** Adds a new VertexElement to this declaration.
      @remarks
        This method adds a single element (positions, normals etc) to the end of the
        vertex declaration. <b>Please read the information in VertexDeclaration about
        the importance of ordering and structure for compatibility with older D3D drivers</b>.
      @param source The binding index of HardwareVertexBuffer which will provide the source for this element. 
        See VertexBufferBinding for full information.
      @param offset The offset in bytes where this element is located in the buffer
      @param theType The data format of the element (3 floats, a colour etc)
      @param semantic The meaning of the data (position, normal, diffuse colour etc)
      @param index Optional index for multi-input elements like texture coordinates
      @return A reference to the VertexElement added.
  */
  const VertexElement& AddElement( VertexElementType format, VertexElementSemantic semantic, int semanticIndex, unsigned short index = 0 );

  /** Remove the element at the given index from this declaration. */
  void removeElement( unsigned short elem_index );

  /** Remove the element with the given semantic and usage index.
      @remarks
        In this case 'index' means the usage index for repeating elements such
        as texture coordinates. For other elements this will always be 0 and does
        not refer to the index in the vector.
  */
  void removeElement(VertexElementSemantic semantic, unsigned short index = 0);

  /** Remove all elements. */
  void removeAllElements(void);

  /** Gets the vertex size defined by this declaration for a given source. */
  std::size_t GetVertexSize() const;

}; //VertexDecleration

bool operator ==( const VertexDeceleration& lhs, const VertexDeceleration& rhs );

} // Nebulae

#endif // NEBULAE_ALPHA_VERTEXDECLERATION_H__