#ifndef NEBULAE_BETA_CAMERA_CAMERA_H_
#define NEBULAE_BETA_CAMERA_CAMERA_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Camera
{
private:
  Vector4 m_position;
  Vector4 m_lookAt;
  Vector4 m_up;
  Matrix4 m_view;
  Matrix4 m_projection;

public:
  Camera();

  const Vector4& GetPosition() const;
  const Vector4& GetLookAt() const;
  const Vector4& GetUp() const;
  const Matrix4& GetViewMatrix() const;
  const Matrix4& GetProjectionMatrix() const;

  void SetPosition( const Vector4& pos );
  void SetLookAt( const Vector4& at );
  void LookAt( const Vector4& vPos, const Vector4& vAt, const Vector4& vUp );
  void SetPerspective( const Real fFovY, const Real fAspectRatio, const Real fNearZ, const Real fFarZ );
  void SetOrtho( const Real width, const Real height, const Real near_, const Real far_ );

}; // Camera

} // namespace Nebulae

#endif // NEBULAE_BETA_CAMERA_CAMERA_H_