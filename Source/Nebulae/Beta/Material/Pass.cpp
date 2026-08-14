
#include "Pass.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>

using namespace Nebulae;


Pass::Pass() : m_vertexShader( nullptr ), m_pixelShader( nullptr ) {}


Pass::~Pass() {}


HardwareShader* Pass::GetVertexShader() const { return m_vertexShader; }


HardwareShader* Pass::GetPixelShader() const { return m_pixelShader; }


void Pass::SetVertexShader( HardwareShader* vertexShader ) { m_vertexShader = vertexShader; }


void Pass::SetPixelShader( HardwareShader* pixelShader ) { m_pixelShader = pixelShader; }

const UniformDefinitionMap& Pass::GetUniformSchema() const { return m_uniformDefinitions; }

void Pass::SetUniformSchema( const UniformDefinitionMap& schema ) { m_uniformDefinitions = schema; }
