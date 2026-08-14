
#include "Material.h"

#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Beta/Material/Pass.h>


using namespace Nebulae;

Material::Material( const std::string& name ) : m_name( name ) {}


Material::~Material()
{
  // Release passes
  for ( auto& pass : m_passes )
  {
    delete pass;
  }
}


const std::string& Material::GetName() const { return m_name; }


std::size_t Material::GetPassCount() const { return m_passes.size(); }


Pass* Material::GetPass( std::size_t index ) const { return m_passes[index]; }


const UniformDefinitionMap& Material::GetUniformDefinitions() const { return m_uniformDefinitions; }


Pass* Material::CreatePass()
{
  m_passes.push_back( new Pass() );
  return m_passes.back();
}

void Material::RemovePass( uint16 index )
{
  if ( index >= m_passes.size() )
  {
    return;
  }

  std::vector<Pass*>::iterator itr = m_passes.begin() + index;
  if ( itr != m_passes.end() )
  {
    m_passes.erase( itr );
  }
}

void Material::RefreshUniformSchemas( RenderSystem* renderSystem )
{
  m_uniformDefinitions.clear();

  for ( auto& pass : m_passes )
  {
    HardwareShader* vs = pass->GetVertexShader();
    HardwareShader* ps = pass->GetPixelShader();

    UniformDefinitionMap passSchema;
    if ( !renderSystem->ReflectProgramShaders( vs, ps, passSchema ) )
    {
      // Program not linked yet; skip this pass.
      continue;
    }

    pass->SetUniformSchema( passSchema );

    // Build merged name->type index (no locations).
    for ( const auto& [name, def] : passSchema )
    {
      auto it = m_uniformDefinitions.find( name );
      if ( it == m_uniformDefinitions.end() )
      {
        // First pass to introduce this uniform - add with no location.
        UniformDefinitionBase merged = def;
        merged.logicalIndex = static_cast<std::size_t>( -1 );
        m_uniformDefinitions[name] = merged;
      }
      else if ( it->second.type != def.type )
      {
        NE_LOG_WARN( "Material", "Uniform '%s' has conflicting types across passes: %d vs %d", name.c_str(),
                     it->second.type, def.type );
      }
    }
  }
}
