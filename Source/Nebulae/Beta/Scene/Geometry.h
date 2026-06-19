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
  HardwareBuffer* m_vertexBuffer{ nullptr };
  VertexDeceleration* m_vertexDeceleration{ nullptr };
  uint32 m_vertexCount{ 0 };
  HardwareBuffer* m_indexBuffer{ nullptr };
  uint32 m_indexCount{ 0 };
  OperationType m_primitiveTopology{ OT_TRIANGLELIST };
}; // Geometry

} // namespace Nebulae

#endif // NEBULAE_BETA_SCENE_GEOMETRY_H_
