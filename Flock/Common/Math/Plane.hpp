#ifndef MATH_PLANE_H__
#define MATH_PLANE_H__

struct plane_t
{
public:
  vector4_t p; // (x,yz) represent a normal, & w-component represents the distance along normal to origin.

};

float Distance( const plane_t& plane, const vector4_t& pt );

#endif // MATH_PLANE_H__