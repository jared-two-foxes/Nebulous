#include <Nebulae/Beta/RenderQueue/StreamCompiler.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/Scene/Geometry.h>
#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>

#include "gtest/gtest.h"
#include <cstring>
#include <vector>

using namespace Nebulae;

namespace
{
struct Drawable
{
  HardwareBuffer buffer{ "vb", nullptr };
  InputLayout layout{ "layout", nullptr };
  Geometry geometry;

  Drawable()
  {
    geometry.m_vertexBuffer = &buffer;
    geometry.m_vertexCount = 3;
  }

  void Attach( SceneObject& object, std::size_t slot = 0 )
  {
    object.SetSlotGeometry( slot, &geometry );
    object.SetSlotInputLayout( slot, &layout );
  }
};

std::vector<PacketHeader> Headers( const RenderStream& stream )
{
  std::vector<PacketHeader> headers;
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, stream.Data() + offset, sizeof( header ) );
    EXPECT_GT( header.size, 0 );
    if ( !header.size ) break;
    headers.push_back( header );
    offset += header.size;
  }
  return headers;
}

std::size_t CountPackets( const RenderStream& stream, PacketType type )
{
  std::size_t result = 0;
  for ( const auto& header : Headers( stream ) ) result += header.type == type;
  return result;
}

UniformDefinitionMap Schema()
{
  UniformDefinitionMap schema;
  UniformDefinitionBase def;
  def.type = UT_FLOAT1;
  def.logicalIndex = 3;
  schema["value"] = def;
  return schema;
}

float LastUniform( const RenderStream& stream )
{
  float value = 0;
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, stream.Data() + offset, sizeof( header ) );
    if ( header.type == PT_SetUniform )
      std::memcpy( &value, stream.Data() + offset + sizeof( PacketSetUniform ), sizeof( value ) );
    offset += header.size;
  }
  return value;
}
} // namespace

TEST( StreamCompiler, EmitsCompleteStateForEveryDraw )
{
  Material material( "same" );
  auto* pass = material.CreatePass();
  pass->SetUniformSchema( Schema() );
  SceneObject object( nullptr );
  object.AddSlot( &material );
  Drawable drawable;
  drawable.Attach( object );
  object.AddProvider( "value", []( UniformBinder& b ) { b.Set( "value", 1.0f ); } );
  DrawItemList items;
  object.EmitDrawItems( items, 0, 0 );
  items.Add( items[0] );
  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  EXPECT_EQ( 2u, CountPackets( stream, PT_Draw ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetProgram ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetUniform ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetGeometry ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetRenderState ) );
}

TEST( StreamCompiler, ResolvesProvidersForEveryDraw )
{
  Material material( "dynamic" );
  material.CreatePass()->SetUniformSchema( Schema() );
  SceneObject object( nullptr );
  object.AddSlot( &material );
  Drawable drawable;
  drawable.Attach( object );
  int invocation = 0;
  object.AddProvider( "value", [&invocation]( UniformBinder& b ) { b.Set( "value", static_cast<float>( ++invocation ) ); } );
  DrawItemList items;
  object.EmitDrawItems( items, 0, 0 );
  items.Add( items[0] );

  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  EXPECT_EQ( 2, invocation );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetUniform ) );
  EXPECT_FLOAT_EQ( 2.0f, LastUniform( stream ) );
}

TEST( StreamCompiler, ProgramSwitchRebindsGeometryAndDeeperScopeWins )
{
  Material material( "switch" );
  auto* first = material.CreatePass();
  auto* second = material.CreatePass();
  first->SetUniformSchema( Schema() );
  second->SetUniformSchema( Schema() );
  // Identity is only compared by the compiler; no shader is dereferenced.
  second->SetVertexShader( reinterpret_cast<HardwareShader*>( 1 ) );
  SceneObject object( nullptr );
  object.AddSlot( &material );
  Drawable drawable;
  drawable.Attach( object );
  object.AddProvider( "override", []( UniformBinder& b ) { b.Set( "value", 7.0f ); } );
  DrawItemList items;
  object.EmitDrawItems( items, 0, 0 );
  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  EXPECT_EQ( 2u, CountPackets( stream, PT_Draw ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetProgram ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetUniform ) );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetGeometry ) );
  EXPECT_FLOAT_EQ( 7.0f, LastUniform( stream ) );
}

TEST( StreamCompiler, ObjectScopeOverridesEarlierObject )
{
  Material material( "override" );
  material.CreatePass()->SetUniformSchema( Schema() );
  SceneObject first( nullptr ), second( nullptr );
  first.AddSlot( &material );
  second.AddSlot( &material );
  Drawable drawable;
  drawable.Attach( first );
  drawable.Attach( second );
  first.AddProvider( "value", []( UniformBinder& b ) { b.Set( "value", 1.0f ); } );
  second.AddProvider( "value", []( UniformBinder& b ) { b.Set( "value", 2.0f ); } );
  DrawItemList items;
  first.EmitDrawItems( items, 0, 0 );
  second.EmitDrawItems( items, 0, 0 );
  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  EXPECT_EQ( 2u, CountPackets( stream, PT_SetUniform ) );
  EXPECT_FLOAT_EQ( 2.0f, LastUniform( stream ) );
}

