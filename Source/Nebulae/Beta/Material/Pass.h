#ifndef NEBULAE_BETA_MATERIAL_PASS_H_
#define NEBULAE_BETA_MATERIAL_PASS_H_

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>

namespace Nebulae
{

struct BlendState
{
  bool isTransparent = false;
};

class Pass
{
private:
  HardwareShader* m_vertexShader;            ///< Vertex shader which is to be applied for pass.
  HardwareShader* m_pixelShader;             ///< Pixel shader which is to be applied for pass.
  UniformDefinitionMap m_uniformDefinitions; ///< List of all of the uniforms registered for this pass.
  BlendState m_blendState;                   ///< Blend state for this pass.

public:
  Pass();
  virtual ~Pass();

  /** Returns a pointer to the hardware specific vertex shader. */
  HardwareShader* GetVertexShader() const;

  /** Returns a pointer to the hardware specific pixel shader. */
  HardwareShader* GetPixelShader() const;

  /** Sets the HardwareShader that is to be used in the vertex portion of the rendering emulation. */
  void SetVertexShader( HardwareShader* vertexShader );

  /** Sets the HardwareShader that is to be used in the pixel/fragment portion of the rendering emulation. */
  void SetPixelShader( HardwareShader* pixelShader );

  const UniformDefinitionMap& GetUniformSchema() const;
  void SetUniformSchema( const UniformDefinitionMap& schema );

  const BlendState& GetBlendState() const;
  void SetBlendState( const BlendState& blendState );

}; // Pass

} // namespace Nebulae

#endif // NEBULAE_BETA_MATERIAL_PASS_H_
