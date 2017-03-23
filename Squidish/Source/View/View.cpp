
#include "View.h"
#include "EntityRenderSystem.h"
#include "ParticleViewSystem.h"

#include "Model/Model.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Alpha.h>
#include <Nebulae/Beta/Camera/Camera.h>
#include <Nebulae/Beta/Debug/DebugUtil.h>
#include <Nebulae/Beta/Gui/GuiManager.h>
#include <Nebulae/Beta/Gui/WidgetRenderer.h>
#include <Nebulae/Beta/Particle/ParticleSystem.h>
#include <Nebulae/Beta/Scene/SceneGraph.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimationManager.h>
#include <Nebulae/Beta/SpriteBatch/SpriteBatch.h>


using namespace Game;
using namespace Nebulae;


View::View( FileArchivePtr fileSystem, RenderSystemPtr renderDevice )
///
/// Constructor
///
: m_fileSystem( fileSystem ), 
  m_renderDevice( renderDevice ),
  m_renderer( nullptr ),
  m_particleViewSystem( nullptr ),
  m_widgetRenderer( nullptr )
{}


View::~View()
///
/// Destructor
///
{
  if( m_renderer != nullptr ) {
    delete m_renderer;
  }
  m_renderer = nullptr;

  if( m_particleViewSystem != nullptr ) {
    delete m_particleViewSystem;
  }
  m_particleViewSystem = nullptr;
}

void
View::Clear()
///
/// Clear the internal states of the view components.
///
/// @return
///   Nothing.
///
{
  //m_debugUtil->Clear();
  m_renderer->Clear();
  m_particleViewSystem->Clear();
  //m_widgetRenderer->Clear();
}

void 
View::Init( const std::shared_ptr<Model>& model )
///
/// Initializes the render components.
///
/// @param renderer
///   Low level render object.
///
/// @param model
///   The model.
///
/// @return
///   Nothing.
///
{

//
// Create the root render library objects.
//

  // Create the camera
  Vector4 vCameraEye( 0.0f, 0.0f, 500.0f );
  Vector4 vLookAt( 0.0f, 0.0f, 0.0f );
  Vector4 vUp( 0.0f, 1.0f, 0.0f );

  const RenderSystem::WindowPtr window = m_renderDevice->GetWindow();

  m_camera = CameraPtr( new Nebulae::Camera() );
  m_camera->LookAt( vCameraEye, vLookAt, vUp );
  m_camera->SetOrtho( Real(window->GetWidth()), Real(window->GetHeight()), 0.001f, 1000.0f );
  
  // Create the Sprite Util classes.
  m_atlasManager     = SpriteAtlasManagerPtr( new SpriteAtlasManager(m_fileSystem, m_renderDevice) );
  m_animationManager = SpriteAnimationManagerPtr( new SpriteAnimationManager(m_fileSystem, m_atlasManager) );


//
// Create the Renderer objects.
//

  // Create the ParticleSystem
  m_particleSystem = ParticleRendererPtr( new ParticleSystem(m_fileSystem, m_renderDevice, m_atlasManager) );

  // Create the Scene
  m_sceneGraph = SceneGraphPtr( new SceneGraph(m_renderDevice) );
  m_sceneGraph->Initialize();

  // Create the debug renderer
  m_debugUtil = DebugRendererPtr( new DebugUtil(m_renderDevice) );
  m_debugUtil->Init();

  SpriteBatch* batcher = new SpriteBatch(m_renderDevice);
  batcher->Init();

  m_widgetRenderer = WidgetRendererPtr( new Nebulae::WidgetRenderer(m_fileSystem, batcher, m_renderDevice) );
  m_widgetRenderer->Init();


//
// Create the View Controller objects.
//

  m_renderer = new EntityRenderSystem( m_fileSystem, m_renderDevice, m_sceneGraph, m_atlasManager, m_animationManager, m_debugUtil, model );
  m_renderer->SetCamera( m_camera );
  m_renderer->Init( 512 );

  m_particleViewSystem = new ParticleViewSystem( m_fileSystem, m_renderDevice, m_particleSystem, model );
  m_particleViewSystem->SetCamera( m_camera );
  m_particleViewSystem->Init( 512 );

}

void 
View::ApplyEntityTintFactor( const Entity& entity, const Real tintValue )
///
/// Sets the strength of the tint colour of a specific entity.  The @param
/// tintValue needs to be a value between [0,1] where 0 will apply no tint and
/// 1 will apply maximum tint. 
///
/// @param entity
///   The entity to apply the tint factor too.
///
/// @param tintValue
///   The strength of tint to apply.
///
/// @return 
///   Nothing.
///
/// @note
///   The tint colour is only applied to the portion of the billboard that has
///   some degree of alpha
///
{
  NE_ASSERT( m_renderer != nullptr, "Attempting to access a nullptr." )();

  // Enforce the max and min values of the tint.
  Real tintStrength = tintValue;
  tintStrength = std::max<float>( tintValue, 0.0f );
  tintStrength = std::min<float>( tintValue, 1.0f );

  m_renderer->ApplyTintFactor( entity, tintStrength );
}

