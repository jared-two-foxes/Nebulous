#ifndef __CONTROLLER_CAMERACONTROLLER_H__
#define __CONTROLLER_CAMERACONTROLLER_H__

#include <Nebulae/Nebulae.h>

namespace Nebulae {
  class Camera;
}

namespace Game {
  
class CameraController
///
/// 
///
{
private:
  Nebulae::Camera* m_camera;
  float            m_verticalSpeed;

  public:
    CameraController( Nebulae::Camera& camera );

    void SetVerticalSpeed( const float speed );
    void SetPosition( const Vector4& position );
    void SetPosition( const Real depth );
    void Update( const float elapsed );

};

}

#endif //__CONTROLLER_CAMERACONTROLLER_H__