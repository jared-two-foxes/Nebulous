
#include "MaterialSerializer.h"
#include "Material.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>


using namespace Nebulae;


MaterialSerializer::MaterialSerializer( RenderSystemPtr renderDevice ) : m_renderDevice( renderDevice ) {}


bool MaterialSerializer::Serialize( File& stream, Material* material )
{
  const char* filename = "";

  Json::Value root;
  if ( !ParseJSON( stream, &root ) )
  {
    return false;
  }

  Json::Value passes = root["passes"];
  NE_ASSERT( passes.isArray(), "Processing '%s' failed.  Unable to find an array of passes", filename );
  if ( !passes.isArray() )
  {
    return false;
  }

  Json::ArrayIndex index = 0u;
  while ( passes.isValidIndex( index ) )
  {
    bool success = ProcessPass( passes[index], material );
    NE_ASSERT( success, "Failed to load shaders. Check log for details." );
    index++;
  }

  // Reflect uniform schemas from each pass's program.
  material->RefreshUniformSchemas( m_renderDevice.get() );

  return true;
}


bool MaterialSerializer::ProcessPass( Json::Value& passData, Material* material )
{
  Pass* pass = material->CreatePass();
  bool success = true;

  // Warn about deprecated pass-level "uniforms" block.
  if ( !passData["uniforms"].isNull() )
  {
    NE_LOG_WARN( "Material", "Deprecated 'uniforms' block found in material file; "
                             "uniforms are now auto-reflected from shaders." );
  }

  success |= ProcessShader( passData["vertexshader"], VERTEX_SHADER, material, pass );
  // success |= ProcessShader( passData["geometryShader"], GEOMETRY_SHADER, material, pass );
  success |= ProcessShader( passData["fragmentshader"], PIXEL_SHADER, material, pass );

  return success;
}


bool MaterialSerializer::ProcessShader( Json::Value& shader, HardwareShaderType shaderType, Material* material,
                                        Pass* pass )
{
  const char* name = shader["name"].asCString();

  HardwareShader* hardwareShader = m_renderDevice->FindShaderByName( name );
  if ( nullptr == hardwareShader )
  {
    hardwareShader = m_renderDevice->CreateShader( name, shaderType );
  }

  if ( shaderType == VERTEX_SHADER )
  {
    pass->SetVertexShader( hardwareShader );
  }
  else if ( shaderType == PIXEL_SHADER )
  {
    pass->SetPixelShader( hardwareShader );
  }
  else
  {
    NE_ASSERT( false, "Shader type not yet supported" );
  }

  // Warn about deprecated "uniforms" block (auto-reflected since Phase 3.1).
  if ( !shader["uniforms"].isNull() )
  {
    NE_LOG_WARN( "Material", "Deprecated 'uniforms' block found in material file; "
                             "uniforms are now auto-reflected from shaders." );
  }

  return true;
}
