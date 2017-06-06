
///////////////////////////////////////////////////////////////////////////////
// Matrix4.inl
// ===========
// NxN Matrix Math classes
//
// All matrices are row major. (OpenGL uses column-major matrix)
//
//   |  0  1  2  3 |
//   |  4  5  6  7 |
//   |  8  9 10 11 |
//   | 12 13 14 15 |
//
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <algorithm>

const float DEG2RAD = 3.141593f / 180.0f;


inline Matrix4::Matrix4()
{}


inline Matrix4::Matrix4( Real xx, Real xy, Real xz, Real xw,
                         Real yx, Real yy, Real yz, Real yw,
                         Real zx, Real zy, Real zz, Real zw,
                         Real wx, Real wy, Real wz, Real ww )
{
  m[0] = xx;  m[1] = xy;  m[2] = xz;  m[3] = xw;
  m[4] = yx;  m[5] = yy;  m[6] = yz;  m[7] = yw;
  m[8] = zx;  m[9] = zy;  m[10]= zz;  m[11]= zw;
  m[12]= wx;  m[13]= wy;  m[14]= wz;  m[15]= ww;
}

inline void 
Matrix4::SetRow( int index, const Vector4& v )
{
  m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;  m[index*4 + 3] = v.w;
}


inline void 
Matrix4::SetColumn( int index, const Vector4& v )
{
  m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;  m[index + 12] = v.w;
}


inline Real* 
Matrix4::ptr()
{
  return m;
}


inline const Real* 
Matrix4::ptr() const
{
  return m;
}


inline void 
Matrix4::GetOpenGL( Real* buffer ) const
{
  buffer[0]  = m[0];
  buffer[1]  = m[4];
  buffer[2]  = m[8];
  buffer[3]  = m[12];
  buffer[4]  = m[1];
  buffer[5]  = m[5];
  buffer[6]  = m[9];
  buffer[7]  = m[13];
  buffer[8]  = m[2];
  buffer[9]  = m[6];
  buffer[10] = m[10];
  buffer[11] = m[14];
  buffer[12] = m[3];
  buffer[13] = m[7];
  buffer[14] = m[11];
  buffer[15] = m[15];
}

inline void 
Matrix4::SetFromOpenGl( const Real* buffer )
{
  m[0]  = buffer[0];
  m[4]  = buffer[1];
  m[8]  = buffer[2];
  m[12] = buffer[3];
  m[1]  = buffer[4];
  m[5]  = buffer[5];
  m[9]  = buffer[6];
  m[13] = buffer[7];
  m[2]  = buffer[8];
  m[6]  = buffer[9];
  m[10] = buffer[10];
  m[14] = buffer[11];
  m[3]  = buffer[12];
  m[7]  = buffer[13];
  m[11] = buffer[14];
  m[15] = buffer[15];
}

inline void 
Matrix4::SetIdentity()
{
  m[0] = m[5] = m[10] = m[15] = 1.0f;
  m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
}


inline Matrix4& 
Matrix4::Transpose()
{
  std::swap(m[1],  m[4]);
  std::swap(m[2],  m[8]);
  std::swap(m[3],  m[12]);
  std::swap(m[6],  m[9]);
  std::swap(m[7],  m[13]);
  std::swap(m[11], m[14]);

  return *this;
}


