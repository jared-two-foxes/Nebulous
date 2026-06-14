#include <gtest/gtest.h>

#include <Nebulae/Alpha/RenderStream/RenderStream.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>
#include <vector>

namespace Nebulae
{
namespace
{

constexpr std::size_t Align4( std::size_t value ) { return ( value + 3u ) & ~static_cast<std::size_t>( 3u ); }

template <typename T> T MakeTaggedPacket( std::uint16_t type, std::uint8_t fill )
{
  static_assert( std::is_trivially_copyable_v<T>, "Packets must be trivially copyable for stream writes." );

  T packet{};
  std::memset( &packet, fill, sizeof( T ) );
  packet.header.type = type;
  packet.header.size = static_cast<std::uint16_t>( sizeof( T ) );
  return packet;
}

template <typename T> T ReadPacketAt( const RenderStream& stream, std::size_t offset )
{
  T packet{};
  std::memcpy( &packet, stream.Data() + offset, sizeof( T ) );
  return packet;
}

template <typename T, typename = void> struct HasReflectProgramMethod : std::false_type
{
};

template <typename T>
struct HasReflectProgramMethod<T, std::void_t<decltype( std::declval<T&>().ReflectProgram(
                                    std::declval<ProgramObject&>(), std::declval<UniformDefinitionMap&>() ) )>>
  : std::true_type
{
};

template <typename T, typename = void> struct HasExecuteStreamMethod : std::false_type
{
};

template <typename T>
struct HasExecuteStreamMethod<
  T, std::void_t<decltype( std::declval<T&>().ExecuteStream( std::declval<const RenderStream&>() ) )>> : std::true_type
{
};

// ---------------------------------------------------------------------------
// Compile-time API / interface checks
// ---------------------------------------------------------------------------

static_assert( std::is_trivially_copyable_v<PacketHeader>, "PacketHeader must be trivially copyable." );
static_assert( std::is_trivially_copyable_v<PacketSetProgram>, "PacketSetProgram must be trivially copyable." );
static_assert( std::is_trivially_copyable_v<PacketSetGeometry>, "PacketSetGeometry must be trivially copyable." );
static_assert( std::is_trivially_copyable_v<PacketSetRenderState>, "PacketSetRenderState must be trivially copyable." );
static_assert( std::is_trivially_copyable_v<PacketSetUniform>, "PacketSetUniform must be trivially copyable." );
static_assert( std::is_trivially_copyable_v<PacketSetSampler>, "PacketSetSampler must be trivially copyable." );
static_assert( std::is_trivially_copyable_v<PacketDraw>, "PacketDraw must be trivially copyable." );

static_assert( offsetof( PacketSetProgram, header ) == 0u, "PacketSetProgram must start with PacketHeader." );
static_assert( offsetof( PacketSetGeometry, header ) == 0u, "PacketSetGeometry must start with PacketHeader." );
static_assert( offsetof( PacketSetRenderState, header ) == 0u, "PacketSetRenderState must start with PacketHeader." );
static_assert( offsetof( PacketSetUniform, header ) == 0u, "PacketSetUniform must start with PacketHeader." );
static_assert( offsetof( PacketSetSampler, header ) == 0u, "PacketSetSampler must start with PacketHeader." );
static_assert( offsetof( PacketDraw, header ) == 0u, "PacketDraw must start with PacketHeader." );

static_assert( HasReflectProgramMethod<RenderSystem>::value,
               "RenderSystem must declare ReflectProgram(ProgramObject&, UniformDefinitionMap&)." );
static_assert( HasExecuteStreamMethod<RenderSystem>::value,
               "RenderSystem must declare ExecuteStream(const RenderStream&)." );

class RenderSystemMissingStreamMethods final : public RenderSystem
{
public:
  RenderSystemMissingStreamMethods() : RenderSystem( nullptr, nullptr ) {}

  const std::string& GetName() const override
  {
    static const std::string kName = "stub";
    return kName;
  }

  void Clear() override {}

protected:
  UniformDefinitionBase GetUniformImpl( const char* ) const override { return UniformDefinitionBase(); }

