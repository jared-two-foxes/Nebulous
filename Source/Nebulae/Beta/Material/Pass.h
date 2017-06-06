#ifndef __NEBULAE_ALPHA_PASS_H__
#define __NEBULAE_ALPHA_PASS_H__

#include <Nebulae/Common/Common.h>

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>

namespace Nebulae
{

class Pass
{
private:
  HardwareShader* m_vertexShader; ///< Vertex shader which is to be applied for pass.
  HardwareShader* m_pixelShader;  ///< Pixel shader which is to be applied for pass.

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


}; //Pass

} //Nebulae

#endif // __NEBULAE_PASS_H__