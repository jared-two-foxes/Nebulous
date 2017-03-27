
#include "Particle.h"
#include "ParticleGroup.h"
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
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>


using namespace Nebulae;


ParticleGroup::ParticleGroup( RenderSystemPtr renderDevice, SpriteAtlasManagerPtr atlasManager, const std::string& name, uint32 capacity )
: m_name(name),
  m_renderDevice(renderDevice),
  m_atlasManager( atlasManager ),
  m_pool(capacity),
  m_template_life(),
  m_template_scale(),
  m_screenAlignment(FacingCameraPosition),
  m_pMaterial(nullptr),
  m_pVertexBuffer(nullptr),
  m_pInputLayout(nullptr),
  m_pTexture(nullptr)
{}

ParticleGroup::~ParticleGroup()
{
  if( m_pTexture != nullptr )
  {
    delete m_pTexture;
    m_pTexture = nullptr;
  }
}

bool
ParticleGroup::Load( File& is )
{
  NE_ASSERT( m_pMaterial == 0, "Material already initiated." )();
  
//
// Read in the material file.
//
  Json::Value root;
  if( !ParseJSON(is, &root) )
  {
    return false;
  }


//
// Setup the material for rendering.  
//
  HardwareShader* vertexShader = m_renderDevice->CreateShader( "default_particle_vs.glsl", VERTEX_SHADER );
  HardwareShader* pixelShader  = m_renderDevice->CreateShader( "default_particle_fs.glsl", PIXEL_SHADER );

  m_pMaterial = new Material( "materials//default_particle.material" ); 
  Pass* pass = m_pMaterial->CreatePass();
  pass->SetVertexShader( vertexShader );
  pass->SetPixelShader( pixelShader );

  m_pVertexBuffer = m_renderDevice->FindBufferByName( "ParticleVertexBuffer" );
  if( !m_pVertexBuffer ) {
    static Real s_fRectVertices[] = { 0.0f,0.0f, 1.0f,0.0f, 0.0,1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f };
    m_pVertexBuffer = m_renderDevice->CreateBuffer( "ParticleVertexBuffer", HBU_STATIC_WRITE_ONLY, 12*sizeof(Real),
      HBB_VERTEX, (void*)&s_fRectVertices[0] );
  }

  // Create the vertex deceleration.
  VertexDeceleration* pVertexDecl = new VertexDeceleration( 2 );
  pVertexDecl->AddElement( VET_FLOAT2, VES_POSITION, 0 );

  // Attempt to grab the input layout
  m_pInputLayout = m_renderDevice->FindInputLayoutByUsage( pVertexDecl, vertexShader );
  if( !m_pInputLayout ) {

    // Create the InputLayout.
    m_pInputLayout = m_renderDevice->CreateInputLayout( "BasicParticleLayout", pVertexDecl, vertexShader );
  }

  delete pVertexDecl;

  const char* texture_name = json_cast_with_default<const char* >( root["texture"], nullptr );
  const char* atlas_name   = json_cast_with_default<const char* >( root["atlas"], nullptr );
  const char* frame_name   = json_cast_with_default<const char* >( root["frame"], nullptr );
  
  // Load the texture to act as the appearance of the Particle.
  if( texture_name != nullptr )
  {
    Texture* texture = m_renderDevice->FindTextureByName( texture_name );
    if( texture == nullptr ) 
    {
      texture = m_renderDevice->CreateTexture( texture_name );
    }

    NE_ASSERT( texture != nullptr, "Unable to find or create the Texture named '%s'", texture_name )();
    m_pTexture = new SubTexture( texture, 0, 0, 1.0f, 1.0f );
  }
  else if( atlas_name != nullptr )
  {
    SpriteAtlas* atlas = m_atlasManager->GetByName( atlas_name );
    if( atlas == nullptr )
    {
      atlas = m_atlasManager->Create( atlas_name );
    }

    NE_ASSERT( atlas != nullptr, "Unable to find or create the SpriteAtlas named '%s'", atlas_name )();
    if( atlas != nullptr )
    {
      m_pTexture = atlas->FindModuleSubTexture( frame_name );  
      NE_ASSERT( m_pTexture != nullptr, "Unable to find frame named '%s'", frame_name )( atlas_name );
    }
  }


//
// Setup template
//
  m_template_life.SetConstant( json_cast_with_default<Real >( root["life"], 1.0f ) );
  m_template_scale.SetConstant( json_cast_with_default<Real >( root["scale"], 1.0f ) );
  
  return true;
}

