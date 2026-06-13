#include <gtest/gtest.h>
#include <Nebulae/Alpha/Shaders/UniformBinder.h>

namespace Nebulae {
namespace {

// ============================================================================
// Fixture: provides a schema and a binder for tests that need schema validation
// ============================================================================

class UniformBinderSchemaTest : public ::testing::Test
{
protected:
  UniformDefinitionMap m_schema;
  UniformBinder        m_binder;

  void SetUp() override
  {
    // Populate a simple schema.
    UniformDefinitionBase def;

    def.type        = UT_FLOAT1;
    def.logicalIndex = 0;
    def.elementSize  = 1;
    def.arraySize    = 1;
    m_schema["lightIntensity"] = def;

    def.type        = UT_FLOAT2;
    def.logicalIndex = 1;
    m_schema["uvScale"] = def;

    def.type        = UT_FLOAT4;
    def.logicalIndex = 2;
    m_schema["color"] = def;

    def.type        = UT_INT1;
    def.logicalIndex = 3;
    m_schema["flags"] = def;

    def.type        = UT_MATRIX_3X3;
    def.logicalIndex = 4;
    m_schema["normalMatrix"] = def;

    def.type        = UT_MATRIX_4X4;
    def.logicalIndex = 5;
    m_schema["worldMatrix"] = def;

    def.type        = UT_SAMPLER2D;
    def.logicalIndex = 6;
    m_schema["diffuseTex"] = def;

    m_binder.SetSchema( &m_schema );
  }
};

// ============================================================================
// AC 1: UniformBinder compiles in Nebulae:: namespace
// ============================================================================

TEST(UniformBinderTest, NamespaceIsNebulae)
{
  UniformBinder binder;
  // If this compiles and links, the class exists in Nebulae::.
  SUCCEED();
}

// ============================================================================
// AC 2: SetSchema sets the validation schema
// ============================================================================

TEST_F(UniformBinderSchemaTest, SetSchemaAcceptsMap)
{
  // Schema was set in SetUp. Setting a float that matches should record.
  m_binder.Set<float>( "lightIntensity", 1.5f );
  EXPECT_EQ( 1, m_binder.GetBindings().size() );
}

// ============================================================================
// AC 3: Set<float>, Set<Vector2>, Set<Vector4>, Set<int32>, Set<Matrix3>,
//       Set<Matrix4> each record correct UniformType + payload bytes
// ============================================================================

TEST_F(UniformBinderSchemaTest, SetFloatRecordsFloat1)
{
  m_binder.Set<float>( "lightIntensity", 3.14f );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_FLOAT1, m_binder.GetBindings()[0].type );
  EXPECT_EQ( sizeof(float), m_binder.GetBindings()[0].payload.size() );

  float readBack;
  std::memcpy( &readBack, m_binder.GetBindings()[0].payload.data(), sizeof(float) );
  EXPECT_FLOAT_EQ( 3.14f, readBack );
}

TEST_F(UniformBinderSchemaTest, SetVector2RecordsFloat2)
{
  Vector2 v( 1.0f, 2.0f );
  m_binder.Set<Vector2>( "uvScale", v );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_FLOAT2, m_binder.GetBindings()[0].type );
  EXPECT_EQ( sizeof(Vector2), m_binder.GetBindings()[0].payload.size() );

  Vector2 readBack;
  std::memcpy( &readBack, m_binder.GetBindings()[0].payload.data(), sizeof(Vector2) );
  EXPECT_FLOAT_EQ( 1.0f, readBack.x );
  EXPECT_FLOAT_EQ( 2.0f, readBack.y );
}

TEST_F(UniformBinderSchemaTest, SetVector4RecordsFloat4)
{
  Vector4 v( 0.5f, 0.6f, 0.7f, 1.0f );
  m_binder.Set<Vector4>( "color", v );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_FLOAT4, m_binder.GetBindings()[0].type );
  EXPECT_EQ( sizeof(Vector4), m_binder.GetBindings()[0].payload.size() );

  Vector4 readBack;
  std::memcpy( &readBack, m_binder.GetBindings()[0].payload.data(), sizeof(Vector4) );
  EXPECT_FLOAT_EQ( 0.5f, readBack.x );
  EXPECT_FLOAT_EQ( 0.6f, readBack.y );
  EXPECT_FLOAT_EQ( 0.7f, readBack.z );
  EXPECT_FLOAT_EQ( 1.0f, readBack.w );
}

