#include "quad_resources.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Shaders/UniformWrite.h>
#include <Nebulae/Common/FileSystem/ManifestDiskFileDevice.h>
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>
#include <Nebulae/Common/Window/Window.h>

namespace Nebulae
{

bool InitResources( RenderSystem* renderSystem, QuadResources& res )
{
  // Use a temporary struct to avoid leaving res in a half-initialized state on failure.
  QuadResources tmp = {};

  // Load shaders.
  tmp.vertexShader = renderSystem->CreateShader( "textured_quad_vs.glsl", VERTEX_SHADER );
  if ( !tmp.vertexShader || !tmp.vertexShader->IsLoaded() )
  {
    std::fprintf( stderr, "Failed to create vertex shader.\n" );
    return false;
  }

  tmp.pixelShader = renderSystem->CreateShader( "textured_quad_ps.glsl", PIXEL_SHADER );
  if ( !tmp.pixelShader || !tmp.pixelShader->IsLoaded() )
  {
    std::fprintf( stderr, "Failed to create pixel shader.\n" );
    // Rollback: unload vertex shader and release its impl to prevent leak on retry.
    tmp.vertexShader->Unload();
    delete tmp.vertexShader->GetImpl();
    tmp.vertexShader->SetImpl( nullptr );
    return false;
  }

  // Create vertex buffer: full-screen quad (6 vertices, 2 triangles, non-indexed).
  Vertex vertices[6] = {
    { -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },   { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },  { -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
  };
  tmp.vertexBuffer =
    renderSystem->CreateBuffer( "QuadVertexBuffer", HBU_STATIC_WRITE_ONLY, sizeof( vertices ), HBB_VERTEX, vertices );
  if ( !tmp.vertexBuffer || !tmp.vertexBuffer->IsLoaded() )
  {
    std::fprintf( stderr, "Failed to create vertex buffer.\n" );
    // Rollback: unload shaders and release their impls.
    tmp.pixelShader->Unload();
    delete tmp.pixelShader->GetImpl();
    tmp.pixelShader->SetImpl( nullptr );
    tmp.vertexShader->Unload();
    delete tmp.vertexShader->GetImpl();
    tmp.vertexShader->SetImpl( nullptr );
    return false;
  }

  // Create input layout.
  VertexDeceleration vertexDecl( 2 );
  vertexDecl.AddElement( VET_FLOAT4, VES_POSITION, 0 );
  vertexDecl.AddElement( VET_FLOAT2, VES_TEXTURE_COORDINATES, 0 );
  tmp.inputLayout = renderSystem->CreateInputLayout( "QuadLayout", &vertexDecl, tmp.vertexShader );
  if ( !tmp.inputLayout || !tmp.inputLayout->IsLoaded() )
  {
    std::fprintf( stderr, "Failed to create input layout.\n" );
    // Rollback: unload vertex buffer, shaders, release their impls.
    tmp.vertexBuffer->Unload();
    delete tmp.vertexBuffer->GetImpl();
    tmp.vertexBuffer->SetImpl( nullptr );
    tmp.pixelShader->Unload();
    delete tmp.pixelShader->GetImpl();
    tmp.pixelShader->SetImpl( nullptr );
    tmp.vertexShader->Unload();
    delete tmp.vertexShader->GetImpl();
    tmp.vertexShader->SetImpl( nullptr );
    return false;
  }

  // Create a 2x2 checkerboard texture.
  unsigned char textureData[16] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  };
  tmp.texture = renderSystem->CreateTexture( "Checkerboard", false );
  if ( !tmp.texture )
  {
    std::fprintf( stderr, "Failed to create texture.\n" );
    // Rollback: unload input layout, vertex buffer, shaders, release their impls.
    tmp.inputLayout->Unload();
    delete tmp.inputLayout->GetImpl();
    tmp.inputLayout->SetImpl( nullptr );
    tmp.vertexBuffer->Unload();
    delete tmp.vertexBuffer->GetImpl();
    tmp.vertexBuffer->SetImpl( nullptr );
    tmp.pixelShader->Unload();
    delete tmp.pixelShader->GetImpl();
    tmp.pixelShader->SetImpl( nullptr );
    tmp.vertexShader->Unload();
    delete tmp.vertexShader->GetImpl();
    tmp.vertexShader->SetImpl( nullptr );
    return false;
  }
  if ( !tmp.texture->LoadFromMemory( textureData, 4, 4, 2, 2 ) )
  {
    std::fprintf( stderr, "Failed to load texture from memory.\n" );
    // Rollback: unload texture and all prior resources, release their impls.
    tmp.texture->Unload();
    delete tmp.texture->GetImpl();
    tmp.texture->SetImpl( nullptr );
    tmp.inputLayout->Unload();
    delete tmp.inputLayout->GetImpl();
    tmp.inputLayout->SetImpl( nullptr );
    tmp.vertexBuffer->Unload();
    delete tmp.vertexBuffer->GetImpl();
    tmp.vertexBuffer->SetImpl( nullptr );
    tmp.pixelShader->Unload();
    delete tmp.pixelShader->GetImpl();
    tmp.pixelShader->SetImpl( nullptr );
    tmp.vertexShader->Unload();
    delete tmp.vertexShader->GetImpl();
    tmp.vertexShader->SetImpl( nullptr );
    return false;
  }

  // Bind shaders temporarily to query uniform locations.
  renderSystem->SetShaders( tmp.vertexShader, tmp.pixelShader );

  // Get the uniform location for s_texture.
  tmp.uniformDef = renderSystem->GetUniformByName<Texture*>( "s_texture" );

  // Validate the uniform location before use.
  if ( !tmp.uniformDef.IsValid() )
  {
    std::fprintf( stderr, "Failed to locate uniform 's_texture' in shader program.\n" );
    // Rollback: unload all resources, release their impls.
    tmp.texture->Unload();
    delete tmp.texture->GetImpl();
    tmp.texture->SetImpl( nullptr );
    tmp.inputLayout->Unload();
    delete tmp.inputLayout->GetImpl();
    tmp.inputLayout->SetImpl( nullptr );
    tmp.vertexBuffer->Unload();
    delete tmp.vertexBuffer->GetImpl();
    tmp.vertexBuffer->SetImpl( nullptr );
    tmp.pixelShader->Unload();
    delete tmp.pixelShader->GetImpl();
    tmp.pixelShader->SetImpl( nullptr );
    tmp.vertexShader->Unload();
    delete tmp.vertexShader->GetImpl();
    tmp.vertexShader->SetImpl( nullptr );
    return false;
  }

  // All resources created successfully — commit the temporary struct.
  res = tmp;
  return true;
}

RenderStream BuildQuadStream( const QuadResources& res )
{
  // Enforce preconditions: all resources must be valid and fully initialized.
  NE_ASSERT( res.vertexShader, "BuildQuadStream: vertexShader is null" );
  NE_ASSERT( res.pixelShader, "BuildQuadStream: pixelShader is null" );
  NE_ASSERT( res.vertexBuffer, "BuildQuadStream: vertexBuffer is null" );
  NE_ASSERT( res.inputLayout, "BuildQuadStream: inputLayout is null" );
  NE_ASSERT( res.texture, "BuildQuadStream: texture is null" );
  NE_ASSERT( res.uniformDef.IsValid(), "BuildQuadStream: uniformDef is invalid" );

  const std::size_t stride = sizeof( Vertex );
  const std::size_t offset = 0;

  RenderStream stream;

  // PT_SetProgram
  PacketSetProgram progPacket;
  progPacket.header.type = PT_SetProgram;
  progPacket.vertexShader = res.vertexShader;
  progPacket.fragmentShader = res.pixelShader;
  stream.Write( progPacket );

  // PT_SetGeometry
  PacketSetGeometry geoPacket;
  geoPacket.header.type = PT_SetGeometry;
  geoPacket.vertexBuffer = res.vertexBuffer;
  geoPacket.indexBuffer = nullptr;
  geoPacket.inputLayout = res.inputLayout;
  geoPacket.stride = stride;
  geoPacket.offset = offset;
  stream.Write( geoPacket );

  // PT_SetRenderState
  PacketSetRenderState rsPacket;
  rsPacket.header.type = PT_SetRenderState;
  rsPacket.blendingEnabled = false;
  rsPacket.depthTestEnabled = false;
  rsPacket.clearColourR = 0.0f;
  rsPacket.clearColourG = 0.0f;
  rsPacket.clearColourB = 0.0f;
  rsPacket.clearColourA = 1.0f;
  stream.Write( rsPacket );

  // PT_SetSampler
  PacketSetSampler samplerPacket;
  samplerPacket.header.type = PT_SetSampler;
  samplerPacket.write.gpuLocation = static_cast<int32>( res.uniformDef.logicalIndex );
  samplerPacket.write.unit = 0;
  samplerPacket.write.tex = res.texture;
  stream.Write( samplerPacket );

  // PT_Draw
  PacketDraw drawPacket;
  drawPacket.header.type = PT_Draw;
  drawPacket.vertexCount = 6;
  drawPacket.startVertex = 0;
  stream.Write( drawPacket );

  return stream;
}

void RenderFrame( Platform* platform, RenderSystem* renderSystem, const RenderStream& stream )
{
  while ( !platform->ShouldExit() )
  {
    platform->MessagePump();
    renderSystem->Clear();
    renderSystem->ExecuteStream( stream );
    renderSystem->SwapBuffers();
  }
}

} // namespace Nebulae
