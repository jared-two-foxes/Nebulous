#ifndef NEBULAE_ENTITY_SYSTEMS_BOUNDINGBOX_H_
#define NEBULAE_ENTITY_SYSTEMS_BOUNDINGBOX_H_

#include <Nebulae/Common/Common.h>

namespace Sample
{

class BoundingBox
///
/// Describes a simple box shape centered around the origin.
///
{
private:
  Vector4 m_halfExtents;

public:
  BoundingBox( const Vector4& halfExtents );

  /*inline*/ const Vector4& GetHalfExtents() const;

  void SetHalfExtents( const Vector4& halfExtents );
};

} // namespace Sample

#endif // NEBULAE_ENTITY_SYSTEMS_BOUNDINGBOX_H_