TEST_F(UniformBinderSchemaTest, SetInt32RecordsInt1)
{
  int32 value = 42;
  m_binder.Set<int32>( "flags", value );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_INT1, m_binder.GetBindings()[0].type );
  EXPECT_EQ( sizeof(int32), m_binder.GetBindings()[0].payload.size() );

  int32 readBack = 0;
  std::memcpy( &readBack, m_binder.GetBindings()[0].payload.data(), sizeof(int32) );
  EXPECT_EQ( 42, readBack );
}

TEST_F(UniformBinderSchemaTest, SetMatrix3RecordsMatrix3x3)
{
  Matrix3 m;
  m.setValue( 1,2,3,4,5,6,7,8,9 );
  m_binder.Set<Matrix3>( "normalMatrix", m );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_MATRIX_3X3, m_binder.GetBindings()[0].type );
  EXPECT_EQ( sizeof(Matrix3), m_binder.GetBindings()[0].payload.size() );
}

TEST_F(UniformBinderSchemaTest, SetMatrix4RecordsMatrix4x4)
{
  Matrix4 m;
  m.SetIdentity();
  m_binder.Set<Matrix4>( "worldMatrix", m );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_MATRIX_4X4, m_binder.GetBindings()[0].type );
  EXPECT_EQ( sizeof(Matrix4), m_binder.GetBindings()[0].payload.size() );
}

// ============================================================================
// AC 4: SetMatrix4 records payloadBytes = 64 (16 floats × 4 bytes)
// ============================================================================

TEST_F(UniformBinderSchemaTest, SetMatrix4PayloadIs64Bytes)
{
  Matrix4 m;
  m.SetIdentity();
  m_binder.Set<Matrix4>( "worldMatrix", m );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( 64u, m_binder.GetBindings()[0].payload.size() );
}

// ============================================================================
// AC 5: SetTexture records a SamplerBinding with correct unit + tex pointer
// ============================================================================

TEST_F(UniformBinderSchemaTest, SetTextureRecordsSamplerBinding)
{
  // We don't need a real Texture* — just test that the pointer is stored.
  const Texture* fakeTex = reinterpret_cast<const Texture*>( 0xDEADBEEF );
  m_binder.SetTexture( "diffuseTex", fakeTex, 3 );

  ASSERT_EQ( 1, m_binder.GetSamplerBindings().size() );
  EXPECT_EQ( "diffuseTex", m_binder.GetSamplerBindings()[0].name );
  EXPECT_EQ( 3, m_binder.GetSamplerBindings()[0].unit );
  EXPECT_EQ( fakeTex, m_binder.GetSamplerBindings()[0].tex );
}

// ============================================================================
// AC 6: SetArray<Matrix4>(name, values, 2) records one binding with
//       arraySize=2 and double payload
// ============================================================================

TEST_F(UniformBinderSchemaTest, SetArrayMatrix4RecordsArraySize2)
{
  Matrix4 mats[2];
  mats[0].SetIdentity();
  mats[1].SetIdentity();

  m_binder.SetArray<Matrix4>( "worldMatrix", mats, 2 );

  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  EXPECT_EQ( UT_MATRIX_4X4, m_binder.GetBindings()[0].type );
  EXPECT_EQ( 2, m_binder.GetBindings()[0].arraySize );
  EXPECT_EQ( 2 * sizeof(Matrix4), m_binder.GetBindings()[0].payload.size() );
  EXPECT_EQ( 128u, m_binder.GetBindings()[0].payload.size() );
}

// ============================================================================
// AC 7: Re-setting same name replaces previous entry (last-write-wins)
// ============================================================================

TEST_F(UniformBinderSchemaTest, ReSetSameNameReplacesInPlace)
{
  m_binder.Set<float>( "lightIntensity", 1.0f );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );

  // Re-set with a different value.
  m_binder.Set<float>( "lightIntensity", 99.0f );
  ASSERT_EQ( 1, m_binder.GetBindings().size() ); // still one entry

  float readBack = 0;
  std::memcpy( &readBack, m_binder.GetBindings()[0].payload.data(), sizeof(float) );
  EXPECT_FLOAT_EQ( 99.0f, readBack );
}

