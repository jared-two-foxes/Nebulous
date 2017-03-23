/*
Copyright (c) 2003-2006 Gino van den Bergen / Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#define SIMD_EPSILON 0.0001f

/**@brief The Quaternion implements quaternion to perform linear algebra rotations in combination with Matrix3, Vector4 and Transform. 
 */
class Quaternion
{
public:
  /**@brief No initialization constructor 
   */
	Quaternion();

	// Copy constructor
	Quaternion( const Quaternion& rhs );

	// Assignment Operator
	Quaternion& operator=( const Quaternion& v );


  /**@brief Constructor from scalars 
   */
	Quaternion( const Real& _x, const Real& _y, const Real& _z, const Real& _w );

  /**@brief Axis angle Constructor
   * @param axis The axis which the rotation is around
   * @param angle The magnitude of the rotation around the angle (Radians) 
   */
	Quaternion( const Vector4& _axis, const Real& _angle );
  
	/**@brief Constructor from Euler angles
   * @param yaw Angle around Y unless BT_EULER_DEFAULT_ZYX defined then Z
   * @param pitch Angle around X unless BT_EULER_DEFAULT_ZYX defined then Y
   * @param roll Angle around Z unless BT_EULER_DEFAULT_ZYX defined then X 
   */
	Quaternion( const Real& yaw, const Real& pitch, const Real& roll );

  void setValue( Real _x, Real _y, Real _z, Real _w );

  /**@brief Set the rotation using axis angle notation 
   * @param axis The axis around which to rotate
   * @param angle The magnitude of the rotation in Radians 
   */
	void setRotation( const Vector4& axis, const Real& _angle );

  /**@brief Set the quaternion using Euler angles
   * @param yaw Angle around Y
   * @param pitch Angle around X
   * @param roll Angle around Z 
   */
	void setEuler( const Real& yaw, const Real& pitch, const Real& roll );

  /**@brief Set the quaternion using euler angles 
   * @param yaw Angle around Z
   * @param pitch Angle around Y
   * @param roll Angle around X 
   */
	void setEulerZYX( const Real& yaw, const Real& pitch, const Real& roll );

  /**@brief Add two quaternions
   * @param q The quaternion to add to this one 
   */
	Quaternion& operator+=( const Quaternion& q );

  /**@brief Subtract out a quaternion
   * @param q The quaternion to subtract from this one 
   */
	Quaternion& operator-=( const Quaternion& q );

  /**@brief Scale this quaternion
   * @param s The scalar to scale by 
   */
	Quaternion& operator*=( const Real& s );

  /**@brief Multiply this quaternion by q on the right
   * @param q The other quaternion 
   * Equivilant to this = this * q 
   */
	Quaternion& operator*=( const Quaternion& q );

  /**@brief Return the dot product between this quaternion and another
   * @param q The other quaternion 
   */
	Real dot( const Quaternion& q ) const;

  /**@brief Return the length squared of the quaternion 
   */
	Real length2() const;

  /**@brief Return the length of the quaternion 
   */
	Real length() const;

  /**@brief Normalize the quaternion 
   * Such that x^2 + y^2 + z^2 +w^2 = 1 
   */
	Quaternion& normalize();

  /**@brief Return a scaled version of this quaternion
   * @param s The scale factor 
   */
	Quaternion operator*( const Real& s ) const;

  /**@brief Return an inversely scaled version of this quaternion
   * @param s The inverse scale factor 
   */
	Quaternion operator/(const Real& s) const;

  /**@brief Inversely scale this quaternion
   * @param s The scale factor 
   */
	Quaternion& operator/=( const Real& s );

  /**@brief Return a normalized version of this quaternion 
   */
	Quaternion normalized() const;

  /**@brief Return the angle between this quaternion and the other 
   * @param q The other quaternion 
   */
	Real angle( const Quaternion& q ) const;

  /**@brief Return the angle of rotation represented by this quaternion 
   */
	Real getAngle() const;

	/**@brief Return the axis of the rotation represented by this quaternion 
   */
	Vector4 getAxis() const;

	/**@brief Return the inverse of this quaternion 
   */
	Quaternion inverse() const;

  /**@brief Return the sum of this quaternion and the other 
   * @param q2 The other quaternion 
   */
	Quaternion operator+( const Quaternion& q2 ) const;

  /**@brief Return the difference between this quaternion and the other 
   * @param q2 The other quaternion 
   */
	Quaternion operator-( const Quaternion& q2 ) const;

  /**@brief Return the negative of this quaternion 
   * This simply negates each element 
   */
	Quaternion operator-() const;

