// Matrix4.h

#ifndef NEBULAE_MATH_MATRIX4_H__
#define NEBULAE_MATH_MATRIX4_H__

class Matrix4
{
private:
  Real m[16];
 
public:
  static const Matrix4 Identity;
  static const Matrix4 Zero;
    
  public:
    Matrix4();
    Matrix4( Real xx, Real xy, Real xz, Real xw,
             Real yx, Real yy, Real yz, Real yw,
             Real zx, Real zy, Real zz, Real zw,
             Real wx, Real wy, Real wz, Real ww );

    void        SetRow( int index, const Real row[4] );
    void        SetRow( int index, const Vector4& v );
    void        SetColumn( int index, const Real col[4] );
    void        SetColumn( int index, const Vector4& v );

    Real*       ptr();
    const Real* ptr() const;

    void GetOpenGL( Real* buffer ) const;
    void SetFromOpenGl( const Real* buffer );
    
    void MakeOrthoMatrix( const Real l, const Real r, const Real b, const Real t, float n, const Real f );
    void MakePerspectiveMatrix( const Real fov, const Real nearplane, const Real farplane, const Real aspect );
    void MakePerspectiveMatrix( const Real l, const Real r, const Real t, const Real b, const Real n, const Real f );

    void MakeLookAtMatrix( const Vector4& pos, const Vector4& at, const Vector4& up );

    void        SetIdentity();                       
    Matrix4&    Transpose();                              // transpose itself and returns reference to itself (allows chaining).
    Matrix4&    Inverse();                                // inverses itself and returns reference to itself (allows chaining).
    Real        Determinant();

    // operators
    Matrix4     operator+( const Matrix4& rhs ) const;    // add rhs
    Matrix4     operator-( const Matrix4& rhs ) const;    // subtract rhs
    Matrix4     operator*( const Matrix4& rhs ) const;    // multiplication: M3 = M1 * M2

    Vector4     operator*( const Vector4& rhs ) const;    // multiplication: v' = M * v

    Matrix4&    operator+=( const Matrix4& rhs );         // add rhs and update this object
    Matrix4&    operator-=( const Matrix4& rhs );         // subtract rhs and update this object
    Matrix4&    operator*=( const Matrix4& rhs );         // multiplication: M1' = M1 * M2
      
    bool        operator==( const Matrix4& rhs ) const;   // exact compare, no epsilon
    bool        operator!=( const Matrix4& rhs ) const;   // exact compare, no epsilon
      
    Real        operator[]( int index ) const;            // subscript operator v[0], v[1]
    Real&       operator[]( int index );                  // subscript operator v[0], v[1]


  private:

    Real GetCofactor( Real m0, Real m1, Real m2, 
      Real m3, Real m4, Real m5,
      Real m6, Real m7, Real m8 );

}; //Matrix4

#endif // NEBULAE_MATH_MATRIX4_H__
