#include "Sampler.h"

using namespace Nebulae;

// constructor
Sampler::Sampler( const std::string& name, RenderSystem* renderDevice ) : Resource( name, renderDevice ) {}

// destructor
Sampler::~Sampler()
{
  if ( m_impl != nullptr )
  {
    delete m_impl;
  }
  m_impl = NULL;
}

void Sampler::SetImpl( Impl* impl ) { m_impl = impl; }


Sampler::Impl* Sampler::GetImpl() const { return m_impl; }


bool Sampler::LoadImpl_( File* is )
{
  (void*)is; // to disable warning.

  if ( m_impl != nullptr )
  {
    return m_impl->Load();
  }
  return false;
}


bool Sampler::UnloadImpl_()
{
  if ( m_impl == nullptr )
  {
    return false;
  }

  return m_impl->Unload();
}