#ifndef GEOMETRY_H__
#define GEOMETRY_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/RenderSystem/OperationType.h>

namespace Nebulae
{

class HardwareBuffer;
class VertexDeceleration;

class Geometry
{
public:
  HardwareBuffer*     m_vertexBuffer;
  VertexDeceleration* m_vertexDeceleration;
  uint32              m_vertexCount;
  HardwareBuffer*     m_indexBuffer;
  uint32              m_indexCount;
  OperationType       m_primitiveTopology;

  Geometry()
  : m_vertexBuffer( nullptr ), 
    m_vertexDeceleration( nullptr ), 
    m_vertexCount( 0 ), 
    m_indexBuffer( nullptr ),
    m_indexCount( 0 ), 
    m_primitiveTopology( OT_TRIANGLELIST )
  {}

}; //Geometry

} //Nebulae

#endif // GEOMETRY_H__