
#include "ParticleGroup.h"
#include "Particle.h"
#include "ParticleEmitter.h"

#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Alpha/Buffer/HardwareBuffer.h>
#include <Nebulae/Alpha/InputLayout/InputLayout.h>
#include <Nebulae/Alpha/InputLayout/VertexDeceleration.h>
#include <Nebulae/Alpha/RenderSystem/RenderSystem.h>
#include <Nebulae/Alpha/Shaders/HardwareShader.h>
#include <Nebulae/Alpha/Shaders/UniformDefinition.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Alpha/Texture/Texture.h>

#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Material/Material.h>
#include <Nebulae/Beta/Material/Pass.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>


using namespace Nebulae;


ParticleGroup::ParticleGroup( RenderSystemPtr renderDevice, SpriteAtlasManagerPtr atlasManager, const std::string& name,
                              uint32 capacity )
  : m_name( name ),
    m_renderDevice( renderDevice ),
    m_atlasManager( atlasManager ),
    m_pool( capacity ),
    m_template_life(),
    m_template_scale(),
    m_screenAlignment( FacingCameraPosition ),
    m_pMaterial( nullptr ),
    m_pVertexBuffer( nullptr ),
    m_pInputLayout( nullptr ),
    m_pTexture( nullptr )
{
}

ParticleGroup::~ParticleGroup() = default;

bool ParticleGroup::Load( File& is )
{
  NE_ASSERT( m_pMaterial == 0, "Material already initiated." );

  //
  // Read in the material file.
  //
  Json::Value root;
  if ( !ParseJSON( is, &root ) )
  {
    return false;
  }

  const char* texture_name = json_cast_with_default<const char*>( root["texture"], nullptr );
  const char* atlas_name = json_cast_with_default<const char*>( root["atlas"], nullptr );
  const char* frame_name = json_cast_with_default<const char*>( root["frame"], nullptr );
  if ( texture_name == nullptr && ( atlas_name == nullptr || frame_name == nullptr || !m_atlasManager ) )
  {
    NE_LOG_WARN( "ParticleGroup", "Group '%s' needs a texture or an atlas frame", m_name.c_str() );
    return false;
  }


  //
  // Setup the material for rendering.
  //
  HardwareShader* vertexShader = m_renderDevice->CreateShader( "default_particle_vs.glsl", VERTEX_SHADER );
  HardwareShader* pixelShader = m_renderDevice->CreateShader( "default_particle_fs.glsl", PIXEL_SHADER );

  m_pMaterial = new Material( "materials//default_particle.material" );
  Pass* pass = m_pMaterial->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );
  BlendState blendState;
  blendState.isTransparent = true;
  pass->SetBlendState( blendState );

  // StreamCompiler only resolves uniforms present in the pass schema.  Reflection only sees programs the render
  // system has already linked, so link the shader pair first, then install the reflected schema.
  m_renderDevice->SetShaders( vertexShader, pixelShader );
  m_pMaterial->RefreshUniformSchemas( m_renderDevice.get() );
  if ( pass->GetUniformSchema().empty() )
  {
    NE_LOG_WARN( "ParticleGroup", "Group '%s' could not reflect its particle shader uniforms", m_name.c_str() );
    return false;
  }

  m_pVertexBuffer = m_renderDevice->FindBufferByName( "ParticleVertexBuffer" );
  if ( !m_pVertexBuffer )
  {
    static Real s_fRectVertices[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    m_pVertexBuffer = m_renderDevice->CreateBuffer( "ParticleVertexBuffer", HBU_STATIC_WRITE_ONLY, 12 * sizeof( Real ),
                                                    HBB_VERTEX, (void*)&s_fRectVertices[0] );
  }

  // Create the vertex declaration (kept alive for Geometry).
  VertexDeceleration* pVertexDecl = new VertexDeceleration( 2 );
  pVertexDecl->AddElement( VET_FLOAT2, VES_POSITION, 0 );
  m_pVertexDecl.reset( pVertexDecl );

  // Attempt to grab the input layout
  m_pInputLayout = m_renderDevice->FindInputLayoutByUsage( pVertexDecl, vertexShader );
  if ( !m_pInputLayout )
  {
    // Create the InputLayout.
    m_pInputLayout = m_renderDevice->CreateInputLayout( "BasicParticleLayout", pVertexDecl, vertexShader );
  }

  // Load the texture to act as the appearance of the Particle.
  if ( texture_name != nullptr )
  {
    Texture* texture = m_renderDevice->FindTextureByName( texture_name );
    if ( texture == nullptr )
    {
      texture = m_renderDevice->CreateTexture( texture_name );
    }

    NE_ASSERT( texture != nullptr, "Unable to find or create the Texture named '%s'", texture_name );
    m_ownedTexture.reset( new SubTexture( texture, 0, 0, 1.0f, 1.0f ) );
    m_pTexture = m_ownedTexture.get();
  }
  else if ( atlas_name != nullptr )
  {
    SpriteAtlas* atlas = m_atlasManager->GetByName( atlas_name );
    if ( atlas == nullptr )
    {
      atlas = m_atlasManager->Create( atlas_name );
    }

    NE_ASSERT( atlas != nullptr, "Unable to find or create the SpriteAtlas named '%s'", atlas_name );
    if ( atlas != nullptr )
    {
      m_pTexture = atlas->FindModuleSubTexture( frame_name );
      NE_ASSERT( m_pTexture != nullptr, "Unable to find frame named '%s'", frame_name );
    }
  }

  if ( m_pTexture == nullptr )
  {
    return false;
  }

  //
  // Setup template
  //
  m_template_life.SetConstant( json_cast_with_default<Real>( root["life"], 1.0f ) );
  m_template_scale.SetConstant( json_cast_with_default<Real>( root["scale"], 1.0f ) );

  //
  // Setup the shared geometry descriptor and pre-allocate the render pool.
  //
  m_geometry.m_vertexBuffer = m_pVertexBuffer;
  m_geometry.m_vertexDeceleration = m_pVertexDecl.get();
  m_geometry.m_vertexCount = 6;
  m_geometry.m_primitiveTopology = OT_TRIANGLES;

  InitRenderPool();

  return true;
}

