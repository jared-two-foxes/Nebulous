#include "StreamCompiler.h"

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/Scene/Geometry.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>

#include <map>
#include <limits>
#include <initializer_list>

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
struct Value
{
  UniformType type;
  std::uint16_t count;
  std::vector<std::uint8_t> bytes;
  std::int32_t unit = 0;
  const Texture* texture = nullptr;
  bool sampler = false;
};

using Values = std::map<std::string, Value>;
Values Resolve( std::initializer_list<const UniformBinder*> scopes, const UniformDefinitionMap& schema )
{
  Values values;
  for ( const auto* scope : scopes )
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

void EmitValues( const Values& values, const UniformDefinitionMap& schema, RenderStream& stream )
{
  for ( const auto& [name, value] : values )
  {
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

  for ( std::size_t i = 0; i < items.Size(); ++i )
  {
    const DrawItem& item = items[i];
    if ( !item.object || !item.pass || item.slotIndex >= item.object->GetSlotCount() )
    {
      continue;
    }
    const RenderSlot& slot = item.object->GetSlot( item.slotIndex );
    // The GL interpreter dereferences the vertex buffer and input layout.
    // A slot may have a material before its geometry has been initialized.
    if ( !slot.geometry || !slot.geometry->m_vertexBuffer || !slot.inputLayout ||
         ( slot.geometry->m_indexBuffer ? slot.geometry->m_indexCount == 0 : slot.geometry->m_vertexCount == 0 ) )
    {
      continue;
    }
    UniformBinder nodeScope;
    if ( const Matrix4* world = items.GetNodeWorld( item.node ) )
    {
      nodeScope.Set( "world", *world );
    }
    UniformBinder objectScope;
    for ( const auto& provider : slot.providers )
    {
      provider.second( objectScope );
    }
    PacketSetProgram program{};
    program.header.type = PT_SetProgram;
    program.vertexShader = item.pass->GetVertexShader();
    program.fragmentShader = item.pass->GetPixelShader();
    stream.Write( program );

    const auto& schema = item.pass->GetUniformSchema();
    EmitValues( Resolve( { &scene, &nodeScope, &objectScope }, schema ), schema, stream );

    PacketSetGeometry geometry{};
    geometry.header.type = PT_SetGeometry;
    geometry.inputLayout = slot.inputLayout;
    geometry.vertexBuffer = slot.geometry->m_vertexBuffer;
    geometry.indexBuffer = slot.geometry->m_indexBuffer;
    geometry.stride = slot.geometry->m_vertexDeceleration ? slot.geometry->m_vertexDeceleration->GetVertexSize() : 0;
    geometry.topology = slot.geometry->m_primitiveTopology;
    stream.Write( geometry );

    PacketSetRenderState state{};
    state.header.type = PT_SetRenderState;
    state.blendingEnabled = item.pass->GetBlendState().isTransparent;
    state.depthTestEnabled = true;
    stream.Write( state );

    PacketDraw draw{};
    draw.header.type = PT_Draw;
    draw.indexed = slot.geometry->m_indexBuffer != nullptr;
    if ( draw.indexed )
    {
      draw.indexCount = slot.geometry->m_indexCount;
    }
    else
    {
      draw.vertexCount = slot.geometry->m_vertexCount;
    }
    stream.Write( draw );
  }
}
} // namespace Nebulae