inline Matrix4& 
Matrix4::Inverse()
{
  Real inv[16], det;
  int i;

  inv[0] =  m[5]  * m[10] * m[15] - 
            m[5]  * m[11] * m[14] - 
            m[9]  * m[6]  * m[15] + 
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] - 
            m[13] * m[7]  * m[10];

  inv[4] = -m[4]  * m[10] * m[15] + 
            m[4]  * m[11] * m[14] + 
            m[8]  * m[6]  * m[15] - 
            m[8]  * m[7]  * m[14] - 
            m[12] * m[6]  * m[11] + 
            m[12] * m[7]  * m[10];

  inv[8] =  m[4]  * m[9] * m[15] - 
            m[4]  * m[11] * m[13] - 
            m[8]  * m[5] * m[15] + 
            m[8]  * m[7] * m[13] + 
            m[12] * m[5] * m[11] - 
            m[12] * m[7] * m[9];

  inv[12] = -m[4]  * m[9] * m[14] + 
             m[4]  * m[10] * m[13] +
             m[8]  * m[5] * m[14] - 
             m[8]  * m[6] * m[13] - 
             m[12] * m[5] * m[10] + 
             m[12] * m[6] * m[9];

  inv[1] = -m[1]  * m[10] * m[15] + 
            m[1]  * m[11] * m[14] + 
            m[9]  * m[2] * m[15] - 
            m[9]  * m[3] * m[14] - 
            m[13] * m[2] * m[11] + 
            m[13] * m[3] * m[10];

  inv[5] = m[0]  * m[10] * m[15] - 
           m[0]  * m[11] * m[14] - 
           m[8]  * m[2] * m[15] + 
           m[8]  * m[3] * m[14] + 
           m[12] * m[2] * m[11] - 
           m[12] * m[3] * m[10];

  inv[9] = -m[0]  * m[9] * m[15] + 
            m[0]  * m[11] * m[13] + 
            m[8]  * m[1] * m[15] - 
            m[8]  * m[3] * m[13] - 
            m[12] * m[1] * m[11] + 
            m[12] * m[3] * m[9];

  inv[13] = m[0]  * m[9] * m[14] - 
            m[0]  * m[10] * m[13] - 
            m[8]  * m[1] * m[14] + 
            m[8]  * m[2] * m[13] + 
            m[12] * m[1] * m[10] - 
            m[12] * m[2] * m[9];

  inv[2] = m[1]  * m[6] * m[15] - 
            m[1]  * m[7] * m[14] - 
            m[5]  * m[2] * m[15] + 
            m[5]  * m[3] * m[14] + 
            m[13] * m[2] * m[7] - 
            m[13] * m[3] * m[6];

  inv[6] = -m[0]  * m[6] * m[15] + 
            m[0]  * m[7] * m[14] + 
            m[4]  * m[2] * m[15] - 
            m[4]  * m[3] * m[14] - 
            m[12] * m[2] * m[7] + 
            m[12] * m[3] * m[6];

  inv[10] = m[0]  * m[5] * m[15] - 
            m[0]  * m[7] * m[13] - 
            m[4]  * m[1] * m[15] + 
            m[4]  * m[3] * m[13] + 
            m[12] * m[1] * m[7] - 
            m[12] * m[3] * m[5];

  inv[14] = -m[0]  * m[5] * m[14] + 
             m[0]  * m[6] * m[13] + 
             m[4]  * m[1] * m[14] - 
             m[4]  * m[2] * m[13] - 
             m[12] * m[1] * m[6] + 
             m[12] * m[2] * m[5];

  inv[3] = -m[1] * m[6] * m[11] + 
            m[1] * m[7] * m[10] + 
            m[5] * m[2] * m[11] - 
            m[5] * m[3] * m[10] - 
            m[9] * m[2] * m[7] + 
            m[9] * m[3] * m[6];

  inv[7] = m[0] * m[6] * m[11] - 
           m[0] * m[7] * m[10] - 
           m[4] * m[2] * m[11] + 
           m[4] * m[3] * m[10] + 
           m[8] * m[2] * m[7] - 
           m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11] + 
             m[0] * m[7] * m[9] + 
             m[4] * m[1] * m[11] - 
             m[4] * m[3] * m[9] - 
             m[8] * m[1] * m[7] + 
             m[8] * m[3] * m[5];

  inv[15] = m[0] * m[5] * m[10] - 
            m[0] * m[6] * m[9] - 
            m[4] * m[1] * m[10] + 
            m[4] * m[2] * m[9] + 
            m[8] * m[1] * m[6] - 
            m[8] * m[2] * m[5];

  det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

  assert( det != 0 );

  if( det != 0 ) {
    det = 1.0f / det;
    for( i = 0; i < 16; i++ ) {
      m[i] = inv[i] * det;
    }
  }
  
  return *this;
}



