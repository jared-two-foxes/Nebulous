#ifndef _VIEW_H__
#define _VIEW_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {
  class Camera;
  class DebugUtil;
  class GuiManager;
  class ParticleSystem;
  class RenderSystem;
  class SceneGraph;
  class SpriteAnimationManager;
  class SpriteAtlasManager;
  class WidgetRenderer;
}

namespace Game {

class Entity;
class EntityRenderSystem;
class Model;
class ParticleViewSystem;

class View
///
/// Is responsible for presenting the current state of the Model to the user.
///
{
public:
  typedef std::shared_ptr<Nebulae::FileSystem >             FileArchivePtr;
  typedef std::shared_ptr<Nebulae::RenderSystem >           RenderSystemPtr;
  typedef std::shared_ptr<Nebulae::SpriteAtlasManager >     SpriteAtlasManagerPtr;
  typedef std::shared_ptr<Nebulae::SpriteAnimationManager > SpriteAnimationManagerPtr;
  typedef std::shared_ptr<Nebulae::Camera >                 CameraPtr;
  typedef std::shared_ptr<Nebulae::SceneGraph >             SceneGraphPtr;
  typedef std::shared_ptr<Nebulae::ParticleSystem >         ParticleRendererPtr;
  typedef std::shared_ptr<Nebulae::WidgetRenderer >         WidgetRendererPtr;
  typedef std::shared_ptr<Nebulae::DebugUtil >              DebugRendererPtr;

private:
  FileArchivePtr                   m_fileSystem;         ///< Pointer to the file system controller.
  RenderSystemPtr                  m_renderDevice;       ///< The root render device.
  CameraPtr                        m_camera;             ///< The position and state from which the user views the world.
  SpriteAtlasManagerPtr            m_atlasManager;       ///< Sprite Atlas cache and factory.
  SpriteAnimationManagerPtr        m_animationManager;   ///< Sprite Animation cache and factory.
  SceneGraphPtr                    m_sceneGraph;         ///< Render object layout controller.
  ParticleRendererPtr              m_particleSystem;     ///< Particle subsystem renderer.
  WidgetRendererPtr                m_widgetRenderer;     ///< Renderer for gui components.
  DebugRendererPtr                 m_debugUtil;          ///< Debug utility helper class.

  EntityRenderSystem*              m_renderer;           ///< View Controller for model Entities.
  ParticleViewSystem*              m_particleViewSystem; ///< View Controller for Particles.

  public:
    /** \name Structors */ ///@{
    View( FileArchivePtr fileSystem, RenderSystemPtr renderDevice );
    ~View();
    //@}

    /** \name Mutators */ ///@{
    void Clear();
    void Init( const std::shared_ptr<Model >& model );
    void ApplyEntityTintFactor( const Entity& entity, const Real tintValue );
    void ApplyEntityTintColour( const Entity& entity, const Nebulae::Colour& tintColour );
    void ProcessAndRender( const std::shared_ptr<Model >& model ) const;
    void ProcessAndRender( const std::shared_ptr<Nebulae::GuiManager >& guiMgr );
    void Render();
    void Update( const uint64 elapsed );
    //@}

    /** \name Accessors */ ///@{
    RenderSystemPtr           GetRenderDevice();
    CameraPtr                 GetCamera();
    WidgetRendererPtr         GetWidgetRenderer();
    SpriteAnimationManagerPtr GetSpriteAnimationManager() const;
    SpriteAtlasManagerPtr     GetSpriteAtlasManager() const;
    EntityRenderSystem&       GetEntityRenderer();
    ParticleViewSystem&       GetParticleViewSystem();
    //@}    
    
};

}

#endif // __VIEW_H__