#ifndef NEBULAE_ALPHA_UNIFORMBINDER_H__
#define NEBULAE_ALPHA_UNIFORMBINDER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Shaders/UniformType.h>
#include <Nebulae/Alpha/Shaders/UniformTypeTraits.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace Nebulae
{

/// A single uniform binding: name, type, array size, and raw payload bytes.
struct UniformBinding
{
  std::string name;
  UniformType type;
  std::uint16_t arraySize;
  std::vector<std::uint8_t> payload;

  UniformBinding() : type( UT_UNKNOWN ), arraySize( 0 ) {}

  UniformBinding( const std::string& name_, UniformType type_, std::uint16_t arraySize_,
                  const std::vector<std::uint8_t>& payload_ )
    : name( name_ ), type( type_ ), arraySize( arraySize_ ), payload( payload_ )
  {
  }
};

/// A single sampler binding: name, texture unit, and texture pointer.
struct SamplerBinding
{
  std::string name;
  std::int32_t unit;
  const Texture* tex;

  SamplerBinding() : unit( 0 ), tex( nullptr ) {}

  SamplerBinding( const std::string& name_, std::int32_t unit_, const Texture* tex_ )
    : name( name_ ), unit( unit_ ), tex( tex_ )
  {
  }
};

/** UniformBinder
 *
 *  Collects uniform and sampler bindings for a single draw call scope.
 *  Bindings are stored as name + type + payload bytes (not as UniformWrite,
 *  because gpuLocation is resolved later by StreamCompiler).
 *
 *  Schema validation: if a schema (UniformDefinitionMap) is provided via
 *  SetSchema(), then Set<T>() will only record bindings for names that exist
 *  in the schema AND whose type matches the schema's expected type.
 *  Unknown names and type mismatches are silently skipped.
 *
 *  Last-write-wins: re-setting the same name replaces the previous entry
 *  in-place rather than appending.
 */
class UniformBinder
{
public:
  UniformBinder() : m_schema( nullptr ) {}

  /// Provide an optional validation schema. Pass nullptr to disable validation.
  void SetSchema( const UniformDefinitionMap* schema ) { m_schema = schema; }

  /// Set a uniform value. Records one binding with arraySize=1.
  template <typename T> void Set( const std::string& name, const T& value )
  {
    constexpr UniformType type = UniformTypeTraits<T>::value;

    if ( !Validate( name, type ) )
      return;

    const std::uint16_t arraySize = 1;
    const std::uint16_t payloadBytes = static_cast<std::uint16_t>( sizeof( T ) );
    std::vector<std::uint8_t> payload( payloadBytes );
    std::memcpy( payload.data(), &value, payloadBytes );

    UpsertUniform( name, type, arraySize, std::move( payload ) );
  }

  /// Set an array of uniform values. Records ONE binding with arraySize = count.
  template <typename T> void SetArray( const std::string& name, const T* values, std::uint16_t count )
  {
    constexpr UniformType type = UniformTypeTraits<T>::value;

    if ( !Validate( name, type ) )
      return;

    const std::uint16_t payloadBytes = static_cast<std::uint16_t>( count * sizeof( T ) );
    std::vector<std::uint8_t> payload( payloadBytes );
    std::memcpy( payload.data(), values, payloadBytes );

    UpsertUniform( name, type, count, std::move( payload ) );
  }

  /// Set a texture sampler binding.
  void SetTexture( const std::string& name, const Texture* tex, std::int32_t unit )
  {
    // Sampler bindings are not validated against the uniform schema.
    UpsertSampler( name, unit, tex );
  }

  /// Remove all bindings.
  void Clear()
  {
    m_bindings.clear();
    m_samplerBindings.clear();
  }

  /// Accessors for inspection / streaming.
  const std::vector<UniformBinding>& GetBindings() const { return m_bindings; }
  const std::vector<SamplerBinding>& GetSamplerBindings() const { return m_samplerBindings; }

private:
  /// Validate name + type against the schema (if set).
  bool Validate( const std::string& name, UniformType type ) const
  {
    if ( m_schema == nullptr )
      return true;

    auto it = m_schema->find( name );
    if ( it == m_schema->end() )
      return false; // unknown name

    if ( it->second.type != type )
      return false; // type mismatch

    return true;
  }

  /// Insert or replace a uniform binding by name (last-write-wins).
  void UpsertUniform( const std::string& name, UniformType type, std::uint16_t arraySize,
                      std::vector<std::uint8_t>&& payload )
  {
    for ( auto& b : m_bindings )
    {
      if ( b.name == name )
      {
        b.type = type;
        b.arraySize = arraySize;
        b.payload = std::move( payload );
        return;
      }
    }

    m_bindings.emplace_back( name, type, arraySize, std::move( payload ) );
  }

  /// Insert or replace a sampler binding by name (last-write-wins).
  void UpsertSampler( const std::string& name, std::int32_t unit, const Texture* tex )
  {
    for ( auto& b : m_samplerBindings )
    {
      if ( b.name == name )
      {
        b.unit = unit;
        b.tex = tex;
        return;
      }
    }

    m_samplerBindings.emplace_back( name, unit, tex );
  }

  const UniformDefinitionMap* m_schema;
  std::vector<UniformBinding> m_bindings;
  std::vector<SamplerBinding> m_samplerBindings;
};

} // namespace Nebulae

#endif // NEBULAE_ALPHA_UNIFORMBINDER_H__
