#ifndef __DEBUGUTIL_H__
#define __DEBUGUTIL_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Camera;
class HardwareBuffer;
class InputLayout;
class Material;
class RenderSystem;

class DebugUtil
///
/// A utility class that assists in drawing debug lines.
///
{
public:
  typedef std::shared_ptr<RenderSystem >    RenderSystemPtr;
  typedef std::shared_ptr<Nebulae::Camera > CameraPtr;

private:
  uint32          m_primitiveCount;
  RenderSystemPtr m_renderSystem;
  Material*       m_material;
  HardwareBuffer* m_vertexBuffer;
  InputLayout*    m_inputLayout;

  public:
    DebugUtil( RenderSystemPtr renderer );

    void Init();

    void Reset();

    void AddLine( const Vector4& p1, const Vector4& p2, const Vector4& colour );
    void AddCircle( const Vector4& position, const Real radius, const Vector4& colour );

    void Draw( const CameraPtr& camera );
};

}

#endif // __DEBUGUTIL_H__