///////////////////////////////////////////////////////////////////////////////
// return determinant of 4x4 matrix
///////////////////////////////////////////////////////////////////////////////
inline Real 
Matrix4::Determinant()
{
  return m[0] * GetCofactor(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]) -
         m[1] * GetCofactor(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]) +
         m[2] * GetCofactor(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]) -
         m[3] * GetCofactor(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);
}



///////////////////////////////////////////////////////////////////////////////
// compute cofactor of 3x3 minor matrix without sign
// input params are 9 elements of the minor matrix
// NOTE: The caller must know its sign.
///////////////////////////////////////////////////////////////////////////////
inline Real 
Matrix4::GetCofactor( float m0, float m1, float m2,
                      float m3, float m4, float m5,
                      float m6, float m7, float m8 )
{
  return m0 * (m4 * m8 - m5 * m7) -
         m1 * (m3 * m8 - m5 * m6) +
         m2 * (m3 * m7 - m4 * m6);
}



///////////////////////////////////////////////////////////////////////////////
// translate this matrix by (x, y, z)
///////////////////////////////////////////////////////////////////////////////
//Matrix4& Matrix4::translate(const Vector4& v)
//{
//  return translate(v.x, v.y, v.z);
//}
//
//Matrix4& Matrix4::translate(float x, float y, float z)
//{
//  m[0] += m[12]*x;   m[1] += m[13]*x;   m[2] += m[14]*x;   m[3] += m[15]*x;
//  m[4] += m[12]*y;   m[5] += m[13]*y;   m[6] += m[14]*y;   m[7] += m[15]*y;
//  m[8] += m[12]*z;   m[9] += m[13]*z;   m[10]+= m[14]*z;   m[11]+= m[15]*z;
//  return *this;
//}



///////////////////////////////////////////////////////////////////////////////
// uniform scale
///////////////////////////////////////////////////////////////////////////////
//Matrix4& Matrix4::scale( Real s )
//{
//  return scale( s, s, s );
//}
//
//Matrix4& Matrix4::scale( Real x, Real y, Real z )
//{
//  m[0] = m[0]*x;   m[1] = m[1]*x;   m[2] = m[2]*x;   m[3] = m[3]*x;
//  m[4] = m[4]*y;   m[5] = m[5]*y;   m[6] = m[6]*y;   m[7] = m[7]*y;
//  m[8] = m[8]*z;   m[9] = m[9]*z;   m[10]= m[10]*z;  m[11]= m[11]*z;
//  return *this;
//}



