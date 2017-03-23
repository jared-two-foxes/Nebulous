
#include "BoundingBox.h"

namespace Sample
{
  
BoundingBox::BoundingBox( const Vector4& halfExtents )
  : m_halfExtents( halfExtents )
{

}

const Vector4& 
BoundingBox::GetHalfExtents() const
{
  return m_halfExtents;
}

void
BoundingBox::SetHalfExtents( const Vector4& halfExtents )
{
  m_halfExtents = halfExtents;
}

}