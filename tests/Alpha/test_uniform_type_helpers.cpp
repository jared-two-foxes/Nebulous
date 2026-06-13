#include <gtest/gtest.h>
#include <Nebulae/Alpha/Shaders/UniformTypeHelpers.h>

namespace Nebulae {
namespace {

// ============================================================================
// GetUniformBufferClass tests
// ============================================================================

TEST(UniformTypeHelpersTest, GetUniformBufferClass_FloatTypes)
{
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_FLOAT1 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_FLOAT2 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_FLOAT3 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_FLOAT4 ) );
}

TEST(UniformTypeHelpersTest, GetUniformBufferClass_MatrixTypes)
{
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_2X2 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_2X3 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_2X4 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_3X2 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_3X3 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_3X4 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_4X2 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_4X3 ) );
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_MATRIX_4X4 ) );
}

TEST(UniformTypeHelpersTest, GetUniformBufferClass_IntTypes)
{
  EXPECT_EQ( UniformBufferClass::Int, GetUniformBufferClass( UT_INT1 ) );
  EXPECT_EQ( UniformBufferClass::Int, GetUniformBufferClass( UT_INT2 ) );
  EXPECT_EQ( UniformBufferClass::Int, GetUniformBufferClass( UT_INT3 ) );
  EXPECT_EQ( UniformBufferClass::Int, GetUniformBufferClass( UT_INT4 ) );
}

