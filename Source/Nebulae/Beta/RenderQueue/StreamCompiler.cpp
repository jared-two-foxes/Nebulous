#include "StreamCompiler.h"

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/Scene/Geometry.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>

#include <map>
#include <limits>

namespace Nebulae
{
UniformProvider MakeCameraProvider( const Camera* camera )
{
  return [camera]( UniformBinder& binder ) {
    if ( camera )
    {
      binder.Set( "view", camera->GetViewMatrix() );
      binder.Set( "projection", camera->GetProjectionMatrix() );
    }
  };
}

namespace
{
struct ScopeStack
{
  std::vector<const UniformBinder*> scopes;
  struct Guard
  {
    ScopeStack& stack;
    Guard( ScopeStack& s, const UniformBinder& binder ) : stack( s ) { stack.scopes.push_back( &binder ); }
    ~Guard() { stack.scopes.pop_back(); }
    Guard( const Guard& ) = delete;
    Guard& operator=( const Guard& ) = delete;
  };
};

struct Value
{
  UniformType type;
  std::uint16_t count;
  std::vector<std::uint8_t> bytes;
  std::int32_t unit = 0;
  const Texture* texture = nullptr;
  bool sampler = false;
  bool operator==( const Value& rhs ) const
  {
    return type == rhs.type && count == rhs.count && bytes == rhs.bytes && unit == rhs.unit &&
           texture == rhs.texture && sampler == rhs.sampler;
  }
};

using Values = std::map<std::string, Value>;
Values Resolve( const ScopeStack& stack, const UniformDefinitionMap& schema )
{
  Values values;
  for ( const auto* scope : stack.scopes )
  {
    for ( const auto& binding : scope->GetBindings() )
    {
      auto def = schema.find( binding.name );
      if ( def != schema.end() && def->second.type == binding.type )
      {
        values[binding.name] = { binding.type, binding.arraySize, binding.payload };
      }
    }
    for ( const auto& binding : scope->GetSamplerBindings() )
    {
      auto def = schema.find( binding.name );
      if ( def != schema.end() )
      {
        Value value{};
        value.type = def->second.type;
        value.count = 1;
        value.unit = binding.unit;
        value.texture = binding.tex;
        value.sampler = true;
        values[binding.name] = std::move( value );
      }
    }
  }
  return values;
}

void EmitValues( const Values& values, const Values& previous, bool programChanged,
                 const UniformDefinitionMap& schema, RenderStream& stream )
{
  for ( const auto& [name, value] : values )
  {
    auto old = previous.find( name );
    if ( !programChanged && old != previous.end() && old->second == value )
    {
      continue;
    }
    const auto& definition = schema.at( name );
    if ( !definition.IsValid() || definition.logicalIndex > static_cast<std::size_t>( std::numeric_limits<std::int32_t>::max() ) )
    {
      continue;
    }
    const auto location = static_cast<std::int32_t>( definition.logicalIndex );
    if ( value.sampler )
    {
      PacketSetSampler packet{};
      packet.header.type = PT_SetSampler;
      packet.write = { location, value.unit, value.texture };
      stream.Write( packet );
    }
    else
    {
      PacketSetUniform packet{};
      packet.header.type = PT_SetUniform;
      packet.write = { location, value.type, value.count, static_cast<std::uint16_t>( value.bytes.size() ) };
      stream.WritePayload( packet, value.bytes.data(), value.bytes.size() );
    }
  }
}
} // namespace

void StreamCompiler::Compile( DrawItemList items, const Camera* camera, RenderStream& stream )
{
  stream.Clear();
  items.Sort();
  UniformBinder scene;
  MakeCameraProvider( camera )( scene );
  ScopeStack stack;
  ScopeStack::Guard sceneGuard( stack, scene );
  HardwareShader* vertexShader = nullptr;
  HardwareShader* fragmentShader = nullptr;
  bool hasProgram = false;
  Values previous;
  const SceneNode* previousNode = nullptr;
  const SceneObject* previousObject = nullptr;
  std::size_t previousSlot = 0;
  UniformBinder nodeScope;
  UniformBinder objectScope;
  const Geometry* previousGeometry = nullptr;
  InputLayout* previousLayout = nullptr;
  const Pass* previousStatePass = nullptr;
  bool hasGeometry = false;
  bool hasState = false;

  for ( std::size_t i = 0; i < items.Size(); ++i )
  {
    const DrawItem& item = items[i];
    if ( !item.object || !item.pass || item.slotIndex >= item.object->GetSlotCount() )
    {
      continue;
    }
    const RenderSlot& slot = item.object->GetSlot( item.slotIndex );
    if ( item.node != previousNode )
    {
      nodeScope.Clear();
      if ( const Matrix4* world = items.GetNodeWorld( item.node ) )
      {
        nodeScope.Set( "world", *world );
      }
      previousNode = item.node;
    }
    if ( item.object != previousObject || item.slotIndex != previousSlot )
    {
      objectScope.Clear();
      for ( const auto& provider : slot.providers )
      {
        provider.second( objectScope );
      }
      previousObject = item.object;
      previousSlot = item.slotIndex;
    }
    ScopeStack::Guard nodeGuard( stack, nodeScope );
    ScopeStack::Guard objectGuard( stack, objectScope );
    bool programChanged = !hasProgram || vertexShader != item.pass->GetVertexShader() ||
                          fragmentShader != item.pass->GetPixelShader();
    if ( programChanged )
    {
      vertexShader = item.pass->GetVertexShader();
      fragmentShader = item.pass->GetPixelShader();
      PacketSetProgram packet{};
      packet.header.type = PT_SetProgram;
      packet.vertexShader = vertexShader;
      packet.fragmentShader = fragmentShader;
      stream.Write( packet );
      hasProgram = true;
    }
    const auto& schema = item.pass->GetUniformSchema();
    Values values = Resolve( stack, schema );
    EmitValues( values, previous, programChanged, schema, stream );
    previous = std::move( values );

    if ( !hasGeometry || previousGeometry != slot.geometry || previousLayout != slot.inputLayout )
    {
      PacketSetGeometry packet{};
      packet.header.type = PT_SetGeometry;
      packet.inputLayout = slot.inputLayout;
      if ( slot.geometry )
      {
        packet.vertexBuffer = slot.geometry->m_vertexBuffer;
        packet.indexBuffer = slot.geometry->m_indexBuffer;
        packet.stride = slot.geometry->m_vertexDeceleration ? slot.geometry->m_vertexDeceleration->GetVertexSize() : 0;
      }
      stream.Write( packet );
      previousGeometry = slot.geometry;
      previousLayout = slot.inputLayout;
      hasGeometry = true;
    }
    if ( !hasState || previousStatePass->GetBlendState().isTransparent != item.pass->GetBlendState().isTransparent )
    {
      PacketSetRenderState packet{};
      packet.header.type = PT_SetRenderState;
      packet.blendingEnabled = item.pass->GetBlendState().isTransparent;
      packet.depthTestEnabled = true;
      stream.Write( packet );
      previousStatePass = item.pass;
      hasState = true;
    }
    PacketDraw draw{};
    draw.header.type = PT_Draw;
    draw.vertexCount = slot.geometry ? slot.geometry->m_vertexCount : 0;
    stream.Write( draw );
  }
}
} // namespace Nebulae
