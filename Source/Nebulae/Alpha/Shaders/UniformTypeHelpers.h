#ifndef NEBULAE_ALPHA_UNIFORMTYPEHELPERS_H__
#define NEBULAE_ALPHA_UNIFORMTYPEHELPERS_H__

#include <cstddef>
#include <Nebulae/Alpha/Shaders/UniformType.h>

namespace Nebulae
{

/**
 * Classification of a uniform's buffer class.
 *
 * NOTE: UT_FLOAT4 and UT_MATRIX_2X2 both have elementCount == 4, but they are
 * different UniformTypes.  Code that dispatches on element count alone will
 * conflate the two.  Always dispatch on UniformType (or UniformBufferClass)
 * when the distinction matters.
 */
enum class UniformBufferClass
{
  Float,
  Int,
  Sampler
};

/** Classify a UniformType into its buffer class. */
constexpr inline UniformBufferClass GetUniformBufferClass( UniformType type ) noexcept
{
  switch ( type )
  {
    case UT_FLOAT1:
    case UT_FLOAT2:
    case UT_FLOAT3:
    case UT_FLOAT4:
    case UT_MATRIX_2X2:
    case UT_MATRIX_2X3:
    case UT_MATRIX_2X4:
    case UT_MATRIX_3X2:
    case UT_MATRIX_3X3:
    case UT_MATRIX_3X4:
    case UT_MATRIX_4X2:
    case UT_MATRIX_4X3:
    case UT_MATRIX_4X4:
      return UniformBufferClass::Float;

    case UT_INT1:
    case UT_INT2:
    case UT_INT3:
    case UT_INT4:
      return UniformBufferClass::Int;

    case UT_SAMPLER1D:
    case UT_SAMPLER2D:
    case UT_SAMPLER3D:
    case UT_SAMPLERCUBE:
    case UT_SAMPLER1DSHADOW:
    case UT_SAMPLER2DSHADOW0:
    case UT_SAMPLER2DARRAY1:
      return UniformBufferClass::Sampler;

    case UT_UNKNOWN:
    default:
      return UniformBufferClass::Float;
  }
}

/** Return the number of scalar elements for a given UniformType. */
constexpr inline std::size_t GetUniformElementCount( UniformType type ) noexcept
{
  switch ( type )
  {
    case UT_FLOAT1:
    case UT_INT1:
    case UT_UNKNOWN:
      return 1;

    case UT_FLOAT2:
    case UT_INT2:
      return 2;

    case UT_FLOAT3:
    case UT_INT3:
      return 3;

    case UT_FLOAT4:
    case UT_INT4:
    case UT_MATRIX_2X2:
      return 4;

    case UT_MATRIX_2X3:
      return 6;

    case UT_MATRIX_2X4:
    case UT_MATRIX_4X2:
      return 8;

    case UT_MATRIX_3X2:
      return 6;

    case UT_MATRIX_3X3:
      return 9;

    case UT_MATRIX_3X4:
    case UT_MATRIX_4X3:
      return 12;

    case UT_MATRIX_4X4:
      return 16;

    case UT_SAMPLER1D:
    case UT_SAMPLER2D:
    case UT_SAMPLER3D:
    case UT_SAMPLERCUBE:
    case UT_SAMPLER1DSHADOW:
    case UT_SAMPLER2DSHADOW0:
    case UT_SAMPLER2DARRAY1:
      return 1;

    default:
      return 1;
  }
}

/**
 * Return the total payload size in bytes for a uniform of the given type and
 * array size.  Each scalar element occupies 4 bytes.
 *
 *   payloadBytes = elementCount * 4 * arraySize
 *
 * Returns 0 when arraySize is 0.
 */
constexpr inline std::size_t GetUniformPayloadBytes( UniformType type, std::size_t arraySize ) noexcept
{
  return GetUniformElementCount( type ) * 4u * arraySize;
}

} // namespace Nebulae

#endif // NEBULAE_ALPHA_UNIFORMTYPEHELPERS_H__
