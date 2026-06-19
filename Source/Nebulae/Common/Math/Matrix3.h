#ifndef NEBULAE_SOURCE_NEBULAE_COMMON_MATH_MATRIX3_H_
#define NEBULAE_SOURCE_NEBULAE_COMMON_MATH_MATRIX3_H_

/** @brief Basic 3x3 matrix.  Stored Row Major as 9 contiguous Real values.
 *
 *  Storage layout (row-major):
 *    m_el[0] = xx, m_el[1] = xy, m_el[2] = xz
 *    m_el[3] = yx, m_el[4] = yy, m_el[5] = yz
 *    m_el[6] = zx, m_el[7] = zy, m_el[8] = zz
 */
class Matrix3
{
public:
  /** @brief Mutable row wrapper.
   *
   *  Wraps a pointer to 3 consecutive Real values (one row of the matrix).
   *  Supports element access via operator[], dot product with Vector4,
   *  and implicit conversion to Real* for compatibility with tdotx/y/z.
   */
  class Row
  {
    friend class Matrix3;
    Real* m_data;

    Row( Real* data ) : m_data( data ) {}

  public:
    Real& operator[]( int i ) { return m_data[i]; }
    const Real& operator[]( int i ) const { return m_data[i]; }

    Real dot( const Vector4& v ) const { return m_data[0] * v.x + m_data[1] * v.y + m_data[2] * v.z; }

    operator Real*() { return m_data; }
    operator const Real*() const { return m_data; }
  };

  /** @brief Const row wrapper.
   *
   *  Wraps a const pointer to 3 consecutive Real values (one row of the matrix).
   *  Supports const element access, dot product with Vector4,
   *  and implicit conversion to const Real*.
   */
  class ConstRow
  {
    friend class Matrix3;
    const Real* m_data;

    ConstRow( const Real* data ) : m_data( data ) {}

  public:
    const Real& operator[]( int i ) const { return m_data[i]; }

    Real dot( const Vector4& v ) const { return m_data[0] * v.x + m_data[1] * v.y + m_data[2] * v.z; }

    operator const Real*() const { return m_data; }
  };
  /** @brief No initializaion constructor */
  Matrix3() {}

  /** @brief Constructor from Quaternion */
  explicit Matrix3( const Quaternion& q ) { setRotation( q ); }

  /** @brief Constructor with row major formatting */
  Matrix3( const Real& xx, const Real& xy, const Real& xz, const Real& yx, const Real& yy, const Real& yz,
           const Real& zx, const Real& zy, const Real& zz )
  {
    setValue( xx, xy, xz, yx, yy, yz, zx, zy, zz );
  }

  /** @brief Copy constructor */
  Matrix3( const Matrix3& other )
  {
    m_el[0] = other.m_el[0];
    m_el[1] = other.m_el[1];
    m_el[2] = other.m_el[2];
    m_el[3] = other.m_el[3];
    m_el[4] = other.m_el[4];
    m_el[5] = other.m_el[5];
    m_el[6] = other.m_el[6];
    m_el[7] = other.m_el[7];
    m_el[8] = other.m_el[8];
  }

  /** @brief Assignment Operator */
  Matrix3& operator=( const Matrix3& other )
  {
    m_el[0] = other.m_el[0];
    m_el[1] = other.m_el[1];
    m_el[2] = other.m_el[2];
    m_el[3] = other.m_el[3];
    m_el[4] = other.m_el[4];
    m_el[5] = other.m_el[5];
    m_el[6] = other.m_el[6];
    m_el[7] = other.m_el[7];
    m_el[8] = other.m_el[8];
    return *this;
  }

  Real* ptr();
  const Real* ptr() const;

  /** @brief Get a column of the matrix as a vector
   *  @param i Column number 0 indexed
   */
  Vector4 getColumn( uint32 i ) const { return Vector4( m_el[i], m_el[3 + i], m_el[6 + i] ); }

