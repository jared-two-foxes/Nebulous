#ifndef NEBULAE_ALPHAQUAD_QUAD_RESOURCES_H_
#define NEBULAE_ALPHAQUAD_QUAD_RESOURCES_H_

#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderStream/RenderStream.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/Texture.h>
#include <cstdio>
#include <cstring>

namespace Nebulae
{

class Platform;

struct Vertex
{
  float x, y, z, w;
  float u, v;
};

struct QuadResources
{
  HardwareShader* vertexShader;
  HardwareShader* pixelShader;
  HardwareBuffer* vertexBuffer;
  InputLayout* inputLayout;
  Texture* texture;
  UniformDefinition<Texture*> uniformDef;
};

/// Initialises all rendering resources for the AlphaQuad sample.
/// On failure, the QuadResources struct is left in its original state (unchanged).
/// @return true if all resources were created successfully, false otherwise.
bool InitResources( RenderSystem* renderSystem, QuadResources& res );

/// Builds a render stream that draws a full-screen textured quad.
RenderStream BuildQuadStream( const QuadResources& res );

/// Runs the main render loop.
void RenderFrame( Platform* platform, RenderSystem* renderSystem, const RenderStream& stream );

} // namespace Nebulae

#endif // NEBULAE_ALPHAQUAD_QUAD_RESOURCES_H_