///////////////////////////////////////////////////////////////////////////////
// build a rotation matrix with given angle(degree) and rotation axis, then
// multiply it with this object
///////////////////////////////////////////////////////////////////////////////
//Matrix4& Matrix4::rotate( Real angle, const Vector4& axis )
//{
//  return rotate( angle, axis.x, axis.y, axis.z );
//}
//
//Matrix4& Matrix4::rotate( Real angle, Real x, Real y, Real z)
//{
//  float c = cosf(angle * DEG2RAD);    // cosine
//  float s = sinf(angle * DEG2RAD);    // sine
//  float xx = x * x;
//  float xy = x * y;
//  float xz = x * z;
//  float yy = y * y;
//  float yz = y * z;
//  float zz = z * z;
//
//  // build rotation matrix
//  Matrix4 m;
//  m[0] = xx * (1 - c) + c;
//  m[1] = xy * (1 - c) - z * s;
//  m[2] = xz * (1 - c) + y * s;
//  m[3] = 0;
//  m[4] = xy * (1 - c) + z * s;
//  m[5] = yy * (1 - c) + c;
//  m[6] = yz * (1 - c) - x * s;
//  m[7] = 0;
//  m[8] = xz * (1 - c) - y * s;
//  m[9] = yz * (1 - c) + x * s;
//  m[10]= zz * (1 - c) + c;
//  m[11]= 0;
//  m[12]= 0;
//  m[13]= 0;
//  m[14]= 0;
//  m[15]= 1;
//
//  // multiply it
//  *this = m * (*this);
//
//  return *this;
//}
//
//Matrix4& Matrix4::rotateX(float angle)
//{
//  float c = cosf(angle * DEG2RAD);
//  float s = sinf(angle * DEG2RAD);
//  float m4 = m[4], m5 = m[5], m6 = m[6],  m7 = m[7],
//        m8 = m[8], m9 = m[9], m10= m[10], m11= m[11];
//
//  m[4] = m4 * c + m8 *-s;
//  m[5] = m5 * c + m9 *-s;
//  m[6] = m6 * c + m10*-s;
//  m[7] = m7 * c + m11*-s;
//  m[8] = m4 * s + m8 * c;
//  m[9] = m5 * s + m9 * c;
//  m[10]= m6 * s + m10* c;
//  m[11]= m7 * s + m11* c;
//
//  return *this;
//}
//
//Matrix4& Matrix4::rotateY(float angle)
//{
//  float c = cosf(angle * DEG2RAD);
//  float s = sinf(angle * DEG2RAD);
//  float m0 = m[0], m1 = m[1], m2 = m[2],  m3 = m[3],
//        m8 = m[8], m9 = m[9], m10= m[10], m11= m[11];
//
//  m[0] = m0 * c + m8 * s;
//  m[1] = m1 * c + m9 * s;
//  m[2] = m2 * c + m10* s;
//  m[3] = m3 * c + m11* s;
//  m[8] = m0 *-s + m8 * c;
//  m[9] = m1 *-s + m9 * c;
//  m[10]= m2 *-s + m10* c;
//  m[11]= m3 *-s + m11* c;
//
//  return *this;
//}
//
//Matrix4& Matrix4::rotateZ(float angle)
//{
//  float c = cosf(angle * DEG2RAD);
//  float s = sinf(angle * DEG2RAD);
//  float m0 = m[0], m1 = m[1], m2 = m[2],  m3 = m[3],
//        m4 = m[4], m5 = m[5], m6 = m[6],  m7 = m[7];
//
//  m[0] = m0 * c + m4 *-s;
//  m[1] = m1 * c + m5 *-s;
//  m[2] = m2 * c + m6 *-s;
//  m[3] = m3 * c + m7 *-s;
//  m[4] = m0 * s + m4 * c;
//  m[5] = m1 * s + m5 * c;
//  m[6] = m2 * s + m6 * c;
//  m[7] = m3 * s + m7 * c;
//
//  return *this;
//}

//void Matrix4::SetTrans( Real x, Real y, Real z )
//{            
//  m[12] = x;
//  m[13] = y;
//  m[14] = z;
//}
//
//void Matrix4::MakeTrans( Real x, Real y, Real z )
//{
//  m[0] = 1.0; m[4] = 0.0; m[8]  = 0.0; m[12] = x;
//  m[1] = 0.0; m[5] = 1.0; m[9]  = 0.0; m[13] = y;
//  m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = z;
//  m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
//}


inline Matrix4 
Matrix4::operator+( const Matrix4& rhs ) const
{
  return Matrix4( m[0]+rhs.m[0],   m[1]+rhs.m[1],   m[2]+rhs.m[2],   m[3]+rhs.m[3],
                  m[4]+rhs.m[4],   m[5]+rhs.m[5],   m[6]+rhs.m[6],   m[7]+rhs.m[7],
                  m[8]+rhs.m[8],   m[9]+rhs.m[9],   m[10]+rhs.m[10], m[11]+rhs.m[11],
                  m[12]+rhs.m[12], m[13]+rhs.m[13], m[14]+rhs.m[14], m[15]+rhs.m[15] );
}


inline Matrix4 
Matrix4::operator-( const Matrix4& rhs ) const
{
  return Matrix4( m[0]-rhs.m[0],   m[1]-rhs.m[1],   m[2]-rhs.m[2],   m[3]-rhs.m[3],
                  m[4]-rhs.m[4],   m[5]-rhs.m[5],   m[6]-rhs.m[6],   m[7]-rhs.m[7],
                  m[8]-rhs.m[8],   m[9]-rhs.m[9],   m[10]-rhs.m[10], m[11]-rhs.m[11],
                  m[12]-rhs.m[12], m[13]-rhs.m[13], m[14]-rhs.m[14], m[15]-rhs.m[15] );
}