  /** @brief Get a row of the matrix as a vector
   *  @param i Row number 0 indexed
   */
  Vector4 getRow( uint32 i ) const
  {
    NE_ASSERT( i < 3, "Invalid row index" );
    return Vector4( m_el[i * 3], m_el[i * 3 + 1], m_el[i * 3 + 2] );
  }

  /** @brief Get a mutable row wrapper
   *  @param i Row number 0 indexed
   */
  Row operator[]( uint32 i )
  {
    NE_ASSERT( i < 3, "Invalid row index" );
    return Row( m_el + i * 3 );
  }

  /** @brief Get a const row wrapper
   *  @param i Row number 0 indexed
   */
  ConstRow operator[]( uint32 i ) const
  {
    NE_ASSERT( i < 3, "Invalid row index" );
    return ConstRow( m_el + i * 3 );
  }

  /** @brief Multiply by the target matrix on the right
   *  @param m Rotation matrix to be applied
   * Equivilant to this = this * m
   */
  Matrix3& operator*=( const Matrix3& m )
  {
    setValue( m.tdotx( m_el ), m.tdoty( m_el ), m.tdotz( m_el ), m.tdotx( m_el + 3 ), m.tdoty( m_el + 3 ),
              m.tdotz( m_el + 3 ), m.tdotx( m_el + 6 ), m.tdoty( m_el + 6 ), m.tdotz( m_el + 6 ) );
    return *this;
  }

  /** @brief Adds by the target matrix on the right
   *  @param m matrix to be applied
   * Equivilant to this = this + m
   */
  Matrix3& operator+=( const Matrix3& m )
  {
    m_el[0] += m.m_el[0];
    m_el[1] += m.m_el[1];
    m_el[2] += m.m_el[2];
    m_el[3] += m.m_el[3];
    m_el[4] += m.m_el[4];
    m_el[5] += m.m_el[5];
    m_el[6] += m.m_el[6];
    m_el[7] += m.m_el[7];
    m_el[8] += m.m_el[8];
    return *this;
  }

  /** @brief Substractss by the target matrix on the right
   *  @param m matrix to be applied
   * Equivilant to this = this - m
   */
  Matrix3& operator-=( const Matrix3& m )
  {
    m_el[0] -= m.m_el[0];
    m_el[1] -= m.m_el[1];
    m_el[2] -= m.m_el[2];
    m_el[3] -= m.m_el[3];
    m_el[4] -= m.m_el[4];
    m_el[5] -= m.m_el[5];
    m_el[6] -= m.m_el[6];
    m_el[7] -= m.m_el[7];
    m_el[8] -= m.m_el[8];
    return *this;
  }

