#include "ParticleSystem.h"

#include "ParticleEmitter.h"
#include "EmitterSerializer.h"
#include "ParticleGroup.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Beta/Camera/Camera.h>

#include <algorithm>

using namespace Nebulae;

ParticleSystem::ParticleSystem( FileArchivePtr fileSystem, RenderSystemPtr renderer, AtlasManagerPtr atlasManager )
  : m_fileSystem( std::move( fileSystem ) ), m_renderDevice( std::move( renderer ) ),
    m_atlasManager( std::move( atlasManager ) ), m_camera( nullptr )
{
}

ParticleSystem::~ParticleSystem() = default;

void ParticleSystem::Clear()
{
  m_emitters.clear();
  m_groups.clear();
}

void ParticleSystem::Update( const uint64 elapsed )
{
  for ( const auto& emitter : m_emitters )
  {
    emitter->Update( elapsed );
  }
  for ( const auto& group : m_groups )
  {
    group->Update( elapsed );
  }
}

void ParticleSystem::Render()
{
  if ( m_camera == nullptr ) return;

  // Temporary direct path until particle DrawItem emission is implemented in SA-468.
  for ( const auto& group : m_groups )
  {
    if ( group->GetParticleCount() == 0 ) continue;
    group->PreRender( m_camera );
    group->Render( m_camera );
  }
}

ParticleGroup* ParticleSystem::CreateGroup( const std::string& filename )
{
  const auto existing = std::find_if( m_groups.begin(), m_groups.end(), [&]( const auto& group )
                                      { return group->GetName() == filename; } );
  if ( existing != m_groups.end() ) return existing->get();
  if ( !m_fileSystem || !m_renderDevice ) return nullptr;

  std::unique_ptr<File> file( m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, filename ) );
  if ( !file ) return nullptr;

  auto group = std::make_unique<ParticleGroup>( m_renderDevice, m_atlasManager, filename );
  if ( !group->Load( *file ) ) return nullptr;

  ParticleGroup* result = group.get();
  m_groups.push_back( std::move( group ) );
  return result;
}

ParticleEmitter* ParticleSystem::CreateEmitter( const std::string& filename )
{
  if ( !m_fileSystem ) return nullptr;

  std::unique_ptr<File> file( m_fileSystem->Open( NE_DEFAULT_ROOTDEVICE, filename ) );
  if ( !file ) return nullptr;

  auto emitter = std::make_unique<ParticleEmitter>( filename );
  EmitterSerializer serializer;
  if ( !serializer.Load( *file, this, *emitter ) ) return nullptr;

  ParticleEmitter* result = emitter.get();
  m_emitters.push_back( std::move( emitter ) );
  return result;
}

void ParticleSystem::DestroyEmitter( ParticleEmitter* emitter )
{
  const auto it = std::find_if( m_emitters.begin(), m_emitters.end(),
                                [emitter]( const auto& owned ) { return owned.get() == emitter; } );
  if ( it != m_emitters.end() ) m_emitters.erase( it );
}

void ParticleSystem::SetCamera( Camera* camera ) { m_camera = camera; }

Camera* ParticleSystem::GetCamera() const { return m_camera; }
