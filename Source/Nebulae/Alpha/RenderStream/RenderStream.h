#ifndef NEBULAE_ALPHA_RENDERSTREAM_H__
#define NEBULAE_ALPHA_RENDERSTREAM_H__

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

#include <Nebulae/Alpha/Shaders/UniformWrite.h>


namespace Nebulae
{


// Forward declarations for pointer types used in packets.
class HardwareBuffer;
class HardwareShader;
class InputLayout;


// ---------------------------------------------------------------------------
// Packet type enum
// ---------------------------------------------------------------------------

enum PacketType : std::uint16_t
{
  PT_SetProgram = 0,
  PT_SetGeometry = 1,
  PT_SetRenderState = 2,
  PT_SetUniform = 3,
  PT_SetSampler = 4,
  PT_Draw = 5,
  PT_DrawBatched = 6,      // reserved for future use
  PT_BindUniformRange = 7, // reserved for future use
};


// ---------------------------------------------------------------------------
// Packet header — every packet starts with this
// ---------------------------------------------------------------------------

struct PacketHeader
{
  std::uint16_t type; ///< One of PacketType
  std::uint16_t size; ///< Total packet size in bytes (includes header, 4-byte aligned)
};

static_assert( std::is_trivially_copyable_v<PacketHeader>, "PacketHeader must be trivially copyable." );


// ---------------------------------------------------------------------------
// Packet structs
// ---------------------------------------------------------------------------

struct PacketSetProgram
{
  PacketHeader header;
  HardwareShader* vertexShader;
  HardwareShader* fragmentShader;
};

static_assert( std::is_trivially_copyable_v<PacketSetProgram>, "PacketSetProgram must be trivially copyable." );


struct PacketSetGeometry
{
  PacketHeader header;
  HardwareBuffer* vertexBuffer;
  HardwareBuffer* indexBuffer;
  InputLayout* inputLayout;
  std::size_t stride;
  std::size_t offset;
};

static_assert( std::is_trivially_copyable_v<PacketSetGeometry>, "PacketSetGeometry must be trivially copyable." );


struct PacketSetRenderState
{
  PacketHeader header;
  bool blendingEnabled;
  bool depthTestEnabled;
  float clearColourR;
  float clearColourG;
  float clearColourB;
  float clearColourA;
};

static_assert( std::is_trivially_copyable_v<PacketSetRenderState>, "PacketSetRenderState must be trivially copyable." );


struct PacketSetUniform
{
  PacketHeader header;
  UniformWrite write;
  // Followed in the stream by optional extra payload bytes.
};

static_assert( std::is_trivially_copyable_v<PacketSetUniform>, "PacketSetUniform must be trivially copyable." );


struct PacketSetSampler
{
  PacketHeader header;
  SamplerWrite write;
};

static_assert( std::is_trivially_copyable_v<PacketSetSampler>, "PacketSetSampler must be trivially copyable." );


struct PacketDraw
{
  PacketHeader header;
  std::size_t vertexCount;
  std::size_t startVertex;
};

static_assert( std::is_trivially_copyable_v<PacketDraw>, "PacketDraw must be trivially copyable." );


// ---------------------------------------------------------------------------
// RenderStream — serialises a sequence of render packets into a byte buffer
// ---------------------------------------------------------------------------

class RenderStream
{
public:
  RenderStream() = default;

  /// Reset the stream.  Retains allocated capacity (does not deallocate).
  void Clear();

  /// Raw pointer to the stream data.
  const std::uint8_t* Data() const { return m_data.data(); }

  /// Number of bytes currently in the stream.
  std::size_t Size() const { return m_data.size(); }

  /// Append a packet to the stream.
  /// @param packet     The packet to write (must be trivially copyable).
  /// @param extraPayloadBytes  Extra payload bytes appended after the packet
  ///                           (for forward-compatible extensibility).
  template <typename T> void Write( const T& packet, std::size_t extraPayloadBytes = 0 )
  {
    static_assert( std::is_trivially_copyable_v<T>, "RenderStream::Write requires a trivially copyable packet type." );

    // Total unaligned size = packet struct + extra payload
    const std::size_t unalignedSize = sizeof( T ) + extraPayloadBytes;

    // Round up to 4-byte alignment
    const std::size_t alignedSize = ( unalignedSize + 3u ) & ~static_cast<std::size_t>( 3u );

    const std::size_t oldSize = m_data.size();
    m_data.resize( oldSize + alignedSize );

    // Copy the packet struct into the stream
    std::memcpy( m_data.data() + oldSize, &packet, sizeof( T ) );

    // Zero out any padding / extra payload bytes
    if ( alignedSize > sizeof( T ) )
    {
      std::memset( m_data.data() + oldSize + sizeof( T ), 0, alignedSize - sizeof( T ) );
    }

    // Overwrite the header size field with the aligned total size.
    // Use memcpy instead of reinterpret_cast to avoid undefined behavior
    // from strict-aliasing violations (writing a typed pointer into uint8_t storage).
    const std::uint16_t alignedSize16 = static_cast<std::uint16_t>( alignedSize );
    std::memcpy( m_data.data() + oldSize + offsetof( PacketHeader, size ), &alignedSize16, sizeof( alignedSize16 ) );
  }

private:
  std::vector<std::uint8_t> m_data;
};


} // namespace Nebulae

#endif // NEBULAE_ALPHA_RENDERSTREAM_H__
