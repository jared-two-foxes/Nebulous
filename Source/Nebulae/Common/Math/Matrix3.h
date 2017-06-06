#ifndef __MATRIX3_H__
#define __MATRIX3_H__

/** @brief Basic 3x3 matrix.  Stored Row Major?
 */
class Matrix3
{
public:
  /** @brief No initializaion constructor */
	Matrix3 () {}

	/** @brief Constructor from Quaternion */
	explicit Matrix3( const Quaternion& q ) { setRotation( q ); }

  /** @brief Constructor with row major formatting */
	Matrix3(const Real& xx, const Real& xy, const Real& xz,
		const Real& yx, const Real& yy, const Real& yz,
		const Real& zx, const Real& zy, const Real& zz)
	{ 
		setValue( xx, xy, xz, 
			yx, yy, yz, 
			zx, zy, zz );
	}

  /** @brief Copy constructor */
	Matrix3( const Matrix3& other )
	{
		m_el[0] = other.m_el[0];
		m_el[1] = other.m_el[1];
		m_el[2] = other.m_el[2];
	}

	/** @brief Assignment Operator */
	Matrix3& operator=( const Matrix3& other )
	{
		m_el[0] = other.m_el[0];
		m_el[1] = other.m_el[1];
		m_el[2] = other.m_el[2];
		return *this;
	}

  /** @brief Get a column of the matrix as a vector 
	 *  @param i Column number 0 indexed 
   */
	Vector4 getColumn( uint32 i ) const
	{
		return Vector4( m_el[0][i], m_el[1][i], m_el[2][i] );
	}

	/** @brief Get a row of the matrix as a vector 
	 *  @param i Row number 0 indexed 
   */
	const Vector4& getRow( uint32 i ) const
	{
		NE_ASSERT( i < 3, "Invalid row index" )( i );
		return m_el[i];
	}

  /** @brief Get a mutable reference to a row of the matrix as a vector 
	 *  @param i Row number 0 indexed 
   */
	Vector4& operator[]( uint32 i )
	{ 
		NE_ASSERT( i < 3, "Invalid row index" )( i );
		return m_el[i]; 
	}

	/** @brief Get a const reference to a row of the matrix as a vector 
	 *  @param i Row number 0 indexed 
   */
	const Vector4& operator[]( uint32 i ) const
	{
		NE_ASSERT( i < 3, "Invalid row index" )( i );
		return m_el[i]; 
	}

  /** @brief Multiply by the target matrix on the right
	 *  @param m Rotation matrix to be applied 
	 * Equivilant to this = this * m 
   */
	Matrix3& operator*=( const Matrix3& m )
  {
    setValue( m.tdotx( m_el[0] ), m.tdoty( m_el[0] ), m.tdotz( m_el[0] ),
		  m.tdotx( m_el[1] ), m.tdoty( m_el[1] ), m.tdotz( m_el[1] ),
		  m.tdotx( m_el[2] ), m.tdoty( m_el[2] ), m.tdotz( m_el[2] ) );
	  return *this;
  }

	/** @brief Adds by the target matrix on the right
	 *  @param m matrix to be applied 
	 * Equivilant to this = this + m 
   */
	Matrix3& operator+=( const Matrix3& m )
  {
	  setValue(
		  m_el[0].x + m.m_el[0].x, 
		  m_el[0].y + m.m_el[0].y,
		  m_el[0].z + m.m_el[0].z,
		  m_el[1].x + m.m_el[1].x, 
		  m_el[1].y + m.m_el[1].y,
		  m_el[1].z + m.m_el[1].z,
		  m_el[2].x + m.m_el[2].x, 
		  m_el[2].y + m.m_el[2].y,
		  m_el[2].z + m.m_el[2].z );
	  return *this;
  }

	/** @brief Substractss by the target matrix on the right
	 *  @param m matrix to be applied 
	 * Equivilant to this = this - m 
   */
	Matrix3& operator-=( const Matrix3& m )
  {
	  setValue(
	    m_el[0].x - m.m_el[0].x, 
	    m_el[0].y - m.m_el[0].y,
	    m_el[0].z - m.m_el[0].z,
	    m_el[1].x - m.m_el[1].x, 
	    m_el[1].y - m.m_el[1].y,
	    m_el[1].z - m.m_el[1].z,
	    m_el[2].x - m.m_el[2].x, 
	    m_el[2].y - m.m_el[2].y,
	    m_el[2].z - m.m_el[2].z );
	  return *this;
  }