TEST_F(UniformBinderSchemaTest, ReSetSameNameReplacesSampler)
{
  const Texture* tex1 = reinterpret_cast<const Texture*>( 0x1 );
  const Texture* tex2 = reinterpret_cast<const Texture*>( 0x2 );

  m_binder.SetTexture( "diffuseTex", tex1, 0 );
  ASSERT_EQ( 1, m_binder.GetSamplerBindings().size() );

  m_binder.SetTexture( "diffuseTex", tex2, 5 );
  ASSERT_EQ( 1, m_binder.GetSamplerBindings().size() );
  EXPECT_EQ( 5, m_binder.GetSamplerBindings()[0].unit );
  EXPECT_EQ( tex2, m_binder.GetSamplerBindings()[0].tex );
}

// ============================================================================
// AC 8: Unknown name silently skips (no binding recorded, no crash)
// ============================================================================

TEST_F(UniformBinderSchemaTest, UnknownNameSkipsSilently)
{
  // "nonexistent" is not in the schema.
  m_binder.Set<float>( "nonexistent", 1.0f );
  EXPECT_EQ( 0, m_binder.GetBindings().size() );

  // Also test with other types.
  m_binder.Set<Vector4>( "nonexistent", Vector4() );
  EXPECT_EQ( 0, m_binder.GetBindings().size() );

  m_binder.Set<Matrix4>( "nonexistent", Matrix4() );
  EXPECT_EQ( 0, m_binder.GetBindings().size() );
}

// ============================================================================
// AC 9: Type mismatch vs schema silently skips (no binding recorded, no crash)
// ============================================================================

TEST_F(UniformBinderSchemaTest, TypeMismatchSkipsSilently)
{
  // "lightIntensity" is UT_FLOAT1 in schema, but we try to set a Vector4.
  m_binder.Set<Vector4>( "lightIntensity", Vector4() );
  EXPECT_EQ( 0, m_binder.GetBindings().size() );

  // "worldMatrix" is UT_MATRIX_4X4, try setting a float.
  m_binder.Set<float>( "worldMatrix", 1.0f );
  EXPECT_EQ( 0, m_binder.GetBindings().size() );

  // "flags" is UT_INT1, try setting a Matrix3.
  m_binder.Set<Matrix3>( "flags", Matrix3() );
  EXPECT_EQ( 0, m_binder.GetBindings().size() );
}

// ============================================================================
// AC 10: Clear() removes all bindings
// ============================================================================

TEST_F(UniformBinderSchemaTest, ClearRemovesUniformBindings)
{
  m_binder.Set<float>( "lightIntensity", 1.0f );
  m_binder.Set<Vector4>( "color", Vector4() );
  ASSERT_EQ( 2, m_binder.GetBindings().size() );

  m_binder.Clear();
  EXPECT_EQ( 0, m_binder.GetBindings().size() );
}

TEST_F(UniformBinderSchemaTest, ClearRemovesSamplerBindings)
{
  const Texture* tex = reinterpret_cast<const Texture*>( 0x1 );
  m_binder.SetTexture( "diffuseTex", tex, 0 );
  ASSERT_EQ( 1, m_binder.GetSamplerBindings().size() );

  m_binder.Clear();
  EXPECT_EQ( 0, m_binder.GetSamplerBindings().size() );
}

TEST_F(UniformBinderSchemaTest, ClearRemovesBoth)
{
  m_binder.Set<float>( "lightIntensity", 1.0f );
  const Texture* tex = reinterpret_cast<const Texture*>( 0x1 );
  m_binder.SetTexture( "diffuseTex", tex, 0 );
  ASSERT_EQ( 1, m_binder.GetBindings().size() );
  ASSERT_EQ( 1, m_binder.GetSamplerBindings().size() );

  m_binder.Clear();
  EXPECT_EQ( 0, m_binder.GetBindings().size() );
  EXPECT_EQ( 0, m_binder.GetSamplerBindings().size() );
}

// ============================================================================
// Additional edge cases
// ============================================================================

TEST(UniformBinderTest, NoSchemaAllowsAnyName)
{
  UniformBinder binder; // no schema set
  binder.Set<float>( "anything", 1.0f );
  ASSERT_EQ( 1, binder.GetBindings().size() );
  EXPECT_EQ( "anything", binder.GetBindings()[0].name );
}

