#include <Nebulae/Alpha/RenderStream/RenderStream.h>
#include <Nebulae/Beta/RenderQueue/DrawItem.h>
#include <Nebulae/Beta/RenderQueue/DrawItemList.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

#include "gtest/gtest.h"

using namespace Nebulae;

namespace
{

constexpr std::size_t Align4( std::size_t value ) { return ( value + 3u ) & ~static_cast<std::size_t>( 3u ); }

template <typename T> T MakeTaggedPacket( std::uint16_t type, std::uint8_t fill )
{
  static_assert( std::is_trivially_copyable_v<T> );

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

} // namespace

TEST( DrawItemList, SortPreservesInsertionOrderForEqualSortKeys )
{
  DrawItemList items;
  items.Add( DrawItem{ 4, 20 } );
  items.Add( DrawItem{ 2, 30 } );
  items.Add( DrawItem{ 4, 10 } );

  items.Sort();

  ASSERT_EQ( std::size_t( 3 ), items.Size() );
  EXPECT_EQ( 2, items[0].sortKey );
  EXPECT_EQ( 4, items[1].sortKey );
  EXPECT_EQ( 20, items[1].submissionOrder );
  EXPECT_EQ( 4, items[2].sortKey );
  EXPECT_EQ( 10, items[2].submissionOrder );
}

TEST( RenderStream, MixedPacketsRoundTripWithFourByteAlignedHeaders )
{
  RenderStream stream;
  const PacketSetProgram program = MakeTaggedPacket<PacketSetProgram>( PT_SetProgram, 0x11u );
  const PacketDraw draw = MakeTaggedPacket<PacketDraw>( PT_Draw, 0x22u );

  stream.Write( program );
  stream.Write( draw, 3u );

  std::size_t offset = 0u;
  std::vector<std::uint16_t> types;
  while ( offset < stream.Size() )
  {
    ASSERT_EQ( 0u, offset % 4u );
    const auto* header = reinterpret_cast<const PacketHeader*>( stream.Data() + offset );
    ASSERT_EQ( 0u, static_cast<std::size_t>( header->size ) % 4u );
    ASSERT_GE( header->size, sizeof( PacketHeader ) );
    ASSERT_LE( offset + static_cast<std::size_t>( header->size ), stream.Size() );
    types.push_back( header->type );
    offset += header->size;
  }

  ASSERT_EQ( stream.Size(), offset );
  ASSERT_EQ( std::size_t( 2 ), types.size() );
  EXPECT_EQ( PT_SetProgram, types[0] );
  EXPECT_EQ( PT_Draw, types[1] );
  EXPECT_EQ( Align4( sizeof( PacketSetProgram ) ),
             static_cast<std::size_t>( reinterpret_cast<const PacketHeader*>( stream.Data() )->size ) );
  EXPECT_EQ( Align4( sizeof( PacketDraw ) + 3u ),
             static_cast<std::size_t>(
               reinterpret_cast<const PacketHeader*>( stream.Data() + Align4( sizeof( PacketSetProgram ) ) )->size ) );

  const PacketSetProgram recovered = ReadPacketAt<PacketSetProgram>( stream, 0u );
  EXPECT_EQ( 0, std::memcmp( &program, &recovered, sizeof( PacketSetProgram ) ) );
}