  /**@brief Return the quaternion which is the result of Spherical Linear Interpolation between this and the other quaternion
   * @param q The other quaternion to interpolate with 
   * @param t The ratio between this and q to interpolate.  If t = 0 the result is this, if t=1 the result is q.
   * Slerp interpolates assuming constant velocity.  
   */
	Quaternion slerp( const Quaternion& q, const Real& t ) const;

	static const Quaternion&	getIdentity();

  Vector4 m_vec;
};


// Assert template specialization for vector type.
template <>
inline Assert& Assert::Variable( const char* const name, const Quaternion& value )
{
  printf( "  o Variable %s = <%f,%f,%f,%f> (quaternion)\n", name, value.m_vec.x, value.m_vec.y, value.m_vec.w, value.m_vec.z );
  return *this;
}


// Standard implementation
inline Quaternion::Quaternion() {}

inline Quaternion::Quaternion( const Quaternion& rhs )
{
	m_vec = rhs.m_vec;
}

inline Quaternion& Quaternion::operator=( const Quaternion& v ) 
{
	m_vec = v.m_vec;
	return *this;
}

inline Quaternion::Quaternion( const Real& _x, const Real& _y, const Real& _z, const Real& _w ) 
  : m_vec( _x, _y, _z, _w )
{
}

inline Quaternion::Quaternion( const Vector4& _axis, const Real& _angle ) 
{ 
	setRotation( _axis, _angle ); 
}
  
inline Quaternion::Quaternion( const Real& yaw, const Real& pitch, const Real& roll )
{ 
	setEuler( yaw, pitch, roll ); 
}

inline void Quaternion::setValue( Real _x, Real _y, Real _z, Real _w )
{
  m_vec.set( _x, _y, _z, _w );
}

inline void Quaternion::setRotation( const Vector4& axis, const Real& _angle )
{
	Real d = axis.length();
	NE_ASSERT( d != Real(0.), "Length of axis cannot be zero" )( axis, d );
	Real s = sin( _angle * Real( 0.5 )) / d;
	setValue( axis.x * s, axis.y * s, axis.z * s, 
		cos( _angle * Real( 0.5 ) ) );
}

inline void Quaternion::setEuler( const Real& yaw, const Real& pitch, const Real& roll )
{
	Real halfYaw = Real( yaw ) * Real( 0.5 );  
	Real halfPitch = Real( pitch ) * Real( 0.5 );  
	Real halfRoll = Real( roll ) * Real( 0.5 );  
	Real cosYaw = cos( halfYaw );
	Real sinYaw = sin( halfYaw );
	Real cosPitch = cos( halfPitch );
	Real sinPitch = sin( halfPitch );
	Real cosRoll = cos( halfRoll );
	Real sinRoll = sin( halfRoll );

	setValue(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
		cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
		sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
		cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw);
}

inline void Quaternion::setEulerZYX( const Real& yaw, const Real& pitch, const Real& roll )
{
	Real halfYaw = Real( yaw ) * Real( 0.5 );  
	Real halfPitch = Real( pitch ) * Real( 0.5 );  
	Real halfRoll = Real( roll ) * Real( 0.5 );  
	Real cosYaw = cos( halfYaw );
	Real sinYaw = sin( halfYaw );
	Real cosPitch = cos( halfPitch );
	Real sinPitch = sin( halfPitch );
	Real cosRoll = cos( halfRoll );
	Real sinRoll = sin( halfRoll );
		
  setValue(sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw, //x
    cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw, //y
    cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw, //z
    cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw); //formerly yzx
}

inline Quaternion& Quaternion::operator+=( const Quaternion& q )
{
	m_vec.x += q.m_vec.x; 
  m_vec.y += q.m_vec.y; 
  m_vec.z += q.m_vec.z; 
  m_vec.w += q.m_vec.w;

	return *this;
}

inline Quaternion& Quaternion::operator-=( const Quaternion& q ) 
{
	m_vec.x -= q.m_vec.x; 
  m_vec.y -= q.m_vec.y; 
  m_vec.z -= q.m_vec.z; 
  m_vec.w -= q.m_vec.w;

  return *this;
}

inline Quaternion& Quaternion::operator*=( const Real& s )
{
	m_vec.x *= s; 
  m_vec.y *= s; 
  m_vec.z *= s; 
  m_vec.w *= s;

  return *this;
}

inline Quaternion& Quaternion::operator*=( const Quaternion& q )
{
	setValue(
    m_vec.w * q.m_vec.x + m_vec.x * q.m_vec.w + m_vec.y * q.m_vec.z - m_vec.z * q.m_vec.y,
		m_vec.w * q.m_vec.y + m_vec.y * q.m_vec.w + m_vec.z * q.m_vec.x - m_vec.x * q.m_vec.z,
		m_vec.w * q.m_vec.z + m_vec.z * q.m_vec.w + m_vec.x * q.m_vec.y - m_vec.y * q.m_vec.x,
		m_vec.w * q.m_vec.w - m_vec.x * q.m_vec.x - m_vec.y * q.m_vec.y - m_vec.z * q.m_vec.z);

	return *this;
}