TEST( StreamCompiler, ObjectScopeOverridesNodeWorld )
{
  Material material( "scope" );
  UniformDefinitionMap schema;
  UniformDefinitionBase def;
  def.type = UT_MATRIX_4X4;
  def.logicalIndex = 4;
  schema["world"] = def;
  material.CreatePass()->SetUniformSchema( schema );
  SceneNode node( nullptr );
  SceneObject object( &node );
  object.AddSlot( &material );
  Drawable drawable;
  drawable.Attach( object );
  Matrix4 overrideWorld;
  overrideWorld.SetIdentity();
  overrideWorld[12] = 42.0f;
  object.AddProvider( "world", [overrideWorld]( UniformBinder& b ) { b.Set( "world", overrideWorld ); } );
  DrawItemList items;
  Matrix4 nodeWorld;
  nodeWorld.SetIdentity();
  items.RecordNodeWorld( &node, nodeWorld );
  object.EmitDrawItems( items, 0, 0 );
  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  ASSERT_EQ( 1u, CountPackets( stream, PT_SetUniform ) );
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, stream.Data() + offset, sizeof( header ) );
    if ( header.type == PT_SetUniform )
    {
      Matrix4 emitted;
      std::memcpy( &emitted, stream.Data() + offset + sizeof( PacketSetUniform ), sizeof( emitted ) );
      EXPECT_EQ( overrideWorld, emitted );
    }
    offset += header.size;
  }
}

TEST( SceneTraversal, HiddenParentAndObjectAreCulled )
{
  SceneNode node( nullptr );
  Material material( "visible" );
  material.CreatePass();
  auto* object = node.CreateObject( &material );
  DrawItemList items;
  object->SetVisible( false );
  node.TraverseNode( items );
  EXPECT_EQ( 0u, items.Size() );
  object->SetVisible( true );
  node.SetVisible( false );
  node.TraverseNode( items );
  EXPECT_EQ( 0u, items.Size() );
  node.SetVisible( true );
  node.TraverseNode( items );
  EXPECT_EQ( 1u, items.Size() );
  EXPECT_NE( nullptr, items.GetNodeWorld( &node ) );
}

TEST( StreamCompiler, SkipsSlotsWithoutDrawableGeometry )
{
  Material material( "uninitialized" );
  material.CreatePass();
  SceneObject object( nullptr );
  object.AddSlot( &material );
  DrawItemList items;
  object.EmitDrawItems( items, 0, 0 );

  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  EXPECT_EQ( 0u, stream.Size() );

  Drawable drawable;
  object.SetSlotGeometry( 0, &drawable.geometry );
  compiler.Compile( items, nullptr, stream );
  EXPECT_EQ( 0u, stream.Size() ); // A vertex buffer without an input layout is still unsafe.
}

TEST( StreamCompiler, EmitsIndexedDrawAndPrimitiveTopology )
{
  Material material( "indexed" );
  material.CreatePass();
  SceneObject object( nullptr );
  object.AddSlot( &material );
  Drawable drawable;
  HardwareBuffer indices( "ib", nullptr );
  drawable.geometry.m_indexBuffer = &indices;
  drawable.geometry.m_indexCount = 6;
  drawable.geometry.m_primitiveTopology = OT_TRIANGLE_STRIP;
  drawable.Attach( object );
  DrawItemList items;
  object.EmitDrawItems( items, 0, 0 );

  RenderStream stream;
  StreamCompiler compiler;
  compiler.Compile( items, nullptr, stream );
  ASSERT_EQ( 1u, CountPackets( stream, PT_Draw ) );
  for ( std::size_t offset = 0; offset < stream.Size(); )
  {
    PacketHeader header;
    std::memcpy( &header, stream.Data() + offset, sizeof( header ) );
    if ( header.type == PT_SetGeometry )
    {
      PacketSetGeometry packet;
      std::memcpy( &packet, stream.Data() + offset, sizeof( packet ) );
      EXPECT_EQ( OT_TRIANGLE_STRIP, packet.topology );
    }
    if ( header.type == PT_Draw )
    {
      PacketDraw packet;
      std::memcpy( &packet, stream.Data() + offset, sizeof( packet ) );
      EXPECT_TRUE( packet.indexed );
      EXPECT_EQ( 6u, packet.indexCount );
    }
    offset += header.size;
  }
}

TEST( SceneTraversal, WorldMatrixIncludesRotationAndTranslation )
{
  SceneNode node( nullptr );
  node.SetPosition( Vector4( 5.0f, 6.0f, 0.0f ) );
  node.SetRotation( Quaternion( Vector4( 0.0f, 0.0f, 1.0f ), 1.57079632679f ) );
  DrawItemList items;
  node.TraverseNode( items );
  const Matrix4* world = items.GetNodeWorld( &node );
  ASSERT_NE( nullptr, world );
  EXPECT_NEAR( 0.0f, ( *world )[0], 0.0001f );
  EXPECT_NEAR( -1.0f, ( *world )[1], 0.0001f );
  EXPECT_NEAR( 1.0f, ( *world )[4], 0.0001f );
  EXPECT_NEAR( 5.0f, ( *world )[3], 0.0001f );
  EXPECT_NEAR( 6.0f, ( *world )[7], 0.0001f );
}