TEST(UniformTypeHelpersTest, GetUniformBufferClass_SamplerTypes)
{
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLER1D ) );
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLER2D ) );
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLER3D ) );
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLERCUBE ) );
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLER1DSHADOW ) );
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLER2DSHADOW0 ) );
  EXPECT_EQ( UniformBufferClass::Sampler, GetUniformBufferClass( UT_SAMPLER2DARRAY1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformBufferClass_UnknownDefaultsToFloat)
{
  EXPECT_EQ( UniformBufferClass::Float, GetUniformBufferClass( UT_UNKNOWN ) );
}

// ============================================================================
// GetUniformElementCount tests
// ============================================================================

TEST(UniformTypeHelpersTest, GetUniformElementCount_Float1_Int1)
{
  EXPECT_EQ( 1u, GetUniformElementCount( UT_FLOAT1 ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_INT1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Float2_Int2)
{
  EXPECT_EQ( 2u, GetUniformElementCount( UT_FLOAT2 ) );
  EXPECT_EQ( 2u, GetUniformElementCount( UT_INT2 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Float3_Int3)
{
  EXPECT_EQ( 3u, GetUniformElementCount( UT_FLOAT3 ) );
  EXPECT_EQ( 3u, GetUniformElementCount( UT_INT3 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Float4_Int4_Mat2x2)
{
  EXPECT_EQ( 4u, GetUniformElementCount( UT_FLOAT4 ) );
  EXPECT_EQ( 4u, GetUniformElementCount( UT_INT4 ) );
  EXPECT_EQ( 4u, GetUniformElementCount( UT_MATRIX_2X2 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Matrix2x3)
{
  EXPECT_EQ( 6u, GetUniformElementCount( UT_MATRIX_2X3 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Matrix2x4_4x2)
{
  EXPECT_EQ( 8u, GetUniformElementCount( UT_MATRIX_2X4 ) );
  EXPECT_EQ( 8u, GetUniformElementCount( UT_MATRIX_4X2 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Matrix3x2)
{
  EXPECT_EQ( 6u, GetUniformElementCount( UT_MATRIX_3X2 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Matrix3x3)
{
  EXPECT_EQ( 9u, GetUniformElementCount( UT_MATRIX_3X3 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Matrix3x4_4x3)
{
  EXPECT_EQ( 12u, GetUniformElementCount( UT_MATRIX_3X4 ) );
  EXPECT_EQ( 12u, GetUniformElementCount( UT_MATRIX_4X3 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Matrix4x4)
{
  EXPECT_EQ( 16u, GetUniformElementCount( UT_MATRIX_4X4 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_SamplerTypes)
{
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLER1D ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLER2D ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLER3D ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLERCUBE ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLER1DSHADOW ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLER2DSHADOW0 ) );
  EXPECT_EQ( 1u, GetUniformElementCount( UT_SAMPLER2DARRAY1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformElementCount_Unknown)
{
  EXPECT_EQ( 1u, GetUniformElementCount( UT_UNKNOWN ) );
}

// ============================================================================
// GetUniformPayloadBytes tests
// ============================================================================

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Float4_ArraySize2)
{
  // 4 elements x 4 bytes x 2 = 32
  EXPECT_EQ( 32u, GetUniformPayloadBytes( UT_FLOAT4, 2 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix4x4_ArraySize1)
{
  // 16 elements x 4 bytes x 1 = 64
  EXPECT_EQ( 64u, GetUniformPayloadBytes( UT_MATRIX_4X4, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Float1_ArraySize3)
{
  // 1 element x 4 bytes x 3 = 12
  EXPECT_EQ( 12u, GetUniformPayloadBytes( UT_FLOAT1, 3 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_ArraySize0)
{
  // arraySize = 0 -> 0 bytes regardless of type
  EXPECT_EQ( 0u, GetUniformPayloadBytes( UT_FLOAT4, 0 ) );
  EXPECT_EQ( 0u, GetUniformPayloadBytes( UT_MATRIX_4X4, 0 ) );
  EXPECT_EQ( 0u, GetUniformPayloadBytes( UT_INT1, 0 ) );
  EXPECT_EQ( 0u, GetUniformPayloadBytes( UT_SAMPLER2D, 0 ) );
  EXPECT_EQ( 0u, GetUniformPayloadBytes( UT_UNKNOWN, 0 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Unknown)
{
  // UT_UNKNOWN: elementCount = 1, so 1 x 4 x arraySize
  EXPECT_EQ( 4u,  GetUniformPayloadBytes( UT_UNKNOWN, 1 ) );
  EXPECT_EQ( 12u, GetUniformPayloadBytes( UT_UNKNOWN, 3 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Sampler)
{
  // Samplers: elementCount = 1, so 1 x 4 x arraySize
  EXPECT_EQ( 4u,  GetUniformPayloadBytes( UT_SAMPLER2D, 1 ) );
  EXPECT_EQ( 8u,  GetUniformPayloadBytes( UT_SAMPLER2D, 2 ) );
  EXPECT_EQ( 4u,  GetUniformPayloadBytes( UT_SAMPLERCUBE, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix2x2)
{
  // 4 elements x 4 bytes x 1 = 16
  EXPECT_EQ( 16u, GetUniformPayloadBytes( UT_MATRIX_2X2, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix3x3)
{
  // 9 elements x 4 bytes x 1 = 36
  EXPECT_EQ( 36u, GetUniformPayloadBytes( UT_MATRIX_3X3, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix2x3)
{
  // 6 elements x 4 bytes x 1 = 24
  EXPECT_EQ( 24u, GetUniformPayloadBytes( UT_MATRIX_2X3, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix2x4)
{
  // 8 elements x 4 bytes x 1 = 32
  EXPECT_EQ( 32u, GetUniformPayloadBytes( UT_MATRIX_2X4, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix3x2)
{
  // 6 elements x 4 bytes x 1 = 24
  EXPECT_EQ( 24u, GetUniformPayloadBytes( UT_MATRIX_3X2, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix3x4)
{
  // 12 elements x 4 bytes x 1 = 48
  EXPECT_EQ( 48u, GetUniformPayloadBytes( UT_MATRIX_3X4, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix4x2)
{
  // 8 elements x 4 bytes x 1 = 32
  EXPECT_EQ( 32u, GetUniformPayloadBytes( UT_MATRIX_4X2, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_Matrix4x3)
{
  // 12 elements x 4 bytes x 1 = 48
  EXPECT_EQ( 48u, GetUniformPayloadBytes( UT_MATRIX_4X3, 1 ) );
}

TEST(UniformTypeHelpersTest, GetUniformPayloadBytes_IntTypes)
{
  EXPECT_EQ( 4u,  GetUniformPayloadBytes( UT_INT1, 1 ) );
  EXPECT_EQ( 8u,  GetUniformPayloadBytes( UT_INT2, 1 ) );
  EXPECT_EQ( 12u, GetUniformPayloadBytes( UT_INT3, 1 ) );
  EXPECT_EQ( 16u, GetUniformPayloadBytes( UT_INT4, 1 ) );
}

// ============================================================================
// constexpr evaluation test (compile-time check)
// ============================================================================

TEST(UniformTypeHelpersTest, ConstexprEvaluation)
{
  constexpr auto cls   = GetUniformBufferClass( UT_FLOAT4 );
  constexpr auto count = GetUniformElementCount( UT_MATRIX_4X4 );
  constexpr auto bytes = GetUniformPayloadBytes( UT_FLOAT4, 2 );

  EXPECT_EQ( UniformBufferClass::Float, cls );
  EXPECT_EQ( 16u, count );
  EXPECT_EQ( 32u, bytes );
}

} // namespace
} // namespace Nebulae