std::size_t
ParticleGroup::GetParticleCount() const
{
  return m_particles.size();
}

const 
Particle* ParticleGroup::GetParticle( std::size_t idx ) const
{
  NE_ASSERT( idx < m_particles.size(), "particle buffer overflow." )();
  return m_particles[idx];
}

Particle* 
ParticleGroup::SpawnParticle()
{
  Particle* p = m_pool.fetch();
  if( p )
  {
    p->m_life   = m_template_life.Value();
    Real scale  = m_template_scale.Value();
    p->m_scale  = Vector4(scale,scale,0.0f,0);
    p->m_age    = 0;

    m_particles.push_back( p );
  }

  return p;
}

const char* 
ParticleGroup::GetName() const 
{ return m_name.c_str(); }

void
ParticleGroup::Update( const uint64 elapsed )
{ 
  float deltaSeconds = float(elapsed) / 1000000;

  for( std::vector<Particle*>::iterator it = m_particles.begin(); it != m_particles.end(); )
  {
    // Decrement the life of the particle.
    (*it)->m_life -= deltaSeconds;

    if( (*it)->m_life < 0 ) 
    {
      //@todo [jared.watt 29.05.2013] Maybe move this into a dying pool rather than deleting immediately?
      m_pool.replace( *it );
      it = m_particles.erase( it );
    }
    else
    {
      // Update the particles velocity (v = u + at).
      (*it)->m_velocity += (Vector4(0,-9.8f,0) * deltaSeconds);

      // Update the position of the particle (s=vt).
      (*it)->m_position += (*it)->m_velocity * deltaSeconds;

      ++it;
    }
  }
}

bool YoungSortOrder(const Particle* lhs, const Particle* rhs)
{
  return lhs->m_age < rhs->m_age;
}

bool OldSortOrder(const Particle* lhs, const Particle* rhs)
{
  return lhs->m_age > rhs->m_age;
}

struct FarSortOrder
{
  Camera* m_camera;

  FarSortOrder(Camera* camera) : m_camera(camera) {}

  bool operator()(const Particle* lhs, const Particle* rhs)
  {
    Vector4 pos = m_camera->GetPosition();
    Vector4 dir = m_camera->GetLookAt() - pos;

    float resl = (lhs->m_position - pos).dot(dir);
    float resr = (rhs->m_position - pos).dot(dir);
    return resl < resr;
  }
};

struct NearSortOrder
{
  Camera* m_camera;

  NearSortOrder(Camera* camera) : m_camera(camera) {}

  bool operator()( const Particle* lhs, const Particle* rhs )
  {  
    Vector4 pos = m_camera->GetPosition();
    Vector4 dir = m_camera->GetLookAt() - pos;

    float resl = (lhs->m_position - pos).dot(dir);
    float resr = (rhs->m_position - pos).dot(dir);
    return resl > resr;
  }
};


void
ParticleGroup::PreRender( Camera* camera )
{
  // Sort the live particles.
  std::sort( m_particles.begin(), m_particles.end(), FarSortOrder(camera) );
}