void 
View::ApplyEntityTintColour( const Entity& entity, const Nebulae::Colour& tintColour )
///
/// Sets the colour of the tint that is applied to a specific entity.
///
/// @param entity
///   The entity to apply the tint colour too.
///
/// @param tintValue
///   The tintColour.
///
/// @return 
///   Nothing.
///
/// @note
///   The tint colour is only applied to the portion of the billboard that has
///   some degree of alpha
///
{
  NE_ASSERT( m_renderer != nullptr, "Attempting to access a nullptr." )();

  m_renderer->ApplyTintColour( entity, tintColour );
}

void
View::ProcessAndRender( const std::shared_ptr<Model>& model ) const
///
/// Render the current model state.
///
/// @param model
///   The model.
///
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_renderer != nullptr, "Attempting to access a nullptr." )();
  NE_ASSERT( m_particleViewSystem != nullptr, "Attempting to access a nullptr." )();

  const std::vector<Entity*>& entities = model->GetEntities();
  
  if( m_renderer != nullptr )
  {
    m_renderer->Process( entities );
  }

  if( m_particleViewSystem != nullptr )
  {
    m_particleViewSystem->Process( entities );
  }
}

void
View::ProcessAndRender( const std::shared_ptr<Nebulae::GuiManager >& guiMgr )
///
/// Render the current model state.
///
/// @param model
///   The model.
///
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_widgetRenderer != nullptr, "Attempting to access a nullptr." )();

  const std::list<Nebulae::Widget*>& widgets = guiMgr->GetRegisteredWidgets();

  if( m_widgetRenderer )
  {
    m_widgetRenderer->Render( widgets );
  }
}

void
View::Render()
///
/// Render any current debug geometry.
///
/// @return
///   Nothing.
///
{
  // Reset the RenderState just in case something got changed.
  m_renderDevice->SetDepthTest( false );
  m_renderDevice->SetBlendingState( true );

  // Render the scene.
  if( m_sceneGraph ) 
  {
    m_sceneGraph->Render( m_camera.get() );
  }

  // Render the particle system.
  if( m_particleViewSystem != nullptr )
  {
    m_particleViewSystem->Render();
  }

  // Render the debug geometry.
  if( m_debugUtil != nullptr ) 
  {
    m_debugUtil->Draw( m_camera );
  }

  m_renderDevice->SetBlendingState( false );

  m_renderer->GetPrimitiveBatch().End();

  m_renderDevice->SetBlendingState( true );
}

void
View::Update( const uint64 elapsed )
///
/// Update the visual state by one simulation tick.
///
/// @param elapsed
///   Amount of time to advance the simulation by.
///
/// @return
///   Nothing.
///
{
  NE_ASSERT( m_particleViewSystem != nullptr, "Attempting to access a nullptr." )(); 

  m_particleViewSystem->Update( elapsed );
}

View::RenderSystemPtr 
View::GetRenderDevice()
///
/// Accessor for the renderDevice object.
///
/// @return
///   The render device.
///
{
  return m_renderDevice;
}

View::CameraPtr
View::GetCamera()
///
/// Returns the Camera.
///
/// @return
///   The Camera.
///
{
  NE_ASSERT( m_camera, "Attempting to access the view camera before its been initialized." )();
  return m_camera;
}

View::SpriteAnimationManagerPtr
View::GetSpriteAnimationManager() const
///
/// Returns the sprite animation resource factory
///
/// @return
///   The sprite animation factory.
///
{
  return m_animationManager;
}

View::SpriteAtlasManagerPtr     
View::GetSpriteAtlasManager() const
///
/// Returns the sprite atlas resource factory
///
/// @return
///   The sprite atlas factory.
///
{
  return m_atlasManager;
}

EntityRenderSystem&  
View::GetEntityRenderer()
///
/// Returns the Entity renderer.
///
/// @return
///   The entity renderer.
///
{
  NE_ASSERT( m_renderer, "Attempting to access the entity renderer before its been initialized." )();
  return *m_renderer;
}

ParticleViewSystem&
View::GetParticleViewSystem()
///
/// Accessor for the ParticleSystem object.
///
/// @return
///   The ParticleSystem.
///
{
  NE_ASSERT( m_particleViewSystem, "Attempting to access the particle renderer before its been initialized." )();
  return *m_particleViewSystem;
}