void ParticleGroup::InitRenderPool()
{
  const int capacity = m_pool.GetCapacity();
  m_particleWorlds.resize( capacity );

  m_objectPool.reserve( capacity );
  for ( int i = 0; i < capacity; ++i )
  {
    auto obj = std::make_unique<SceneObject>( nullptr );
    std::size_t slotIndex = obj->AddSlot( m_pMaterial );
    obj->SetSlotGeometry( slotIndex, &m_geometry );
    obj->SetSlotInputLayout( slotIndex, m_pInputLayout );

    // Capture stable pointers: m_particleWorlds[i] and m_pTexture never move after InitRenderPool.
    Matrix4* worldPtr = &m_particleWorlds[i];
    const SubTexture* tex = m_pTexture;
    obj->AddProvider( "particle_world", [worldPtr]( UniformBinder& binder ) { binder.Set( "world", *worldPtr ); } );
    obj->AddProvider( "particle_texture",
                      [tex]( UniformBinder& binder )
                      {
                        Real textureWidth = static_cast<Real>( tex->GetWidth() );
                        Vector2 size( textureWidth, static_cast<Real>( tex->GetHeight() ) );
                        Vector2 offset( -0.5f * textureWidth, 0.0f );
                        Vector2 minUv( tex->GetTexCoords()[0], tex->GetTexCoords()[1] );
                        Vector2 maxUv( tex->GetTexCoords()[2], tex->GetTexCoords()[3] );
                        binder.Set( "size", size );
                        binder.Set( "offset", offset );
                        binder.Set( "min_uv", minUv );
                        binder.Set( "max_uv", maxUv );
                        binder.SetTexture( "diffuseTexture", tex->GetTexture(), 0 );
                      } );

    m_objectPool.push_back( std::move( obj ) );
  }
}

std::size_t ParticleGroup::GetParticleCount() const { return m_particles.size(); }

const Particle* ParticleGroup::GetParticle( std::size_t idx ) const
{
  NE_ASSERT( idx < m_particles.size(), "particle buffer overflow." );
  return m_particles[idx];
}

Particle* ParticleGroup::SpawnParticle()
{
  Particle* p = m_pool.fetch();
  if ( p )
  {
    p->m_life = m_template_life.Value();
    Real scale = m_template_scale.Value();
    p->m_scale = Vector4( scale, scale, 0.0f, 0 );
    p->m_age = 0;

    m_particles.push_back( p );
  }

  return p;
}

const char* ParticleGroup::GetName() const { return m_name.c_str(); }

void ParticleGroup::Update( const uint64 elapsed )
{
  float deltaSeconds = float( elapsed ) / 1000000;

  for ( std::vector<Particle*>::iterator it = m_particles.begin(); it != m_particles.end(); )
  {
    // Decrement the life of the particle.
    ( *it )->m_life -= deltaSeconds;

    if ( ( *it )->m_life < 0 )
    {
      //@todo [jared.watt 29.05.2013] Maybe move this into a dying pool rather than deleting immediately?
      m_pool.replace( *it );
      it = m_particles.erase( it );
    }
    else
    {
      // Update the particles velocity (v = u + at).
      ( *it )->m_velocity += ( Vector4( 0, -9.8f, 0 ) * deltaSeconds );

      // Update the position of the particle (s=vt).
      ( *it )->m_position += ( *it )->m_velocity * deltaSeconds;

      ++it;
    }
  }
}

namespace
{
struct FarSortOrder
{
  const Camera* m_camera;

  FarSortOrder( const Camera* camera ) : m_camera( camera ) {}

  bool operator()( const Particle* lhs, const Particle* rhs )
  {
    Vector4 pos = m_camera->GetPosition();
    Vector4 dir = m_camera->GetLookAt() - pos;

    float resl = ( lhs->m_position - pos ).dot( dir );
    float resr = ( rhs->m_position - pos ).dot( dir );
    return resl > resr; // larger view-space depth first, i.e. farthest particle drawn first
  }
};
} // namespace

void ParticleGroup::EmitDrawItems( DrawItemList& items, const Camera* camera )
{
  if ( m_particles.empty() )
    return;

  // Sort far-to-near for correct transparency compositing.
  std::sort( m_particles.begin(), m_particles.end(), FarSortOrder( camera ) );

  for ( std::size_t j = 0; j < m_particles.size(); ++j )
  {
    const Particle& p = *m_particles[j];

    Matrix4 trans = MatrixMakeTranslation( p.m_position.x, p.m_position.y, p.m_position.z );
    Matrix4 scale = MatrixMakeScale( p.m_scale.x, p.m_scale.y, 1.0f );
    Matrix4 world;
    world.SetIdentity();
    world = trans * scale;

    if ( m_screenAlignment != Velocity )
    {
      Matrix4 view = camera->GetViewMatrix();
      view[3] = view[7] = view[11] = 0;
      view.Inverse();
      world *= view;
    }

    m_particleWorlds[j] = world;
    m_objectPool[j]->EmitDrawItems( items, 0, 0 );
  }
}
