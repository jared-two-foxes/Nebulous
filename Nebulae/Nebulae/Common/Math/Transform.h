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

#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__


/**@brief The btTransform class supports rigid transforms with only translation and rotation and no scaling/shear.
 *It can be used in combination with btVector4, btQuaternion and btMatrix3x3 linear algebra classes. 
 */
class Transform 
{
protected:
  ///Storage for the rotation
	Matrix3 m_basis;
  ///Storage for the translation
	Vector4 m_origin;

public:
	
  /**@brief No initialization constructor */
	Transform()
  {}

  /**@brief Constructor from Quaternion
   * @param q Rotation from quaternion 
   * @param c Translation from Vector */
	explicit Transform( const Quaternion& q, const Vector4& c ) 
		: m_basis( q )
    , m_origin( c )
	{
  }

  /**@brief Constructor from btMatrix3x3
   * @param b Rotation from Matrix 
   * @param c Translation from Vector*/
	explicit Transform( const Matrix3& b, const Vector4& c )
		: m_basis( b )
		, m_origin( c )
	{
  }

  /**@brief Copy constructor */
	Transform( const Transform& other )
		: m_basis( other.m_basis )
		, m_origin( other.m_origin )
	{
	}

  /**@brief Assignment Operator */
	Transform& operator=( const Transform& other )
	{
		m_basis = other.m_basis;
		m_origin = other.m_origin;
		return *this;
	}

  /**@brief Set the current transform as the value of the product of two transforms
   * @param t1 Transform 1
   * @param t2 Transform 2
   * This = Transform1 * Transform2 */
	void mult( const Transform& t1, const Transform& t2 )
	{
		m_basis = t1.m_basis * t2.m_basis;
		m_origin = t1( t2.m_origin );
	}


  //void multInverseLeft( const Transform& t1, const Transform& t2 ) 
  //{
	//	Vector4 v = t2.m_origin - t1.m_origin;
	//	m_basis = btMultTransposeLeft( t1.m_basis, t2.m_basis );
	//	m_origin = v * t1.m_basis;
	//}

	/**@brief Return the transform of the vector */
	Vector4 operator()( const Vector4& x ) const
	{
		return Vector4( m_basis[0].dot(x) + m_origin.x, 
			m_basis[1].dot(x) + m_origin.y, 
			m_basis[2].dot(x) + m_origin.z );
	}

  /**@brief Return the transform of the vector */
	Vector4 operator*( const Vector4& x ) const
	{
		return (*this)(x);
	}

  /**@brief Return the transform of the btQuaternion */
	//Quaternion operator*( const Quaternion& q ) const
	//{
	//	return getRotation() * q;
	//}

  /**@brief Return the basis matrix for the rotation */
	Matrix3&       getBasis()          { return m_basis; }
  /**@brief Return the basis matrix for the rotation */
	const Matrix3& getBasis()    const { return m_basis; }

  /**@brief Return the origin vector translation */
	Vector4&         getOrigin()         { return m_origin; }
  /**@brief Return the origin vector translation */
	const Vector4&   getOrigin()   const { return m_origin; }

  /**@brief Return a quaternion representing the rotation */
	//Quaternion getRotation() const { 
	//	Quaternion q;
	//	m_basis.getRotation( q );
	//	return q;
	//}
	
	
  /**@brief Set from an array 
   * @param m A pointer to a 15 element array (12 rotation(row major padded on the right by 1), and 3 translation */
	void setFromOpenGLMatrix( const Real *m )
	{
		//m_basis.setFromOpenGLSubMatrix( m );
		//m_origin.setValue( m[12], m[13], m[14] );
	}

  /**@brief Fill an array representation
   * @param m A pointer to a 15 element array (12 rotation(row major padded on the right by 1), and 3 translation */
	void getOpenGLMatrix( Real *m ) const 
	{
		m_basis.getOpenGLSubMatrix( m );
		m[12] = m_origin.x;
		m[13] = m_origin.y;
		m[14] = m_origin.z;
		m[15] = Real( 1. );
	}

  /**@brief Set the translational element
   * @param origin The vector to set the translation to */
	void setOrigin( const Vector4& origin ) 
	{ 
		m_origin = origin;
	}

	Vector4 invXform(const Vector4& inVec) const;


  /**@brief Set the rotational element by btMatrix3x3 */
	void setBasis(const Matrix3& basis)
	{ 
		m_basis = basis;
	}

  /**@brief Set the rotational element by btQuaternion */
	void setRotation( const Quaternion& q )
	{
		m_basis.setRotation( q );
	}


  /**@brief Set this transformation to the identity */
	void setIdentity()
	{
		m_basis.setIdentity();
		m_origin.set( 0.0f, 0.0f, 0.f );
	}

  /**@brief Multiply this Transform by another(this = this * another) 
   * @param t The other transform */
	Transform& operator*=( const Transform& t ) 
	{
		m_origin += m_basis * t.m_origin;
		m_basis *= t.m_basis;
		return *this;
	}

  /**@brief Return the inverse of this transform */
	Transform inverse() const
	{ 
		Matrix3 inv = m_basis;
		inv.transpose();
		return Transform( inv, inv * -m_origin );
	}

  /**@brief Return the inverse of this transform times the other transform
   * @param t The other transform 
   * return this.inverse() * the other */
	Transform inverseTimes(const Transform& t) const;  

  /**@brief Return the product of this transform and the other */
	Transform operator*(const Transform& t) const;

  /**@brief Return an identity transform */
	static const Transform&	getIdentity()
	{
		static const Transform identityTransform( Quaternion( 0, 0, 0, 1 ), Vector4( 0, 0, 0, 0 ) ); 
		return identityTransform;
	}
};


inline Vector4 Transform::invXform(const Vector4& inVec) const
{
	Vector4 v = inVec - m_origin;
	Matrix3 m = m_basis;
	m.transpose();
	return (m * v);
}

inline Transform Transform::inverseTimes(const Transform& t) const  
{
	Vector4 v = t.getOrigin() - m_origin;
	Matrix3 m = m_basis;
	m.transpose();
	return Transform(m * t.m_basis,
		v * m_basis);
}

inline Transform Transform::operator*( const Transform& t ) const
{
	return Transform( m_basis * t.m_basis, 
		(*this)(t.m_origin) );
}

/**@brief Test if two transforms have all elements equal */
inline bool operator==( const Transform& t1, const Transform& t2 )
{
   return ( t1.getBasis() == t2.getBasis() &&
            t1.getOrigin() == t2.getOrigin() );
}

#endif //BT_TRANSFORM_H