  /** @brief Set from the rotational part of a 4x4 OpenGL matrix
   *  @param m A pointer to the beginning of the array of scalars
   *
   *  OpenGL stores matrices in column-major order:
   *    m[0]  m[4]  m[8]   m[12]
   *    m[1]  m[5]  m[9]   m[13]
   *    m[2]  m[6]  m[10]  m[14]
   *    m[3]  m[7]  m[11]  m[15]
   *
   *  We extract the upper-left 3x3 and store in row-major order.
   */
  void setFromOpenGLSubMatrix( const Real* m )
  {
    m_el[0] = m[0];
    m_el[1] = m[4];
    m_el[2] = m[8];
    m_el[3] = m[1];
    m_el[4] = m[5];
    m_el[5] = m[9];
    m_el[6] = m[2];
    m_el[7] = m[6];
    m_el[8] = m[10];
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
  void setValue( Real xx, Real xy, Real xz, Real yx, Real yy, Real yz, Real zx, Real zy, Real zz )
  {
    m_el[0] = xx;
    m_el[1] = xy;
    m_el[2] = xz;
    m_el[3] = yx;
    m_el[4] = yy;
    m_el[5] = yz;
    m_el[6] = zx;
    m_el[7] = zy;
    m_el[8] = zz;
  }

  /** @brief Set the matrix from a quaternion
   *  @param q The Quaternion to match
   */
  void setRotation( const Quaternion& q )
  {
    Real d = q.length2();
    NE_ASSERT( d != Real( 0. ), "Length of Quaternion is 0" );
    Real s = Real( 2. ) / d;
    Real xs = q.m_vec.x * s, ys = q.m_vec.y * s, zs = q.m_vec.z * s;
    Real wx = q.m_vec.w * xs, wy = q.m_vec.w * ys, wz = q.m_vec.w * zs;
    Real xx = q.m_vec.x * xs, xy = q.m_vec.x * ys, xz = q.m_vec.x * zs;
    Real yy = q.m_vec.y * ys, yz = q.m_vec.y * zs, zz = q.m_vec.z * zs;

    setValue( Real( 1. ) - ( yy + zz ), xy - wz, xz + wy, xy + wz, Real( 1. ) - ( xx + zz ), yz - wx, xz - wy, yz + wx,
              Real( 1. ) - ( xx + yy ) );
  }

  /**@brief Set the matrix to the identity */
  void setIdentity()
  {
    setValue( Real( 1. ), Real( 0. ), Real( 0. ), Real( 0. ), Real( 1. ), Real( 0. ), Real( 0. ), Real( 0. ),
              Real( 1. ) );
  }

  static const Matrix3& getIdentity()
  {
    static const Matrix3 identityMatrix( Real( 1. ), Real( 0. ), Real( 0. ), Real( 0. ), Real( 1. ), Real( 0. ),
                                         Real( 0. ), Real( 0. ), Real( 1. ) );
    return identityMatrix;
  }

  /** @brief Fill the rotational part of an OpenGL matrix and clear the shear/perspective
   *  @param m The array to be filled
   *
   *  Converts from row-major storage to OpenGL column-major 4x4 layout.
   */
  void getOpenGLSubMatrix( Real* m ) const
  {
    m[0] = m_el[0];
    m[1] = m_el[3];
    m[2] = m_el[6];
    m[3] = Real( 0. );
    m[4] = m_el[1];
    m[5] = m_el[4];
    m[6] = m_el[7];
    m[7] = Real( 0. );
    m[8] = m_el[2];
    m[9] = m_el[5];
    m[10] = m_el[8];
    m[11] = Real( 0. );
    m[12] = Real( 0. );
    m[13] = Real( 0. );
    m[14] = Real( 0. );
    m[15] = Real( 1. );
  }

  /**@brief Get the matrix represented as a quaternion
   * @param q The quaternion which will be set
   */
  void getRotation( Quaternion& q ) const
  {
    Real trace = m_el[0] + m_el[4] + m_el[8];
    Real temp[4];

    if ( trace > Real( 0. ) )
    {
      Real s = sqrt( trace + Real( 1. ) );
      temp[3] = ( s * Real( 0.5 ) );
      s = Real( 0.5 ) / s;

      temp[0] = ( ( m_el[7] - m_el[5] ) * s );
      temp[1] = ( ( m_el[2] - m_el[6] ) * s );
      temp[2] = ( ( m_el[3] - m_el[1] ) * s );
    }
    else
    {
      int i = m_el[0] < m_el[4] ? ( m_el[4] < m_el[8] ? 2 : 1 ) : ( m_el[0] < m_el[8] ? 2 : 0 );
      int j = ( i + 1 ) % 3;
      int k = ( i + 2 ) % 3;

      Real s = sqrt( m_el[i * 3 + i] - m_el[j * 3 + j] - m_el[k * 3 + k] + Real( 1. ) );
      temp[i] = s * Real( 0.5 );
      s = Real( 0.5 ) / s;

      temp[3] = ( m_el[k * 3 + j] - m_el[j * 3 + k] ) * s;
      temp[j] = ( m_el[j * 3 + i] + m_el[i * 3 + j] ) * s;
      temp[k] = ( m_el[k * 3 + i] + m_el[i * 3 + k] ) * s;
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

  /** @brief Transpose dot product with column 0 of this matrix.
   *  Equivalent to column0(this) · v
   */
  Real tdotx( const Real* v ) const { return m_el[0] * v[0] + m_el[3] * v[1] + m_el[6] * v[2]; }
  /** @brief Transpose dot product with column 1 of this matrix.
   *  Equivalent to column1(this) · v
   */
  Real tdoty( const Real* v ) const { return m_el[1] * v[0] + m_el[4] * v[1] + m_el[7] * v[2]; }
  /** @brief Transpose dot product with column 2 of this matrix.
   *  Equivalent to column2(this) · v
   */
  Real tdotz( const Real* v ) const { return m_el[2] * v[0] + m_el[5] * v[1] + m_el[8] * v[2]; }

  /**@brief Calculate the matrix cofactor
   * @param r1 The first row to use for calculating the cofactor
   * @param c1 The first column to use for calculating the cofactor
   * @param r1 The second row to use for calculating the cofactor
   * @param c1 The second column to use for calculating the cofactor
   * See http://en.wikipedia.org/wiki/Cofactor_(linear_algebra) for more details
   */
  Real cofac( int r1, int c1, int r2, int c2 ) const
  {
    return m_el[r1 * 3 + c1] * m_el[r2 * 3 + c2] - m_el[r1 * 3 + c2] * m_el[r2 * 3 + c1];
  }

private:
  Real m_el[9];
};

inline Matrix3 operator*( const Matrix3& m, const Real& k )
{
  return Matrix3( m[0][0] * k, m[0][1] * k, m[0][2] * k, m[1][0] * k, m[1][1] * k, m[1][2] * k, m[2][0] * k,
                  m[2][1] * k, m[2][2] * k );
}

inline Matrix3 operator+( const Matrix3& m1, const Matrix3& m2 )
{
  return Matrix3( m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2], m1[1][0] + m2[1][0],
                  m1[1][1] + m2[1][1], m1[1][2] + m2[1][2], m1[2][0] + m2[2][0], m1[2][1] + m2[2][1],
                  m1[2][2] + m2[2][2] );
}

inline Matrix3 operator-( const Matrix3& m1, const Matrix3& m2 )
{
  return Matrix3( m1[0][0] - m2[0][0], m1[0][1] - m2[0][1], m1[0][2] - m2[0][2], m1[1][0] - m2[1][0],
                  m1[1][1] - m2[1][1], m1[1][2] - m2[1][2], m1[2][0] - m2[2][0], m1[2][1] - m2[2][1],
                  m1[2][2] - m2[2][2] );
}

inline Vector4 operator*( const Matrix3& m, const Vector4& v )
{
  return Vector4( m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z, m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                  m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z );
}

inline Vector4 operator*( const Vector4& v, const Matrix3& m )
{
  return Vector4( m.tdotx( v.ptr() ), m.tdoty( v.ptr() ), m.tdotz( v.ptr() ) );
}

inline Matrix3 operator*( const Matrix3& m1, const Matrix3& m2 )
{
  return Matrix3( m2.tdotx( m1[0] ), m2.tdoty( m1[0] ), m2.tdotz( m1[0] ), m2.tdotx( m1[1] ), m2.tdoty( m1[1] ),
                  m2.tdotz( m1[1] ), m2.tdotx( m1[2] ), m2.tdoty( m1[2] ), m2.tdotz( m1[2] ) );
}

/**@brief Equality operator between two matrices
 * It will test all elements are equal.
 */
inline bool operator==( const Matrix3& m1, const Matrix3& m2 )
{
  return ( m1[0][0] == m2[0][0] && m1[1][0] == m2[1][0] && m1[2][0] == m2[2][0] && m1[0][1] == m2[0][1] &&
           m1[1][1] == m2[1][1] && m1[2][1] == m2[2][1] && m1[0][2] == m2[0][2] && m1[1][2] == m2[1][2] &&
           m1[2][2] == m2[2][2] );
}

#endif // NEBULAE_SOURCE_NEBULAE_COMMON_MATH_MATRIX3_H_
