
#include "ParticleSystem.h"

#include "ParticleEmitter.h"
#include "ParticleGroup.h"
#include "EmitterSerializer.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>

using namespace Nebulae;

ParticleSystem::ParticleSystem( FileArchivePtr fileSystem, RenderSystemPtr renderDevice, AtlasManagerPtr atlasManager )
: m_fileSystem( fileSystem ),
  m_renderDevice( renderDevice ),
  m_atlasManager( atlasManager ),
  m_camera( nullptr )
{}

ParticleSystem::~ParticleSystem()
{
  Clear();
}

void
ParticleSystem::Clear()
{
  for( uint32 i = 0; i < m_groups.size(); ++i ) {
    delete m_groups[i];
  }
  m_groups.clear();

  for( uint32 i = 0; i < m_emitters.size(); ++i ) {
    delete m_emitters[i];
  }
  m_emitters.clear();
}

void 
ParticleSystem::Update( const uint64 elapsed )
{
  // Update the particle groups.
  for( std::size_t i = 0, n = m_groups.size(); i < n; ++i )
  {
    m_groups[i]->Update( elapsed );
  }

  // Update the particle emitters.
  for( std::size_t i = 0, n = m_emitters.size(); i < n; ++i )
  {
    m_emitters[i]->Update( elapsed );
  }
}

void 
ParticleSystem::Render()
{
  PROFILE;

  m_renderDevice->SetDepthTest( false );
  m_renderDevice->SetBlendingState( true );
 
  for( std::size_t i = 0, n = m_groups.size(); i < n; ++i ) 
  {
    m_groups[i]->PreRender( m_camera );
    m_groups[i]->Render( m_camera );
  }

  m_renderDevice->SetBlendingState( false );
  m_renderDevice->SetDepthTest( true );
}

ParticleGroup* 
ParticleSystem::CreateGroup( const std::string& filename )
{ 
  ParticleGroup* group = new ParticleGroup( m_renderDevice, m_atlasManager, filename );
 
  m_groups.push_back( group );

  File* is = m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, filename );
  if( is != NULL )
  {
    group->Load( *is );
  }
  
  return group;
}

ParticleEmitter* 
ParticleSystem::CreateEmitter( const std::string& filename )
{
  ParticleEmitter* emitter = new ParticleEmitter( filename );
  m_emitters.push_back( emitter );
  
  File* is = m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, filename );
  if( is != NULL )
  {
    EmitterSerializer serializer;
    serializer.Load( *is, this, *emitter );
  }

  delete is;

  return emitter;
}

void
ParticleSystem::DestroyEmitter( ParticleEmitter* emitter )
{
  std::vector<ParticleEmitter* >::iterator it = std::find( m_emitters.begin(), m_emitters.end(), emitter );
  if( it != m_emitters.end() )
  {
    m_emitters.erase( it );
    delete emitter;
  }
}

void
ParticleSystem::SetCamera( Camera* camera ) 
{ m_camera = camera; }

Camera*
ParticleSystem::GetCamera() const 
{ return m_camera; }