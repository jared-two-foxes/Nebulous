
#include "Pass.h"

#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

using namespace Nebulae;


Pass::Pass()
  : m_vertexShader( NULL ),
    m_pixelShader( NULL )
{
}


Pass::~Pass()
{
}


HardwareShader*    
Pass::GetVertexShader() const
{ return m_vertexShader; }
    

HardwareShader*    
Pass::GetPixelShader() const
{ return m_pixelShader; }


void 
Pass::SetVertexShader( HardwareShader* vertexShader )
{ m_vertexShader = vertexShader; }


void 
Pass::SetPixelShader( HardwareShader* pixelShader )
{ m_pixelShader = pixelShader; }
