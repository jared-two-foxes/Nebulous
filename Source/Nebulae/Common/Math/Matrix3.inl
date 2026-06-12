// Matrix3.inl
// Inline implementations for Matrix3 class


inline Real*
Matrix3::ptr()
{
  return m_el;
}


inline const Real*
Matrix3::ptr() const
{
  return m_el;
}

// Compute the determinant of the 3x3 matrix
inline Real Matrix3::determinant() const
{
  return m_el[0] * (m_el[4] * m_el[8] - m_el[5] * m_el[7]) -
         m_el[1] * (m_el[3] * m_el[8] - m_el[5] * m_el[6]) +
         m_el[2] * (m_el[3] * m_el[7] - m_el[4] * m_el[6]);
}

// Compute the adjoint (adjugate) matrix
inline Matrix3 Matrix3::adjoint() const
{
  return Matrix3(
    cofac(1, 1, 2, 2), -cofac(0, 1, 2, 2), cofac(0, 1, 1, 2),
    -cofac(1, 0, 2, 2), cofac(0, 0, 2, 2), -cofac(0, 0, 1, 2),
    cofac(1, 0, 2, 1), -cofac(0, 0, 2, 1), cofac(0, 0, 1, 1)
  );
}

// Return the matrix with all values non-negative
inline Matrix3 Matrix3::absolute() const
{
  return Matrix3(
    fabsf(m_el[0]), fabsf(m_el[1]), fabsf(m_el[2]),
    fabsf(m_el[3]), fabsf(m_el[4]), fabsf(m_el[5]),
    fabsf(m_el[6]), fabsf(m_el[7]), fabsf(m_el[8])
  );
}

// Return the transpose of the matrix
inline Matrix3 Matrix3::transpose() const
{
  return Matrix3(
    m_el[0], m_el[3], m_el[6],
    m_el[1], m_el[4], m_el[7],
    m_el[2], m_el[5], m_el[8]
  );
}

// Return the inverse of the matrix
inline Matrix3 Matrix3::inverse() const
{
  Real det = determinant();

  // Check for singular matrix
  if (fabsf(det) < 1e-10f)
  {
    // Return identity if matrix is singular
    return Matrix3::getIdentity();
  }

  // Compute inverse as adjoint / determinant
  Matrix3 adj = adjoint();
  return adj * (Real(1.0) / det);
}