  /** @brief Set from the rotational part of a 4x4 OpenGL matrix
	 *  @param m A pointer to the beginning of the array of scalars
   */
	void setFromOpenGLSubMatrix(const Real *m)
	{
  	m_el[0].set( m[0], m[4], m[8] );
		m_el[1].set( m[1], m[5], m[9] );
		m_el[2].set( m[2], m[6], m[10] );
  }

  /** @brief Set the values of the matrix explicitly (row major)
	 *  @param xx Top left
	 *  @param xy Top Middle
	 *  @param xz Top Right
	 *  @param yx Middle Left
	 *  @param yy Middle Middle
	 *  @param yz Middle Right
	 *  @param zx Bottom Left
	 *  @param zy Bottom Middle
	 *  @param zz Bottom Right
   */
  void setValue( Real xx, Real xy, Real xz,
    Real yx, Real yy, Real yz,
    Real zx, Real zy, Real zz )
  {
		m_el[0].set( xx, xy, xz );
		m_el[1].set( yx, yy, yz );
		m_el[2].set( zx, zy, zz );
	}

  /** @brief Set the matrix from a quaternion
	 *  @param q The Quaternion to match 
   */  
  void setRotation( const Quaternion& q )
  { 
    Real d = q.length2();
	  NE_ASSERT( d != Real(0.), "Length of Quaternion is 0" )( d );
	  Real s = Real( 2. ) / d;
	  Real xs = q.m_vec.x * s,   ys = q.m_vec.y * s,   zs = q.m_vec.z * s;
	  Real wx = q.m_vec.w * xs,  wy = q.m_vec.w * ys,  wz = q.m_vec.w * zs;
	  Real xx = q.m_vec.x * xs,  xy = q.m_vec.x * ys,  xz = q.m_vec.x * zs;
	  Real yy = q.m_vec.y * ys,  yz = q.m_vec.y * zs,  zz = q.m_vec.z * zs;
	
    setValue( Real( 1. ) - ( yy + zz ), xy - wz, xz + wy,
		  xy + wz, Real( 1. ) - ( xx + zz ), yz - wx,
		  xz - wy, yz + wx, Real( 1. ) - ( xx + yy ) );
  }

  /**@brief Set the matrix to the identity */
	void setIdentity()
	{ 
		setValue( Real( 1. ), Real( 0. ), Real( 0. ), 
			Real( 0. ), Real( 1. ), Real( 0. ), 
			Real( 0. ), Real( 0. ), Real( 1. )); 
	}

	static const Matrix3&	getIdentity()
	{
		static const Matrix3 identityMatrix( Real( 1. ), Real( 0. ), Real( 0. ), 
			Real( 0. ), Real( 1. ), Real( 0. ), 
			Real( 0. ), Real( 0. ), Real( 1. ));
		return identityMatrix;
	}

  /** @brief Fill the rotational part of an OpenGL matrix and clear the shear/perspective
	 *  @param m The array to be filled 
   */
	void getOpenGLSubMatrix( Real *m ) const 
	{
		m[0]  = Real( m_el[0].x ); 
		m[1]  = Real( m_el[1].x );
		m[2]  = Real( m_el[2].x );
		m[3]  = Real( 0. ); 
		m[4]  = Real( m_el[0].y );
		m[5]  = Real( m_el[1].y );
		m[6]  = Real( m_el[2].y );
		m[7]  = Real( 0. ); 
		m[8]  = Real( m_el[0].z ); 
		m[9]  = Real( m_el[1].z );
		m[10] = Real( m_el[2].z );
		m[11] = Real( 0. );  
  }
  
  /**@brief Get the matrix represented as a quaternion 
	 * @param q The quaternion which will be set 
   */
	void getRotation( Quaternion& q ) const
	{
		Real trace = m_el[0].x + m_el[1].y + m_el[2].z;
		Real temp[4];

		if ( trace > Real( 0. ) ) 
		{
			Real s = sqrt( trace + Real( 1. ) );
			temp[3] = ( s * Real( 0.5 ) );
			s = Real( 0.5 ) / s;

			temp[0] = ( ( m_el[2].y - m_el[1].z ) * s );
			temp[1] = ( ( m_el[0].z - m_el[2].x ) * s );
			temp[2] = ( ( m_el[1].x - m_el[0].y ) * s );
		} 
		else 
		{
			int i = m_el[0].x < m_el[1].y ? 
				( m_el[1].y < m_el[2].z ? 2 : 1 ) :
				( m_el[0].x < m_el[2].z ? 2 : 0 ); 
			int j = ( i + 1 ) % 3;  
			int k = ( i + 2 ) % 3;

			Real s = sqrt( m_el[i][i] - m_el[j][j] - m_el[k][k] + Real( 1. ) );
			temp[i] = s * Real( 0.5 );
			s = Real( 0.5 ) / s;

			temp[3] = ( m_el[k][j] - m_el[j][k] ) * s;
			temp[j] = ( m_el[j][i] + m_el[i][j] ) * s;
			temp[k] = ( m_el[k][i] + m_el[i][k] ) * s;
		}

		q.setValue( temp[0], temp[1], temp[2], temp[3] );
	}
  