inline Matrix4& 
Matrix4::operator+=( const Matrix4& rhs )
{
  m[0] += rhs.m[0];    m[1] += rhs.m[1];    m[2] += rhs.m[2];    m[3] += rhs.m[3];
  m[4] += rhs.m[4];    m[5] += rhs.m[5];    m[6] += rhs.m[6];    m[7] += rhs.m[7];
  m[8] += rhs.m[8];    m[9] += rhs.m[9];    m[10] += rhs.m[10];  m[11] += rhs.m[11];
  m[12] += rhs.m[12];  m[13] += rhs.m[13];  m[14] += rhs.m[14];  m[15] += rhs.m[15];
  return *this;
}


inline Matrix4& 
Matrix4::operator-=( const Matrix4& rhs )
{
  m[0] -= rhs.m[0];    m[1] -= rhs.m[1];    m[2] -= rhs.m[2];    m[3] -= rhs.m[3];
  m[4] -= rhs.m[4];    m[5] -= rhs.m[5];    m[6] -= rhs.m[6];    m[7] -= rhs.m[7];
  m[8] -= rhs.m[8];    m[9] -= rhs.m[9];    m[10] -= rhs.m[10];  m[11] -= rhs.m[11];
  m[12] -= rhs.m[12];  m[13] -= rhs.m[13];  m[14] -= rhs.m[14];  m[15] -= rhs.m[15];
  return *this;
}


inline Vector4 
Matrix4::operator*( const Vector4& rhs ) const
{
  return Vector4( m[0]*rhs.x  + m[1]*rhs.y  + m[2]*rhs.z  + m[3]*rhs.w,
                  m[4]*rhs.x  + m[5]*rhs.y  + m[6]*rhs.z  + m[7]*rhs.w,
                  m[8]*rhs.x  + m[9]*rhs.y  + m[10]*rhs.z + m[11]*rhs.w,
                  m[12]*rhs.x + m[13]*rhs.y + m[14]*rhs.z + m[15]*rhs.w );
}


inline Matrix4 
Matrix4::operator*( const Matrix4& n ) const
{
  return Matrix4( m[0]*n.m[0]  + m[1]*n.m[4]  + m[2]*n.m[8]  + m[3]*n.m[12],   m[0]*n.m[1]  + m[1]*n.m[5]  + m[2]*n.m[9]  + m[3]*n.m[13],   m[0]*n.m[2]  + m[1]*n.m[6]  + m[2]*n.m[10]  + m[3]*n.m[14],   m[0]*n.m[3]  + m[1]*n.m[7]  + m[2]*n.m[11]  + m[3]*n.m[15],
                  m[4]*n.m[0]  + m[5]*n.m[4]  + m[6]*n.m[8]  + m[7]*n.m[12],   m[4]*n.m[1]  + m[5]*n.m[5]  + m[6]*n.m[9]  + m[7]*n.m[13],   m[4]*n.m[2]  + m[5]*n.m[6]  + m[6]*n.m[10]  + m[7]*n.m[14],   m[4]*n.m[3]  + m[5]*n.m[7]  + m[6]*n.m[11]  + m[7]*n.m[15],
                  m[8]*n.m[0]  + m[9]*n.m[4]  + m[10]*n.m[8] + m[11]*n.m[12],  m[8]*n.m[1]  + m[9]*n.m[5]  + m[10]*n.m[9] + m[11]*n.m[13],  m[8]*n.m[2]  + m[9]*n.m[6]  + m[10]*n.m[10] + m[11]*n.m[14],  m[8]*n.m[3]  + m[9]*n.m[7]  + m[10]*n.m[11] + m[11]*n.m[15],
                  m[12]*n.m[0] + m[13]*n.m[4] + m[14]*n.m[8] + m[15]*n.m[12],  m[12]*n.m[1] + m[13]*n.m[5] + m[14]*n.m[9] + m[15]*n.m[13],  m[12]*n.m[2] + m[13]*n.m[6] + m[14]*n.m[10] + m[15]*n.m[14],  m[12]*n.m[3] + m[13]*n.m[7] + m[14]*n.m[11] + m[15]*n.m[15]);
}


