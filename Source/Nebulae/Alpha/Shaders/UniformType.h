#ifndef NEBULAE_ALPHA_SHADERS_UNIFORMTYPE_H_
#define NEBULAE_ALPHA_SHADERS_UNIFORMTYPE_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

/** Enumeration of the types of constant we may encounter in programs. */
enum UniformType
{
  UT_FLOAT1,
  UT_FLOAT2,
  UT_FLOAT3,
  UT_FLOAT4,
  UT_SAMPLER1D,
  UT_SAMPLER2D,
  UT_SAMPLER3D,
  UT_SAMPLERCUBE,
  UT_SAMPLER1DSHADOW,
  UT_SAMPLER2DSHADOW0,
  UT_SAMPLER2DARRAY1,

  UT_MATRIX_2X2,
  UT_MATRIX_2X3,
  UT_MATRIX_2X4,
  UT_MATRIX_3X2,
  UT_MATRIX_3X3,
  UT_MATRIX_3X4,
  UT_MATRIX_4X2,
  UT_MATRIX_4X3,
  UT_MATRIX_4X4,

  UT_INT1,
  UT_INT2,
  UT_INT3,
  UT_INT4,

  UT_UNKNOWN
};

inline const char* GetUniformTypeName( UniformType type )
{
  switch ( type )
  {
  case UT_FLOAT1:
    return "float";
  case UT_FLOAT2:
    return "vec2";
  case UT_FLOAT3:
    return "vec3";
  case UT_FLOAT4:
    return "vec4";
  case UT_SAMPLER1D:
    return "sampler1D";
  case UT_SAMPLER2D:
    return "sampler2D";
  case UT_SAMPLER3D:
    return "sampler3D";
  case UT_SAMPLERCUBE:
    return "samplerCube";
  case UT_SAMPLER1DSHADOW:
    return "sampler1DShadow";
  case UT_SAMPLER2DSHADOW0:
    return "sampler2DShadow";
  case UT_SAMPLER2DARRAY1:
    return "sampler2DArray";
  case UT_MATRIX_2X2:
    return "mat2";
  case UT_MATRIX_2X3:
    return "mat2x3";
  case UT_MATRIX_2X4:
    return "mat2x4";
  case UT_MATRIX_3X2:
    return "mat3x2";
  case UT_MATRIX_3X3:
    return "mat3";
  case UT_MATRIX_3X4:
    return "mat3x4";
  case UT_MATRIX_4X2:
    return "mat4x2";
  case UT_MATRIX_4X3:
    return "mat4x3";
  case UT_MATRIX_4X4:
    return "mat4";
  case UT_INT1:
    return "int";
  case UT_INT2:
    return "ivec2";
  case UT_INT3:
    return "ivec3";
  case UT_INT4:
    return "ivec4";
  case UT_UNKNOWN:
  default:
    return "unknown";
  }
}

} // namespace Nebulae

#endif // NEBULAE_ALPHA_SHADERS_UNIFORMTYPE_H_
