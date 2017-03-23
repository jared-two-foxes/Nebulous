
#include "UniformParameters.h"

using namespace Nebulae;


UniformParameters::UniformParameters()
{}


const UniformDefinition& 
UniformParameters::GetUniformDefinition( const std::string& name ) const
{  
  static UniformDefinition empty;

  UniformDefinitionMap::const_iterator i = m_definitions.find( name );
  if( i != m_definitions.end() ) {
    // log out to console.
    return i->second;
  }

  return empty;
}


const UniformDefinitionMap& 
UniformParameters::GetUniformDefinitions() const
{ return m_definitions; }


float* 
UniformParameters::GetFloatPointer( std::size_t pos ) 
{ /*_markDirty();*/ return &m_realUniformBuffer[pos]; }

    
const float* 
UniformParameters::GetFloatPointer( std::size_t pos ) const 
{ return &m_realUniformBuffer[pos]; }

    
int* 
UniformParameters::GetIntPointer( std::size_t pos )
{ /*_markDirty();*/ return &m_intUniformBuffer[pos]; }


const int* 
UniformParameters::GetIntPointer( std::size_t pos ) const 
{ return &m_intUniformBuffer[pos]; }


void 
UniformParameters::AddUniformDefinition( const std::string& name, UniformType uniformType, std::size_t arraySize )
{
  // Definition exists if it does exit.
  if( m_definitions.find(name) != m_definitions.end() ) {
    // log out to console.
    return;
  }

  UniformDefinition def;
  def.arraySize = arraySize;
  def.type      = uniformType;
  // for compatibility we do not pad values to multiples of 4
  // when it comes to arrays, user is responsible for creating matching defs
  def.elementSize = UniformDefinition::GetElementSize( uniformType, false );

  // not used
  def.logicalIndex = 0;
  //def.variability = (uint16)GPV_GLOBAL;

  if( def.IsFloat() ) {
    def.physicalIndex = m_realUniformBuffer.size();
    m_realUniformBuffer.resize(m_realUniformBuffer.size() + def.arraySize * def.elementSize);
  } else {
    def.physicalIndex = m_intUniformBuffer.size();
    m_intUniformBuffer.resize(m_intUniformBuffer.size() + def.arraySize * def.elementSize);
  }

  m_definitions[name] = def;
}


void 
UniformParameters::RemoveUniformDefinition( const std::string& name )
{
  UniformDefinitionMap::iterator i = m_definitions.find(name);
  if( i != m_definitions.end() )
  {
    UniformDefinition& def      = i->second;
    bool               isFloat  = def.IsFloat();
    std::size_t        numElems = def.elementSize * def.arraySize;

    for( UniformDefinitionMap::iterator j = m_definitions.begin(); j != m_definitions.end(); ++j ) {
      UniformDefinition& otherDef     = j->second;
      bool               otherIsFloat = otherDef.IsFloat();

      // same type, and comes after in the buffer
      if( ((isFloat && otherIsFloat) || (!isFloat && !otherIsFloat)) && otherDef.physicalIndex > def.physicalIndex ) {
        // adjust index
        otherDef.physicalIndex -= numElems;
      }
    }

    // remove floats and reduce buffer
    if( isFloat ) {
      std::vector<Real >::iterator beg = m_realUniformBuffer.begin();
      std::advance( beg, def.physicalIndex );
      std::vector<Real >::iterator en = beg;
      std::advance( en, numElems );
      m_realUniformBuffer.erase(beg, en);
    } else {
      std::vector<int >::iterator beg = m_intUniformBuffer.begin();
      std::advance( beg, def.physicalIndex );
      std::vector<int >::iterator en = beg;
      std::advance( en, numElems );
      m_intUniformBuffer.erase( beg, en );
    }
  }
}


void 
UniformParameters::RemoveAllUniformDefinitions()
{
  m_definitions.clear();
  m_realUniformBuffer.clear();
  m_intUniformBuffer.clear();
}


void 
UniformParameters::SetNamedUniform( const std::string& name, Real val )
{ SetNamedUniform( name, &val, 1 ); }


void 
UniformParameters::SetNamedUniform( const std::string& name, int val )
{ SetNamedUniform( name, &val, 1 ); }


void 
UniformParameters::SetNamedUniform( const std::string& name, const Vector4& vec )
{ SetNamedUniform( name, vec.ptr(), 4 ); }


void 
UniformParameters::SetNamedUniform( const std::string& name, const Colour& colour )
{ 
  Real buffer[4];
  buffer[0] = colour.r / 255.0f;
  buffer[1] = colour.g / 255.0f;
  buffer[2] = colour.b / 255.0f;
  buffer[3] = colour.a / 255.0f;

  SetNamedUniform( name, &buffer[0], 4 ); 
}

void 
UniformParameters::SetNamedUniform( const std::string& name, const Matrix4& m )
{ /*SetNamedUniform( name, m[0], 16 );*/ }


void 
UniformParameters::SetNamedUniform( const std::string& name, const Matrix4* m, std::size_t numEntries )
{ /*SetNamedUniform( name, m[0][0], 16 * numEntries );*/ }


void 
UniformParameters::SetNamedUniform( const std::string& name, const float *val, std::size_t count )
{
  UniformDefinitionMap::const_iterator i = m_definitions.find(name);
  if( i != m_definitions.end() ) {
    const UniformDefinition& def = i->second;
    memcpy( &m_realUniformBuffer[def.physicalIndex], val, 
      sizeof(float) * std::min(count, def.elementSize * def.arraySize) );
  }

  //_markDirty();
}


void 
UniformParameters::SetNamedUniform( const std::string& name, const double *val, std::size_t count )
{
  UniformDefinitionMap::const_iterator i = m_definitions.find(name);
  if( i != m_definitions.end() ) {
    const UniformDefinition& def = i->second;

    count = std::min(count, def.elementSize * def.arraySize);
    const double* src = val;
    float* dst = &m_realUniformBuffer[def.physicalIndex];
    for( std::size_t v = 0; v < count; ++v ) {
      *dst++ = static_cast<float>(*src++);
    }
  }

  //_markDirty();
}


void 
UniformParameters::SetNamedUniform( const std::string& name, const int *val, std::size_t count )
{
  UniformDefinitionMap::const_iterator i = m_definitions.find(name);
  if( i != m_definitions.end() ) {
    const UniformDefinition& def = i->second;
    memcpy( &m_intUniformBuffer[def.physicalIndex], val, 
      sizeof(int) * std::min(count, def.elementSize * def.arraySize) );
  }

  //_markDirty();
}