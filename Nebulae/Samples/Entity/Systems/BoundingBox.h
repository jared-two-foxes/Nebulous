#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

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

}

#endif // __BOUNDINGBOX_H__