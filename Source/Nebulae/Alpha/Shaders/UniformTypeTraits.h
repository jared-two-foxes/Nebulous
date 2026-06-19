#ifndef NEBULAE_ALPHA_SHADERS_UNIFORMTYPETRAITS_H_
#define NEBULAE_ALPHA_SHADERS_UNIFORMTYPETRAITS_H_

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Shaders/UniformType.h>

namespace Nebulae
{

class Texture;

/** Maps c++ types to UniformType enum values */
template <typename T> struct UniformTypeTraits;


/** Specialization for each type */
template <> struct UniformTypeTraits<float>
{
  static constexpr UniformType value = UT_FLOAT1;
};

template <> struct UniformTypeTraits<Vector2>
{
  static constexpr UniformType value = UT_FLOAT2;
};

template <> struct UniformTypeTraits<Vector4>
{
  static constexpr UniformType value = UT_FLOAT4;
};

template <> struct UniformTypeTraits<int32>
{
  static constexpr UniformType value = UT_INT1;
};

// template<>
// struct UniformTypeTraits<Vector2i>
//{
//   static constexpr UniformType value = UT_INT2;
// };
//
// template<>
// struct UniformTypeTraits<Vector3i>
//{
//   static constexpr UniformType value = UT_INT3;
// };
//
// template<>
// struct UniformTypeTraits<Vector4i>
//{
//   static constexpr UniformType value = UT_INT4;
// };

template <> struct UniformTypeTraits<Matrix3>
{
  static constexpr UniformType value = UT_MATRIX_3X3;
};

template <> struct UniformTypeTraits<Matrix4>
{
  static constexpr UniformType value = UT_MATRIX_4X4;
};

template <> struct UniformTypeTraits<Texture*>
{
  static constexpr UniformType value = UT_SAMPLER2D;
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_SHADERS_UNIFORMTYPETRAITS_H_
