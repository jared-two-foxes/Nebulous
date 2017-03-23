#ifndef SAMPLE_WORLDRENDERSYSTEM_H__
#define SAMPLE_WORLDRENDERSYSTEM_H__

#include <Config.h>
#include <Nebulae/Common/Common.h>

#include <Nebulae/Beta/PrimitiveBatch/Effect.h>
#include <Nebulae/Beta/PrimitiveBatch/PrimitiveBatch.h>

namespace Nebulae {

class Camera;
class DebugUtil;
class Material;
class RenderSystem;
class SceneGraph;
class SceneNode;
class SpriteAnimationManager;
class SpriteAtlasManager;

}

namespace Game {

class Entity;
class Model;

struct Vertex
{
  Vector4 position;
  Vector4 colour;
};

class EntityRenderSystem
///
/// Controls the rendering of all of the active Entities in the current simulation step.
///
/// @todo [jared.watt 09-08-2013] Not sure how to going to handle animation.  May need to add an
///       update loop or atleast pass through the elapsed time to render.  Might be a good idea to
///       modify the AnimationController to be a generic controller rather than a specific controller, 
///       by that I mean pass the information to the controller rather than have a controller own the 
///       information and have a controller per animating object.
///
{
public:
  typedef std::shared_ptr<Nebulae::FileSystem >             FileArchivePtr;
  typedef std::shared_ptr<Nebulae::RenderSystem >           RenderSystemPtr;
  typedef std::shared_ptr<Nebulae::Camera >                 CameraPtr;
  typedef std::shared_ptr<Nebulae::SceneGraph >             SceneGraphPtr;
  typedef std::shared_ptr<Nebulae::SpriteAtlasManager >     SpriteAtlasManagerPtr;
  typedef std::shared_ptr<Nebulae::SpriteAnimationManager > SpriteAnimationManagerPtr;
  typedef std::shared_ptr<Nebulae::DebugUtil >              DebugRendererPtr;

  typedef std::shared_ptr<Model >                           ModelPtr;

private:
  struct Actor
  {
    int32               identifier;
    Nebulae::SceneNode* node;
    bool                processed;
    Nebulae::Colour     tintColour;
    float               tintStrength;

    Actor() : identifier(-1), node(NULL), processed(false), tintColour(Nebulae::CLR_WHITE), tintStrength(0) {}
  };

private:
  FileArchivePtr                   m_fileSystem;       ///< File system interaction object.
  RenderSystemPtr                  m_renderDevice;     ///< The rendering context object.
  CameraPtr                        m_camera;           ///< The position and state from which the user views the world.
  SceneGraphPtr                    m_sceneGraph;       ///< Debug utility helper class.
  SpriteAtlasManagerPtr            m_atlasManager;     ///< Render object layout controller.
  SpriteAnimationManagerPtr        m_animationManager; ///< Sprite Atlas cache and factory.
  DebugRendererPtr                 m_debugUtil;        ///< Sprite Animation cache and factory.
  ModelPtr                         m_model;            ///< Reference to the component cache.
  std::vector<Nebulae::Material* > m_materials;        ///< A list of all the materials to use for rendering.
  std::vector<Actor* >             m_actors;           ///< List of all of the live actors in the system.
  std::vector<Actor* >             m_unusedActors;     ///< List of all of the currently unused actors in the system.

  Nebulae::PrimitiveBatch<Vertex > m_primitiveBatch;
  Nebulae::Effect                  m_effect;

  public:
    /** \name Structors */ ///@{
    EntityRenderSystem( FileArchivePtr fileSystem, RenderSystemPtr renderer, SceneGraphPtr sceneGraph, 
        SpriteAtlasManagerPtr spriteAtlasManager, SpriteAnimationManagerPtr spriteAnimationManager, 
        DebugRendererPtr debugRenderer, ModelPtr model );

    ~EntityRenderSystem();
    //@}

    /** \name Mutators */ ///@{
    void Clear();
    void Init( uint32 count );
    void SetCamera( CameraPtr camera );
    void Process( const std::vector<Entity*>& entities );
    void ApplyTintFactor( const Entity& entity, const Real factor );
    void ApplyTintColour( const Entity& entity, const Nebulae::Colour& colour );
    //@}

    Nebulae::PrimitiveBatch<Vertex >& GetPrimitiveBatch();

  private:
  
    /** \name Accessors */ ///@{
    Nebulae::Material* FindMaterialByName( const std::string& name ) const;
    Actor*             FindActorByIdentifier( uint32 identifier ) const;
    //@}

}; 

}

#endif // SAMPLE_WORLDRENDERSYSTEM_H__

