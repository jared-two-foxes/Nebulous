#ifndef NEBULAE_ALPHA_SHADERS_UNIFORMWRITE_H_
#define NEBULAE_ALPHA_SHADERS_UNIFORMWRITE_H_

#include <Nebulae/Alpha/Shaders/UniformType.h>
#include <cstdint>
#include <type_traits>

namespace Nebulae
{

class Texture;

struct UniformWrite
{
  std::int32_t gpuLocation;
  UniformType type;
  std::uint16_t arraySize;
  std::uint16_t payloadBytes;
};

static_assert( std::is_trivially_copyable_v<UniformWrite>,
               "UniformWrite must be trivially copyable for memcpy-safe streaming." );

struct SamplerWrite
{
  std::int32_t gpuLocation;
  std::int32_t unit;
  const Texture* tex;
};

static_assert( std::is_trivially_copyable_v<SamplerWrite>,
               "SamplerWrite must be trivially copyable for memcpy-safe streaming." );

} // namespace Nebulae

#endif // NEBULAE_ALPHA_SHADERS_UNIFORMWRITE_H_
