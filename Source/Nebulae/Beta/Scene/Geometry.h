#ifndef NEBULAE_BETA_SCENE_GEOMETRY_H_
#define NEBULAE_BETA_SCENE_GEOMETRY_H_

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/RenderSystem/OperationType.h>

namespace Nebulae
{

class HardwareBuffer;
class VertexDeceleration;

class Geometry
{
public:
  HardwareBuffer* m_vertexBuffer;
  VertexDeceleration* m_vertexDeceleration;
  uint32 m_vertexCount;
  HardwareBuffer* m_indexBuffer;
  uint32 m_indexCount;
  OperationType m_primitiveTopology;

  Geometry()
    : m_vertexBuffer( nullptr ),
      m_vertexDeceleration( nullptr ),
      m_vertexCount( 0 ),
      m_indexBuffer( nullptr ),
      m_indexCount( 0 ),
      m_primitiveTopology( OT_TRIANGLELIST )
  {
  }

}; // Geometry

} // namespace Nebulae

#endif // NEBULAE_BETA_SCENE_GEOMETRY_H_