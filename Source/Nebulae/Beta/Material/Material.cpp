
#include "Material.h"

#include <Nebulae/Beta/Material/Pass.h>


using namespace Nebulae;

Material::Material( const std::string& name )
  : m_name( name )
{}


Material::~Material()
{
  // Release passes
  for( std::size_t i = 0, n = m_passes.size(); i<n; ++i ) {
    delete m_passes[i];
  }
}


const std::string&
Material::GetName() const
{ return m_name; }


std::size_t
Material::GetPassCount() const
{ return m_passes.size(); }

  
Pass*
Material::GetPass( std::size_t index ) const
{ return m_passes[index]; }
  

const UniformDefinitionMap& 
Material::GetUniformDefinitions() const
{ return m_uniformDefinitions; }


Pass*
Material::CreatePass()
{
  m_passes.push_back( new Pass() );
  return m_passes.back();
}

void
Material::RemovePass( uint16 index ) 
{
  if( index >= m_passes.size() ) return;

  std::vector<Pass* >::iterator itr = m_passes.begin() + index;
  if( itr != m_passes.end() ) 
  {
    m_passes.erase( itr );
  }
}

void 
Material::AddUniformDefinition( const std::string& name, UniformType type, std::size_t arraySize )
{
  // Definition exists if it does exit.
  if( m_uniformDefinitions.find(name) != m_uniformDefinitions.end() ) {
    // log out to console.
    return;
  }

  UniformDefinition def;
  def.arraySize   = arraySize;
  def.type        = type;
  // for compatibility we do not pad values to multiples of 4
  // when it comes to arrays, user is responsible for creating matching definitions
  def.elementSize = UniformDefinition::GetElementSize( type, false );

  // not used
  def.logicalIndex = 0;
  //def.variability = (uint16)GPV_GLOBAL;

  m_uniformDefinitions[name] = def;
}


void 
Material::RemoveUniformDefinition( const std::string& name )
{
  UniformDefinitionMap::iterator i = m_uniformDefinitions.find(name);
  if( i != m_uniformDefinitions.end() ) {
    m_uniformDefinitions.erase( i );
  }
}


void 
Material::RemoveAllUniformDefinitions()
{ m_uniformDefinitions.clear(); }