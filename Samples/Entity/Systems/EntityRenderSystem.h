#ifndef SAMPLE_WORLDRENDERSYSTEM_H__
#define SAMPLE_WORLDRENDERSYSTEM_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {
  class Camera;
  class Material;
  class RenderSystem;
  class SceneGraph;
  class SceneNode;
  class SpriteAtlasManager;
}

namespace Sample
{

class Entity;

class EntityRenderSystem
///
/// Controls the rendering of all of the active Entities in the current simulation step.
///
/// @todo [jared.watt 09-08-2013] Not sure how Im going to handle animation.  May need to add an
///       update loop or atleast pass through the elapsed time to render.  Might be a good idea to
///       modify the AnimationController to be a generic controller rather than a specific controller, 
///       by that I mean pass the information to the controller rather than have a controller own the 
///       information and have a controller per animating object.
///
{
public:
  typedef std::shared_ptr<Nebulae::Camera >            CameraPtr;
  typedef std::shared_ptr<Nebulae::RenderSystem >      RenderDevicePtr;

private:
  struct Actor
  {
    uint32              identifier;
    Nebulae::SceneNode* node;
    bool                processed;

    Actor() : identifier(0), node(NULL), processed(false) {}
  };

private:
  RenderDevicePtr                  m_renderer;     ///< The render device object.
  CameraPtr                        m_camera;       ///< The position and state from which the user views the world.
  std::vector<Nebulae::Material* > m_materials;    ///< A list of available material objects. 
  Nebulae::SceneGraph*             m_sceneGraph;   ///< The current scene layout.
  Nebulae::SpriteAtlasManager*     m_atlasManager; ///< The atlas controller class.
  std::vector<Actor* >             m_actors;       ///< A list of all of the active actors in the world.
  std::vector<Actor* >             m_unusedActors; ///< A list of all of the currently unused actor objects.

  public:
    EntityRenderSystem( const RenderDevicePtr& renderer );
    ~EntityRenderSystem();

    void Init( uint32 count );
    void Render( const std::vector<Entity*>& entities );
    void ApplyTintFactor( Entity& entity, float factor );
    void ApplyTintColour( Entity& entity, Nebulae::Colour colour );

    CameraPtr GetCamera() const { return m_camera; }

  private:
    Actor* FindActorByIdentifier( uint32 identifier ) const;

};

}

#endif // SAMPLE_WORLDRENDERSYSTEM_H__