void 
ParticleGroup::Render( Camera* camera ) const
{
  NE_ASSERT( m_renderDevice != NULL, "Invalid renderer found." )();
  NE_ASSERT( m_pMaterial->GetPassCount() == 1, "Material is expected to have a single pass." )();

  // Set the operation type
  m_renderDevice->SetOperationType( OT_TRIANGLES );
  
  // bind pass to the RenderSystem. 
  // @todo - Enable multi-pass particle shader's.
  Pass* pass = m_pMaterial->GetPass( 0 );
  if( NULL == pass ) return;
  m_renderDevice->SetShaders( pass->GetVertexShader(), pass->GetPixelShader() );

  // Set the Vertex Buffer
  std::size_t iOffset = 0;
  std::size_t iStride = 2*sizeof(Real);
  m_renderDevice->SetVertexBuffers( 0, m_pVertexBuffer, iStride, iOffset );

  // Set the Vertex input layout
  m_renderDevice->SetInputLayout( m_pInputLayout );

  // Grab references to the PassVariables.
  UniformDefinition viewVarDef       = m_renderDevice->GetUniformByName( "view" );
  UniformDefinition projectionVarDef = m_renderDevice->GetUniformByName( "projection" );

  // Set camera transforms for pass
  m_renderDevice->SetUniformBinding( viewVarDef,       (void*)camera->GetViewMatrix().ptr() );
  m_renderDevice->SetUniformBinding( projectionVarDef, (void*)camera->GetProjectionMatrix().ptr() );

  // Iterate the live particles.
  std::vector<Particle*>::const_iterator it_end = m_particles.end();
  for( std::vector<Particle*>::const_iterator it = m_particles.begin(); it != it_end; ++it )
  {
    RenderParticle( m_renderDevice, camera, *(*it) );
  }
}

void 
ParticleGroup::RenderParticle( RenderSystemPtr renderer, Camera* camera, const Particle& particle ) const
{
  NE_ASSERT( renderer != NULL, "Invalid renderer found." )();

//
// Grab references to the PassVariables.
//
  UniformDefinition worldVarDef      = renderer->GetUniformByName( "world" );
  UniformDefinition diffuseVarDef    = renderer->GetUniformByName( "diffuseTexture" );
  UniformDefinition sizeVarDef       = renderer->GetUniformByName( "size" );
  UniformDefinition offsetVarDef     = renderer->GetUniformByName( "offset" );
  UniformDefinition minVarDef        = renderer->GetUniformByName( "min_uv" );
  UniformDefinition maxVarDef        = renderer->GetUniformByName( "max_uv" );

//
// Calculate the local transform of the particle
//
  Matrix4 trans = MatrixMakeTranslation( particle.m_position.x, particle.m_position.y, particle.m_position.z );
  Matrix4 scale = MatrixMakeScale( particle.m_scale.x, particle.m_scale.y, 1.0f );

  Matrix4 world;
  world.SetIdentity();
  world = (trans * scale);

  if( m_screenAlignment != Velocity )
  {
    Matrix4 view = camera->GetViewMatrix();
    view[3] = view[7] = view[11] = 0; //< remove any translation.
    view.Inverse();

    world *= view;
  }

//
// Set particle uniforms for the pass
//
  float textureWidth = m_pTexture->GetWidth(), textureHeight = m_pTexture->GetHeight();
  float size[2]      = { textureWidth, textureHeight };
  float offset[2]    = { -0.5f*textureWidth, 0.0f }; //< this would be were we would do something to preserve some pivot of a sprint.  Currently assuming bottom middle is the pivot point.
  int32 identifier   = m_pTexture->GetTexture()->GetIdentifier(); //< Grab the atlas's texture handle.
  float min[2]       = { m_pTexture->GetTexCoords()[0], m_pTexture->GetTexCoords()[1] };
  float max[2]       = { m_pTexture->GetTexCoords()[2], m_pTexture->GetTexCoords()[3] };

  renderer->SetUniformBinding( worldVarDef,   (void*)world.ptr() );
  renderer->SetUniformBinding( sizeVarDef,    (void*)&size[0] );
  renderer->SetUniformBinding( offsetVarDef,  (void*)&offset[0] );
  renderer->SetUniformBinding( diffuseVarDef, (void*)&identifier );
  renderer->SetUniformBinding( minVarDef,     (void*)&min[0] );
  renderer->SetUniformBinding( maxVarDef,     (void*)&max[0] );

//
// Draw particle.
//
  renderer->Draw( 6, 0 );

}