inline Real Quaternion::dot( const Quaternion& q ) const
{
	return m_vec.x * q.m_vec.x + 
    m_vec.y * q.m_vec.y + 
    m_vec.z * q.m_vec.z + 
    m_vec.w * q.m_vec.w;
}

inline Real Quaternion::length2() const
{
	return dot(*this);
}

inline Real Quaternion::length() const
{
	return sqrt(length2());
}

inline Quaternion& Quaternion::normalize() 
{
	return *this /= length();
}

inline Quaternion Quaternion::operator*( const Real& s ) const
{
	return Quaternion( m_vec.x * s, m_vec.y * s, m_vec.z * s,  m_vec.w * s );
}

inline Quaternion Quaternion::operator/(const Real& s) const
{
	NE_ASSERT( s != Real(0.), "Cannot divide by zero, undefined behaviour" )(s);
	return *this * (Real(1.0) / s);
}

inline Quaternion& Quaternion::operator/=( const Real& s ) 
{
	NE_ASSERT( s != Real(0.), "Cannot divide by zero, undefined behaviour" )(s);
	return *this *= Real(1.0) / s;
}

inline Quaternion Quaternion::normalized() const 
{
	return *this / length();
} 

inline Real Quaternion::angle( const Quaternion& q ) const 
{
	Real s = sqrt( length2() * q.length2() );
	NE_ASSERT( s != Real(0.), "Cannot divide by zero, undefined behaviour" )( q, s );
	return acos( dot( q ) / s );
}

inline Real Quaternion::getAngle() const 
{
	Real s = Real(2.) * acos( m_vec.w );
	return s;
}

inline Vector4 Quaternion::getAxis() const
{
	Real s_squared = 1.f-m_vec.w*m_vec.w;
		
	if (s_squared < Real(10.0) * SIMD_EPSILON) //Check for divide by zero
  {
		return Vector4(1.0, 0.0, 0.0);  // Arbitrary
  }
  Real s = 1.f/sqrt( s_squared );
	return Vector4( m_vec.x * s, m_vec.y * s, m_vec.z * s );
}

inline Quaternion Quaternion::inverse() const
{
	return Quaternion( -m_vec.x, -m_vec.y, -m_vec.z, m_vec.w );
}

inline Quaternion Quaternion::operator+( const Quaternion& q2 ) const
{
	const Quaternion& q1 = *this;
	return Quaternion( q1.m_vec.x + q2.m_vec.x, q1.m_vec.y + q2.m_vec.y, q1.m_vec.z + q2.m_vec.z, q1.m_vec.w + q2.m_vec.w );
}

inline Quaternion Quaternion::operator-( const Quaternion& q2 ) const
{
	const Quaternion& q1 = *this;
	return Quaternion( q1.m_vec.x - q2.m_vec.x, q1.m_vec.y - q2.m_vec.y, q1.m_vec.z - q2.m_vec.z, q1.m_vec.w - q2.m_vec.w );
}

inline Quaternion Quaternion::operator-() const
{
	const Quaternion& q2 = *this;
	return Quaternion( - q2.m_vec.x, - q2.m_vec.y,  - q2.m_vec.z,  - q2.m_vec.w );
}

inline Quaternion Quaternion::slerp( const Quaternion& q, const Real& t ) const
{
	Real magnitude = sqrt( length2() * q.length2() ); 
	NE_ASSERT( magnitude != Real(0.), "Cannot divide by zero, undefined behaviour" )( q, magnitude );

  Real product = dot(q) / magnitude;
  if ( fabs(product) != Real(1) )
	{
    // Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
    const Real sign = ( product < 0 ) ? Real(-1) : Real(1);

    const Real theta = acos( sign * product );
    const Real s1 = sin( sign * t * theta );   
    const Real d = Real( 1.0 ) / sin( theta );
    const Real s0 = sin( ( Real( 1.0 ) - t ) * theta );

    return Quaternion(
        ( m_vec.x * s0 + q.m_vec.x * s1 ) * d,
        ( m_vec.y * s0 + q.m_vec.y * s1 ) * d,
        ( m_vec.z * s0 + q.m_vec.z * s1 ) * d,
        ( m_vec.z * s0 + q.m_vec.w * s1 ) * d
      );
	}
	else
	{
		return *this;
	}
}

inline const Quaternion& Quaternion::getIdentity()
{
	static const Quaternion identityQuat( Real( 0.0 ), Real( 0.0 ), Real( 0.0 ), Real( 1.0 ) );
	return identityQuat;
}

#endif // __QUATERNION_H__