TEST(UniformBinderTest, MultipleDistinctNames)
{
  UniformBinder binder;
  binder.Set<float>( "a", 1.0f );
  binder.Set<Vector2>( "b", Vector2( 2, 3 ) );
  binder.Set<int32>( "c", 4 );
  ASSERT_EQ( 3, binder.GetBindings().size() );
  EXPECT_EQ( "a", binder.GetBindings()[0].name );
  EXPECT_EQ( "b", binder.GetBindings()[1].name );
  EXPECT_EQ( "c", binder.GetBindings()[2].name );
}

TEST(UniformBinderTest, SetArrayFloatRecordsCorrectPayload)
{
  UniformBinder binder;
  float values[3] = { 1.0f, 2.0f, 3.0f };
  binder.SetArray<float>( "arr", values, 3 );

  ASSERT_EQ( 1, binder.GetBindings().size() );
  EXPECT_EQ( UT_FLOAT1, binder.GetBindings()[0].type );
  EXPECT_EQ( 3, binder.GetBindings()[0].arraySize );
  EXPECT_EQ( 3 * sizeof(float), binder.GetBindings()[0].payload.size() );

  float readBack[3];
  std::memcpy( readBack, binder.GetBindings()[0].payload.data(), sizeof(readBack) );
  EXPECT_FLOAT_EQ( 1.0f, readBack[0] );
  EXPECT_FLOAT_EQ( 2.0f, readBack[1] );
  EXPECT_FLOAT_EQ( 3.0f, readBack[2] );
}

TEST(UniformBinderTest, SetArrayReplacesByName)
{
  UniformBinder binder;
  float vals1[2] = { 1, 2 };
  float vals2[3] = { 10, 20, 30 };

  binder.SetArray<float>( "arr", vals1, 2 );
  ASSERT_EQ( 1, binder.GetBindings().size() );
  EXPECT_EQ( 2, binder.GetBindings()[0].arraySize );

  binder.SetArray<float>( "arr", vals2, 3 );
  ASSERT_EQ( 1, binder.GetBindings().size() );
  EXPECT_EQ( 3, binder.GetBindings()[0].arraySize );
  EXPECT_EQ( 3 * sizeof(float), binder.GetBindings()[0].payload.size() );
}

TEST(UniformBinderTest, SetTextureWithoutSchema)
{
  UniformBinder binder;
  const Texture* tex = reinterpret_cast<const Texture*>( 0x1234 );
  binder.SetTexture( "myTex", tex, 7 );

  ASSERT_EQ( 1, binder.GetSamplerBindings().size() );
  EXPECT_EQ( "myTex", binder.GetSamplerBindings()[0].name );
  EXPECT_EQ( 7, binder.GetSamplerBindings()[0].unit );
  EXPECT_EQ( tex, binder.GetSamplerBindings()[0].tex );
}

TEST(UniformBinderTest, MultipleSamplers)
{
  UniformBinder binder;
  const Texture* tex0 = reinterpret_cast<const Texture*>( 0xA );
  const Texture* tex1 = reinterpret_cast<const Texture*>( 0xB );

  binder.SetTexture( "tex0", tex0, 0 );
  binder.SetTexture( "tex1", tex1, 1 );

  ASSERT_EQ( 2, binder.GetSamplerBindings().size() );
  EXPECT_EQ( 0, binder.GetSamplerBindings()[0].unit );
  EXPECT_EQ( 1, binder.GetSamplerBindings()[1].unit );
}

TEST(UniformBinderTest, ClearEmptyBinderIsSafe)
{
  UniformBinder binder;
  binder.Clear(); // should not crash
  EXPECT_EQ( 0, binder.GetBindings().size() );
  EXPECT_EQ( 0, binder.GetSamplerBindings().size() );
}

TEST(UniformBinderTest, NullSchemaPointerDisablesValidation)
{
  UniformBinder binder;
  UniformDefinitionMap schema;
  UniformDefinitionBase def;
  def.type = UT_FLOAT1;
  schema["exists"] = def;

  binder.SetSchema( &schema );
  binder.Set<float>( "exists", 1.0f );
  ASSERT_EQ( 1, binder.GetBindings().size() );

  // Disable schema by setting nullptr.
  binder.SetSchema( nullptr );
  binder.Set<float>( "anything", 2.0f );
  ASSERT_EQ( 2, binder.GetBindings().size() );
}

} // namespace
} // namespace Nebulae