  void SetUniformImpl( const UniformDefinition<float>&, const float& ) override {}
  void SetUniformImpl( const UniformDefinition<int32>&, const int32& ) override {}
  void SetUniformImpl( const UniformDefinition<Vector2>&, const Vector2& ) override {}
  void SetUniformImpl( const UniformDefinition<Vector4>&, const Vector4& ) override {}
  void SetUniformImpl( const UniformDefinition<Matrix3>&, const Matrix3& ) override {}
  void SetUniformImpl( const UniformDefinition<Matrix4>&, const Matrix4& ) override {}
  void SetUniformImpl( const UniformDefinition<Texture*>&, const Texture* ) override {}
};

// This enforces that RenderSystem adds new pure virtual stream API methods
// (ReflectProgram + ExecuteStream). If they are absent or not pure virtual,
// this class becomes concrete and this assertion fails.
static_assert( std::is_abstract_v<RenderSystemMissingStreamMethods>,
               "RenderSystem must declare stream API methods as pure virtual." );

// ---------------------------------------------------------------------------
// Runtime behavior tests
// ---------------------------------------------------------------------------

TEST( RenderStreamTest, CompilesAndSupportsBasicMethods )
{
  RenderStream stream;

  stream.Clear();
  (void)stream.Data();
  EXPECT_EQ( 0u, stream.Size() );

  PacketSetProgram p = MakeTaggedPacket<PacketSetProgram>( PT_SetProgram, 0x11 );
  stream.Write( p );

  EXPECT_GE( stream.Size(), sizeof( PacketSetProgram ) );
}

TEST( RenderStreamTest, EmptyStreamIterationAfterClearYieldsNoPackets )
{
  RenderStream stream;
  stream.Clear();

  std::size_t offset = 0u;
  std::size_t packetCount = 0u;
  while ( offset < stream.Size() )
  {
    const auto* header = reinterpret_cast<const PacketHeader*>( stream.Data() + offset );
    ++packetCount;
    offset += header->size;
  }

  EXPECT_EQ( 0u, packetCount );
  EXPECT_EQ( 0u, offset );
}

TEST( RenderStreamTest, SinglePacketRoundTripByHeaderWalk )
{
  RenderStream stream;
  const PacketDraw original = MakeTaggedPacket<PacketDraw>( PT_Draw, 0x7Au );

  stream.Write( original );

  ASSERT_GE( stream.Size(), sizeof( PacketDraw ) );

  const auto* header = reinterpret_cast<const PacketHeader*>( stream.Data() );
  ASSERT_EQ( PT_Draw, header->type );
  ASSERT_EQ( Align4( sizeof( PacketDraw ) ), static_cast<std::size_t>( header->size ) );

  const PacketDraw recovered = ReadPacketAt<PacketDraw>( stream, 0u );
  EXPECT_EQ( 0, std::memcmp( &original, &recovered, sizeof( PacketDraw ) ) );
}

TEST( RenderStreamTest, MixedPacketRoundTripAndTypeRecovery )
{
  RenderStream stream;

  const PacketSetProgram p0 = MakeTaggedPacket<PacketSetProgram>( PT_SetProgram, 0x10 );
  const PacketSetGeometry p1 = MakeTaggedPacket<PacketSetGeometry>( PT_SetGeometry, 0x20 );
  const PacketSetRenderState p2 = MakeTaggedPacket<PacketSetRenderState>( PT_SetRenderState, 0x30 );
  const PacketSetUniform p3 = MakeTaggedPacket<PacketSetUniform>( PT_SetUniform, 0x40 );
  const PacketSetSampler p4 = MakeTaggedPacket<PacketSetSampler>( PT_SetSampler, 0x50 );
  const PacketDraw p5 = MakeTaggedPacket<PacketDraw>( PT_Draw, 0x60 );

  stream.Write( p0 );
  stream.Write( p1 );
  stream.Write( p2 );
  stream.Write( p3, 6u ); // extra payload bytes for forward-compat packets
  stream.Write( p4 );
  stream.Write( p5 );

  std::size_t offset = 0u;
  std::vector<std::uint16_t> recoveredTypes;
  std::vector<std::size_t> recoveredSizes;

  while ( offset < stream.Size() )
  {
    const auto* header = reinterpret_cast<const PacketHeader*>( stream.Data() + offset );
    recoveredTypes.push_back( header->type );
    recoveredSizes.push_back( header->size );

    ASSERT_GE( header->size, sizeof( PacketHeader ) );
    ASSERT_EQ( 0u, static_cast<std::size_t>( header->size ) % 4u );
    ASSERT_LE( offset + static_cast<std::size_t>( header->size ), stream.Size() );

    offset += header->size;
  }

  ASSERT_EQ( 6u, recoveredTypes.size() );
  EXPECT_EQ( PT_SetProgram, recoveredTypes[0] );
  EXPECT_EQ( PT_SetGeometry, recoveredTypes[1] );
  EXPECT_EQ( PT_SetRenderState, recoveredTypes[2] );
  EXPECT_EQ( PT_SetUniform, recoveredTypes[3] );
  EXPECT_EQ( PT_SetSampler, recoveredTypes[4] );
  EXPECT_EQ( PT_Draw, recoveredTypes[5] );

  EXPECT_EQ( Align4( sizeof( PacketSetProgram ) ), recoveredSizes[0] );
  EXPECT_EQ( Align4( sizeof( PacketSetGeometry ) ), recoveredSizes[1] );
  EXPECT_EQ( Align4( sizeof( PacketSetRenderState ) ), recoveredSizes[2] );
  EXPECT_EQ( Align4( sizeof( PacketSetUniform ) + 6u ), recoveredSizes[3] );
  EXPECT_EQ( Align4( sizeof( PacketSetSampler ) ), recoveredSizes[4] );
  EXPECT_EQ( Align4( sizeof( PacketDraw ) ), recoveredSizes[5] );

  const PacketSetProgram r0 = ReadPacketAt<PacketSetProgram>( stream, 0u );
  EXPECT_EQ( 0, std::memcmp( &p0, &r0, sizeof( PacketSetProgram ) ) );
}

TEST( RenderStreamTest, AllPacketOffsetsAre4ByteAlignedDuringIteration )
{
  RenderStream stream;

  stream.Write( MakeTaggedPacket<PacketSetProgram>( PT_SetProgram, 0x01 ) );
  stream.Write( MakeTaggedPacket<PacketSetGeometry>( PT_SetGeometry, 0x02 ) );
  stream.Write( MakeTaggedPacket<PacketSetRenderState>( PT_SetRenderState, 0x03 ) );
  stream.Write( MakeTaggedPacket<PacketSetUniform>( PT_SetUniform, 0x04 ), 3u );
  stream.Write( MakeTaggedPacket<PacketSetSampler>( PT_SetSampler, 0x05 ) );
  stream.Write( MakeTaggedPacket<PacketDraw>( PT_Draw, 0x06 ) );

  std::size_t offset = 0u;
  while ( offset < stream.Size() )
  {
    EXPECT_EQ( 0u, offset % 4u ) << "packet offset must remain 4-byte aligned";

    const auto* header = reinterpret_cast<const PacketHeader*>( stream.Data() + offset );
    ASSERT_EQ( 0u, static_cast<std::size_t>( header->size ) % 4u );

    offset += header->size;
  }

  EXPECT_EQ( stream.Size(), offset );
}

TEST( RenderStreamTest, ClearRetainsCapacityByReusingStorage )
{
  RenderStream stream;

  stream.Write( MakeTaggedPacket<PacketSetProgram>( PT_SetProgram, 0xAA ) );
  stream.Write( MakeTaggedPacket<PacketSetGeometry>( PT_SetGeometry, 0xBB ) );
  stream.Write( MakeTaggedPacket<PacketSetRenderState>( PT_SetRenderState, 0xCC ) );

  const std::uint8_t* beforeClearData = stream.Data();
  ASSERT_GT( stream.Size(), 0u );

  stream.Clear();

  EXPECT_EQ( 0u, stream.Size() );
  EXPECT_EQ( beforeClearData, stream.Data() ) << "Clear() should reset size but retain allocated capacity/storage";

  stream.Write( MakeTaggedPacket<PacketDraw>( PT_Draw, 0xDD ) );
  EXPECT_EQ( beforeClearData, stream.Data() ) << "A post-Clear write that fits prior capacity should not reallocate";
}

} // namespace
} // namespace Nebulae