  /**@brief Return the determinant of the matrix */
	Real determinant() const;
	/**@brief Return the adjoint of the matrix */
	Matrix3 adjoint() const;
	/**@brief Return the matrix with all values non negative */
	Matrix3 absolute() const;
	/**@brief Return the transpose of the matrix */
	Matrix3 transpose() const;
	/**@brief Return the inverse of the matrix */
	Matrix3 inverse() const; 

  Real tdotx( const Vector4& v ) const 
	{
		return m_el[0].x * v.x + m_el[1].x * v.y + m_el[2].x * v.z;
	}
	Real tdoty( const Vector4& v ) const 
	{
		return m_el[0].y * v.x + m_el[1].y * v.y + m_el[2].y * v.z;
	}
	Real tdotz( const Vector4& v ) const 
	{
		return m_el[0].z * v.x + m_el[1].z * v.y + m_el[2].z * v.z;
	}
  
  /**@brief Calculate the matrix cofactor 
	 * @param r1 The first row to use for calculating the cofactor
	 * @param c1 The first column to use for calculating the cofactor
	 * @param r1 The second row to use for calculating the cofactor
	 * @param c1 The second column to use for calculating the cofactor
	 * See http://en.wikipedia.org/wiki/Cofactor_(linear_algebra) for more details
	 */
	Real cofac(int r1, int c1, int r2, int c2) const 
	{
		return m_el[r1][c1] * m_el[r2][c2] - m_el[r1][c2] * m_el[r2][c1];
	}

private:
  Vector4 m_el[3];
  
};

inline Matrix3 operator*( const Matrix3& m, const Real& k )
{
	return Matrix3(
		m[0].x*k, m[0].y*k, m[0].z*k,
		m[1].x*k, m[1].y*k, m[1].z*k,
		m[2].x*k, m[2].y*k, m[2].z*k );
}

inline Matrix3 operator+( const Matrix3& m1, const Matrix3& m2 )
{
	return Matrix3(
	  m1[0].x + m2[0].x, 
	  m1[0].y + m2[0].y,
	  m1[0].z + m2[0].z,
	  m1[1].x + m2[1].x, 
	  m1[1].y + m2[1].y,
	  m1[1].z + m2[1].z,
	  m1[2].x + m2[2].x, 
	  m1[2].y + m2[2].y,
	  m1[2].z + m2[2].z );
}

inline Matrix3 operator-( const Matrix3& m1, const Matrix3& m2 )
{
	return Matrix3(
	  m1[0].x - m2[0].x, 
	  m1[0].y - m2[0].y,
	  m1[0].z - m2[0].z,
	  m1[1].x - m2[1].x, 
	  m1[1].y - m2[1].y,
	  m1[1].z - m2[1].z,
	  m1[2].x - m2[2].x, 
	  m1[2].y - m2[2].y,
	  m1[2].z - m2[2].z );
}

inline Vector4 operator*( const Matrix3& m, const Vector4& v ) 
{
	return Vector4( m[0].dot(v), m[1].dot(v), m[2].dot(v) );
}

inline Vector4 operator*( const Vector4& v, const Matrix3& m )
{
	return Vector4( m.tdotx( v ), m.tdoty( v ), m.tdotz( v ) );
}

inline Matrix3 operator*( const Matrix3& m1, const Matrix3& m2 )
{
	return Matrix3(
		m2.tdotx( m1[0] ), m2.tdoty( m1[0] ), m2.tdotz( m1[0] ),
		m2.tdotx( m1[1] ), m2.tdoty( m1[1] ), m2.tdotz( m1[1] ),
		m2.tdotx( m1[2] ), m2.tdoty( m1[2] ), m2.tdotz( m1[2] ) );
}

/**@brief Equality operator between two matrices
 * It will test all elements are equal.  
 */
inline bool operator==( const Matrix3& m1, const Matrix3& m2 )
{
	return ( m1[0][0] == m2[0][0] && m1[1][0] == m2[1][0] && m1[2][0] == m2[2][0] &&
		m1[0][1] == m2[0][1] && m1[1][1] == m2[1][1] && m1[2][1] == m2[2][1] &&
		m1[0][2] == m2[0][2] && m1[1][2] == m2[1][2] && m1[2][2] == m2[2][2] );
}

#endif // __MATRIX3_H__