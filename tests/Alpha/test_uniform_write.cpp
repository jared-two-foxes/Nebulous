#include <gtest/gtest.h>
#include <Nebulae/Alpha/Shaders/UniformWrite.h>

namespace Nebulae {
namespace {

TEST(UniformWriteTest, IsTriviallyCopyable)
{
  EXPECT_TRUE(std::is_trivially_copyable_v<UniformWrite>);
}

TEST(UniformWriteTest, MemberDefaults)
{
  UniformWrite uw{};
  EXPECT_EQ(0, uw.gpuLocation);
  EXPECT_EQ(UT_FLOAT1, uw.type);  // zero-init gives first enum value (0)
  EXPECT_EQ(0, uw.arraySize);
  EXPECT_EQ(0, uw.payloadBytes);
}

TEST(UniformWriteTest, MemberAssignment)
{
  UniformWrite uw;
  uw.gpuLocation = 42;
  uw.type = UT_FLOAT4;
  uw.arraySize = 3;
  uw.payloadBytes = 48;

  EXPECT_EQ(42, uw.gpuLocation);
  EXPECT_EQ(UT_FLOAT4, uw.type);
  EXPECT_EQ(3, uw.arraySize);
  EXPECT_EQ(48, uw.payloadBytes);
}

TEST(UniformWriteTest, SizeIsSane)
{
  EXPECT_GE(sizeof(UniformWrite), 12u);
  EXPECT_LE(sizeof(UniformWrite), 16u);
}

TEST(SamplerWriteTest, IsTriviallyCopyable)
{
  EXPECT_TRUE(std::is_trivially_copyable_v<SamplerWrite>);
}

TEST(SamplerWriteTest, NullTextureAllowed)
{
  SamplerWrite sw;
  sw.gpuLocation = 10;
  sw.unit = 1;
  sw.tex = nullptr;

  EXPECT_EQ(10, sw.gpuLocation);
  EXPECT_EQ(1, sw.unit);
  EXPECT_EQ(nullptr, sw.tex);
}

TEST(SamplerWriteTest, MemberAssignment)
{
  SamplerWrite sw;
  sw.gpuLocation = 5;
  sw.unit = 2;

  EXPECT_EQ(5, sw.gpuLocation);
  EXPECT_EQ(2, sw.unit);
}

TEST(SamplerWriteTest, SizeIsSane)
{
  EXPECT_GE(sizeof(SamplerWrite), 12u);
}

} // namespace
} // namespace Nebulae