inline Matrix4& 
Matrix4::operator*=( const Matrix4& rhs )
{
  *this = *this * rhs;
  return *this;
}


inline bool        
Matrix4::operator==( const Matrix4& rhs ) const
{
  return 
    m[0] == rhs.m[0] && 
    m[1] == rhs.m[1] &&
    m[2] == rhs.m[2] && 
    m[3] == rhs.m[3] && 
    m[4] == rhs.m[4] && 
    m[5] == rhs.m[5] && 
    m[6] == rhs.m[6] && 
    m[7] == rhs.m[7] && 
    m[8] == rhs.m[8] && 
    m[9] == rhs.m[9] && 
    m[10] == rhs.m[10] && 
    m[11] == rhs.m[11] && 
    m[12] == rhs.m[12] && 
    m[13] == rhs.m[13] && 
    m[14] == rhs.m[14] &&
    m[15] == rhs.m[15] ;
}


inline bool     
Matrix4::operator!=( const Matrix4& rhs ) const
{
  return !(*this == rhs);
}


inline Real
Matrix4::operator[]( int index ) const
{ return m[index]; }


inline Real&
Matrix4::operator[]( int index )
{ return m[index]; }


inline Matrix4 
MatrixMakeTranslation( Real fOffsetX, Real fOffsetY, Real fOffsetZ )
{
  return Matrix4( 1.0f, 0,    0,    fOffsetX,
                  0,    1.0f, 0,    fOffsetY, 
                  0,    0,    1.0f, fOffsetZ, 
                  0,    0,    0,    1.0f );
}


inline Matrix4 
MatrixMakeScale( Real fScaleX, Real fScaleY, Real fScaleZ )
{
  return Matrix4( fScaleX, 0, 0, 0,
                  0, fScaleY, 0, 0, 
                  0, 0, fScaleZ, 0, 
                  0, 0, 0, 1.0f );
}


inline Matrix4 
MatrixMakeRotation( const Real angle, const Vector4 axis )
{
  float c = cosf(angle);
  float s = sinf(angle);
  float xx = axis.x * axis.x;
  float xy = axis.x * axis.y;
  float xz = axis.x * axis.z;
  float yy = axis.y * axis.y;
  float yz = axis.y * axis.z;
  float zz = axis.z * axis.z;

  // build rotation matrix
  return Matrix4(
    xx * (1 - c) + c,
    xy * (1 - c) - axis.z * s,
    xz * (1 - c) + axis.y * s,
    0,
    xy * (1 - c) + axis.z * s,
    yy * (1 - c) + c,
    yz * (1 - c) - axis.x * s,
    0,
    xz * (1 - c) - axis.y * s,
    yz * (1 - c) + axis.x * s,
    zz * (1 - c) + c,
    0,
    0,
    0,
    0,
    1 );
}


inline Matrix4 
MatrixMakeFromOGLBuffer( const Real* buffer )
{
  return Matrix4( buffer[0],  buffer[4],  buffer[8],  buffer[12], 
                  buffer[1],  buffer[5],  buffer[9],  buffer[13], 
                  buffer[2],  buffer[6],  buffer[10], buffer[14], 
                  buffer[3],  buffer[7],  buffer[11], buffer[15] );
}


inline void 
MatrixFillOGLBuffer( const Matrix4& m, Real* bufferOut )
{
  const Real* mbuf = m.ptr();

  bufferOut[0]  = mbuf[0];
  bufferOut[1]  = mbuf[4];
  bufferOut[2]  = mbuf[8];
  bufferOut[3]  = mbuf[12];
  bufferOut[4]  = mbuf[1];
  bufferOut[5]  = mbuf[5];
  bufferOut[6]  = mbuf[9];
  bufferOut[7]  = mbuf[13];
  bufferOut[8]  = mbuf[2];
  bufferOut[9]  = mbuf[6];
  bufferOut[10] = mbuf[10];
  bufferOut[11] = mbuf[14];
  bufferOut[12] = mbuf[3];
  bufferOut[13] = mbuf[7];
  bufferOut[14] = mbuf[11];
  bufferOut[15] = mbuf[15];
}

