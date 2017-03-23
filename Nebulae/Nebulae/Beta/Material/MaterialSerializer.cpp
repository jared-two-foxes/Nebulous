
#include "MaterialSerializer.h"
#include "Material.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>


using namespace Nebulae;


UniformType
ConvertStringToUniformType( const std::string& str )
{
  if( str.compare("float") == 0 ) {
    return UT_FLOAT1;
  }
  else if( str.compare("vec2") == 0 ) {
    return UT_FLOAT2;
  }
  else if( str.compare("vec3") == 0 ) {
    return UT_FLOAT3;
  }
  else if( str.compare("vec4") == 0 ) {
    return UT_FLOAT4;
  }
  else if( str.compare("mat2") == 0 ) {
    return UT_MATRIX_2X2;
  }
  else if( str.compare("mat3") == 0 ) {
    return UT_MATRIX_3X3;
  }
  else if( str.compare("mat4") == 0 ) {
    return UT_MATRIX_4X4;
  }
  else if( str.compare("sampler1D") == 0 ) {
    return UT_SAMPLER1D;
  }
  else if( str.compare("sampler2D") == 0 ) {
    return UT_SAMPLER2D;
  }
  else if( str.compare("sampler3D") == 0 ) {
    return UT_SAMPLER3D;
  }
  
  return UT_UNKNOWN;
}


MaterialSerializer::MaterialSerializer( RenderSystemPtr renderDevice )
  : m_renderDevice( renderDevice )
{}


bool
MaterialSerializer::Serialize( File& stream, Material* material )
{ 
  const char* filename = "";

  Json::Value  root;
  if( !ParseJSON(stream, &root) )
  {
    return false;
  }

  Json::Value passes = root["passes"];
  NE_ASSERT( passes.isArray(), "Processing '%s' failed.  Unable to find an array of passes", filename )();
  if( !passes.isArray() ) {
    return false;
  }
  
  Json::ArrayIndex index = 0u;
  while( passes.isValidIndex(index) ) {
    bool success = ProcessPass( passes[index], material );
    NE_ASSERT( success, "Failed to load shaders. Check log for details." )();
    index++;
  }
  
  return true;
}


bool
MaterialSerializer::ProcessPass( Json::Value& passData, Material* material )
{
  Pass* pass    = material->CreatePass();
  bool  success = true;
  
  success |= ProcessShader( passData["vertexshader"], VERTEX_SHADER, material, pass );
  //success |= ProcessShader( passData["geometryShader"], GEOMETRY_SHADER, material, pass );
  success |= ProcessShader( passData["fragmentshader"], PIXEL_SHADER, material, pass );

  return success;
}


bool
MaterialSerializer::ProcessShader( Json::Value& shader, HardwareShaderType shaderType, Material* material, Pass* pass )
{
  const char* name = shader["name"].asCString();
  
  HardwareShader* hardwareShader = m_renderDevice->FindShaderByName( name );
  if( NULL == hardwareShader ) {
    hardwareShader = m_renderDevice->CreateShader( name, shaderType );
  }
  
  if( shaderType == VERTEX_SHADER ) {
    pass->SetVertexShader( hardwareShader );
  } else if( shaderType == PIXEL_SHADER ) {
    pass->SetPixelShader( hardwareShader );
  } else {
    NE_ASSERT( false, "Shader type not yet supported" )();
  }
  
  Json::Value uniforms = shader["uniforms"];
  if( uniforms.isNull() ) return true;
  
  if( !uniforms.isArray() ) {
    //printf( "Found 'uniforms' node does not confirm, expected type array." );
    return false;
  }
  
  Json::ArrayIndex index = 0u;
  while( uniforms.isValidIndex(index) ) {
    Json::Value uniform     = uniforms[index];
    std::size_t count       = 1;
    UniformType uniformType = ConvertStringToUniformType( uniform["type"].asCString() );
    const char* uniformName = uniform["name"].asCString();
    Json::Value countValue  = uniform["count"];
    if( !countValue.isNull() ) count = static_cast<std::size_t>(countValue.asUInt());
    
    material->AddUniformDefinition( uniformName, uniformType, count );
    
    index++;
  }

  return true;
}