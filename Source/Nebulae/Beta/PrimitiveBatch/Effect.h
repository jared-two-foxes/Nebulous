#ifndef NEBULAE_BETA_PRIMITIVEBATCH_EFFECT_H_
#define NEBULAE_BETA_PRIMITIVEBATCH_EFFECT_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class HardwareShader;
class RenderSystem;

class Effect
///
/// Built-in shader /*supports optional texture mapping, vertex coloring, directional lighting*/.
///
{
public:
  typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

private:
  HardwareShader* m_vertexShader;
  HardwareShader* m_pixelShader;

  Matrix4 m_world;
  Matrix4 m_view;
  Matrix4 m_projection;

public:
  Effect();
  ~Effect();

  void Init( RenderSystemPtr renderDevice );
  void Apply( RenderSystemPtr renderDevice );

  const HardwareShader* GetVertexShader() const;
  const HardwareShader* GetPixleShader() const;

  void SetWorld( Matrix4 value );
  void SetView( Matrix4 value );
  void SetProjection( Matrix4 value );
  void SetMatrices( Matrix4 world, Matrix4 view, Matrix4 projection );
};

} // namespace Nebulae

#endif // NEBULAE_BETA_PRIMITIVEBATCH_EFFECT_H_