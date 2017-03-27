/**
 * Vector2.h
 *
 * 2D/3D/4D vectors
 *
 *  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
 * CREATED: 2007-02-14
 * UPDATED: 2011-12-07
 *
 * Copyright (C) 2011 Song Ho Ahn
 */
#ifndef __VECTOR2_H__
#define __VECTOR2_H__


/**
 *  Vector2
 */
class Vector2
{
public:
  float x;
  float y;

  // ctors
  Vector2() : x(0), y(0) {};
  Vector2(float x, float y) : x(x), y(y) {};

  // utils functions
  void        set(float x, float y);
  float       length() const;                           //
  float       distance(const Vector2& vec) const;       // distance between two vectors
  Vector2&    normalize();                              //
  float       dot(const Vector2& vec) const;            // dot product
  bool        equal(const Vector2& vec, float e) const; // compare with epsilon

  // operators
  Vector2     operator-() const;                        // unary operator (negate)
  Vector2     operator+( const Vector2& rhs ) const;    // add rhs
  Vector2     operator-( const Vector2& rhs ) const;    // subtract rhs
  Vector2&    operator+=( const Vector2& rhs );         // add rhs and update this object
  Vector2&    operator-=( const Vector2& rhs );         // subtract rhs and update this object
  Vector2     operator*( const float scale ) const;     // scale
  Vector2     operator*( const Vector2& rhs ) const;    // multiply each element
  Vector2&    operator*=( const float scale );          // scale and update this object
  Vector2&    operator*=( const Vector2& rhs );         // multiply each element and update this object
  Vector2     operator/( const float scale ) const;     // inverse scale
  Vector2&    operator/=( const float scale );          // scale and update this object
  bool        operator==( const Vector2& rhs ) const;   // exact compare, no epsilon
  bool        operator!=( const Vector2& rhs ) const;   // exact compare, no epsilon
  bool        operator<( const Vector2& rhs ) const;    // comparison for sort
  float       operator[]( int index ) const;            // subscript operator v[0], v[1]
  float&      operator[]( int index );                  // subscript operator v[0], v[1]

  friend Vector2 operator*(const float a, const Vector2 vec);

};

// Assert template specialization for vector type.
template <>
inline Assert& Assert::Variable(const char* const name, const Vector2& value)
{
  printf( "  o Variable %s = <%f,%f> (Vector2)\n", name, value.x, value.y );
  return *this;
}

// Standard implementation
inline Vector2 Vector2::operator-() const 
{
  return Vector2(-x, -y);
}

inline Vector2 Vector2::operator+(const Vector2& rhs) const 
{
  return Vector2(x+rhs.x, y+rhs.y);
}

inline Vector2 Vector2::operator-(const Vector2& rhs) const 
{
  return Vector2(x-rhs.x, y-rhs.y);
}

inline Vector2& Vector2::operator+=(const Vector2& rhs) 
{
  x += rhs.x; y += rhs.y; return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& rhs) 
{
  x -= rhs.x; y -= rhs.y; return *this;
}

inline Vector2 Vector2::operator*(const float a) const 
{
  return Vector2(x*a, y*a);
}

inline Vector2 Vector2::operator*(const Vector2& rhs) const 
{
  return Vector2(x*rhs.x, y*rhs.y);
}

inline Vector2& Vector2::operator*=(const float a) 
{
  x *= a; y *= a; return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& rhs) 
{
  x *= rhs.x; y *= rhs.y; return *this;
}

inline Vector2 Vector2::operator/(const float a) const 
{
  return Vector2(x/a, y/a);
}

inline Vector2& Vector2::operator/=(const float a) 
{
  x /= a; y /= a; return *this;
}

inline bool Vector2::operator==(const Vector2& rhs) const 
{
  return (x == rhs.x) && (y == rhs.y);
}

inline bool Vector2::operator!=(const Vector2& rhs) const 
{
  return (x != rhs.x) || (y != rhs.y);
}

inline bool Vector2::operator<(const Vector2& rhs) const 
{
  if(x < rhs.x) return true;
  if(x > rhs.x) return false;
  if(y < rhs.y) return true;
  if(y > rhs.y) return false;
  return false;
}

inline float Vector2::operator[](int index) const 
{
  return (&x)[index];
}

inline float& Vector2::operator[](int index) 
{
  return (&x)[index];
}

inline void Vector2::set(float ax, float ay) 
{
  x = ax; y = ay;
}

inline float Vector2::length() const 
{
  return sqrtf(x*x + y*y);
}

inline float Vector2::distance(const Vector2& vec) const 
{
  return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y));
}

inline Vector2& Vector2::normalize() 
{
  const float EPSILON = 0.000001f;
  float xxyy = x*x + y*y;
  if(xxyy < EPSILON)
    return *this;

  //float invLength = invSqrt(xxyy);
  float invLength = 1.0f / sqrtf(xxyy);
  x *= invLength;
  y *= invLength;
  return *this;
}

inline float Vector2::dot(const Vector2& rhs) const 
{
  return (x*rhs.x + y*rhs.y);
}

inline bool Vector2::equal(const Vector2& rhs, float epsilon) const 
{
  return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

inline Vector2 operator*(const float a, const Vector2 vec) 
{
  return Vector2(a*vec.x, a*vec.y);
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) 
{
  os << "(" << vec.x << ", " << vec.y << ")";
  return os;
}

#endif
