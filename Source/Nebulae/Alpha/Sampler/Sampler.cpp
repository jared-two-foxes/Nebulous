#include "Sampler.h"

using namespace Nebulae;

//constructor
Sampler::Sampler( const std::string& name, RenderSystem* renderDevice )
	: Resource( name, renderDevice ),
    m_impl( NULL )
{
}

//destructor
Sampler::~Sampler()
{
	if( m_impl ) {
		delete m_impl;
	}  
  m_impl = NULL;
}

void  
Sampler::SetImpl( Impl* impl )
{ m_impl = impl; }


Sampler::Impl* 
Sampler::GetImpl() const
{ return m_impl; }


bool 
Sampler::LoadImpl_( File* is )
{
  (void*)is; //to disable warning.

  if( m_impl ) {
	  return m_impl->Load();
  }
  return false;
}


bool
Sampler::UnloadImpl_()
{
  if( !m_impl ) {
    return  false;
  }

  return m_impl->Unload();
}