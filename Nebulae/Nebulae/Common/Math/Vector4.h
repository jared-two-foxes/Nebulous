/**
 * Vector4.h
 *
 * AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
 * CREATED: 2007-02-14
 * UPDATED: 2011-12-07
 *
 * Copyright (C) 2011 Song Ho Ahn
 */

#ifndef __VECTOR4_H__
#define __VECTOR4_H__

/** Vector4.
 */
class Vector4
{
public:

  // ctors
  Vector4() {};
  Vector4( Real x, Real y, Real z, Real w = 0.0f );
  Vector4( const Vector4& v );


  // utils functions
  Real*       ptr();
  const Real* ptr() const;
  void        set( Real x, Real y, Real z, Real w = 0.0f );
  Real        length() const;                              //
  Real        distance( const Vector4& vec ) const;        // distance between two vectors
  Vector4&    normalize();                             //
  Real        dot(const Vector4& vec) const;               // dot product
  Vector4     cross( const Vector4& vec ) const;        // cross product
  bool        equal(const Vector4& vec, float e) const;    // compare with epsilon
  Vector4&    min( const Vector4& v );
  Vector4&    max( const Vector4& v );

  Vector4 operator-() const;                        // unary operator (negate)
  
  Vector4 operator+( const Vector4& rhs ) const;    // add rhs
  Vector4 operator-( const Vector4& rhs ) const;    // subtract rhs
  Vector4 operator*( const Real scale ) const;      // scale
  Vector4 operator/( const Real scale ) const;      // inverse scale

  Vector4& operator+=( const Vector4& rhs );        // add rhs and update this object
  Vector4& operator-=( const Vector4& rhs );        // subtract rhs and update this object
  Vector4& operator*=( const Real scale );          // scale and update this object
  Vector4& operator/=( const Real scale );          // scale and update this object
  
  Real     operator[]( int index ) const;           // subscript operator v[0], v[1]
  Real&    operator[]( int index );                 // subscript operator v[0], v[1]

	bool operator==( const Vector4& other ) const
	{
		return ( (w==other.w) && (z==other.z) && (y==other.y) && (x==other.x) );
	}

	bool operator!=( const Vector4& other ) const
	{
		return !(*this == other);
	}

  Real x, y, z, w;
};


// Assert template specialization for vector type.
template <>
inline Assert& Assert::Variable(const char* const name, const Vector4& value)
{
  printf( "  o Variable %s = <%f,%f,%f,%f> (vector4)\n", name, value.x, value.y, value.w, value.z );
  return *this;
}

// Standard implementation
inline Vector4::Vector4( Real _x, Real _y, Real _z, Real _w )
  : x( _x ), 
    y( _y ), 
    z( _z ), 
    w( _w )
{}

inline Vector4::Vector4( const Vector4& v )
  : x( v.x ), 
    y( v.y ), 
    z( v.z ), 
    w( v.w )
{}

inline Vector4 
Vector4::operator-() const 
{ return Vector4(-x, -y, -z, -w); }

inline Vector4 
Vector4::operator+(const Vector4& rhs) const 
{ return Vector4(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w); }

inline Vector4 
Vector4::operator-(const Vector4& rhs) const 
{ return Vector4(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w); }

inline Vector4& 
Vector4::operator+=(const Vector4& rhs) 
{
  x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; 
  return *this;
}

inline Vector4& 
Vector4::operator-=(const Vector4& rhs) 
{
  x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; 
  return *this;
}

inline Vector4 
Vector4::operator*(const float a) const 
{ return Vector4(x*a, y*a, z*a, w*a); }

inline Vector4& 
Vector4::operator*=(const float a) 
{
  x *= a; y *= a; z *= a; w *= a; 
  return *this;
}

inline Vector4 
Vector4::operator/(const float a) const 
{ return Vector4(x/a, y/a, z/a, w/a); }

inline Vector4& 
Vector4::operator/=(const float a) 
{ 
  x /= a; y /= a; z /= a; w /= a; 
  return *this; 
}

inline Real  
Vector4::operator[]( int index ) const
{ return (&x)[index]; }

inline Real& 
Vector4::operator[]( int index )
{ return (&x)[index]; }

inline Real*       
Vector4::ptr()
{ return &x; }

inline const Real* 
Vector4::ptr() const
{ return &x; }

inline void 
Vector4::set( float x_, float y_, float z_, float w_ ) 
{ x = x_; y = y_; z = z_; w = w_; }

inline Real
Vector4::length() const 
{ return sqrtf(x*x + y*y + z*z + w*w); }

inline Real 
Vector4::distance(const Vector4& vec) const 
{ return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y) + (vec.z-z)*(vec.z-z) + (vec.w-w)*(vec.w-w)); }

inline Vector4& 
Vector4::normalize() 
// Leaves the w-component untouched
{
  const Real EPSILON = 0.000001f;
  Real xxyyzz = x*x + y*y + z*z;
  if( xxyyzz < EPSILON ) {
    return *this; // do nothing if it is zero vector
  }
  
  Real invLength = 1.0f / sqrtf(xxyyzz);
  x *= invLength;
  y *= invLength;
  z *= invLength;

  return *this;
}

inline Real 
Vector4::dot(const Vector4& rhs) const 
{ return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w); }

inline Vector4 
Vector4::cross( const Vector4& rhs ) const 
{
	return Vector4(
	  y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x);
}

inline bool 
Vector4::equal(const Vector4& rhs, float epsilon) const 
{
  return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon &&
         fabs(z - rhs.z) < epsilon && fabs(w - rhs.w) < epsilon;
}

inline Vector4& 
Vector4::max( const Vector4& v ) 
{
  x = std::max(x, v.x);
  y = std::max(y, v.y);
  z = std::max(z, v.z);
  w = std::max(w, v.w);
  return *this;
}

inline Vector4& 
Vector4::min( const Vector4& v ) 
{
  x = std::min(x, v.x);
  y = std::min(y, v.y);
  z = std::min(z, v.z);
  w = std::min(w, v.w);
  return *this;
}

inline Vector4 
operator*( const float a, const Vector4 vec ) 
{
    return Vector4(a*vec.x, a*vec.y, a*vec.z, a*vec.w);
}

#endif
