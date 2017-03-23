#ifndef __SQUIDISH_VIEW_PARICLEVIEWSYSTEM_H__
#define __SQUIDISH_VIEW_PARICLEVIEWSYSTEM_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {
  class Camera;
  class ParticleEmitter;
  class ParticleSystem;
  class RenderSystem;
}

namespace Game {

class Entity;
class Model;
class ParticleComponent;

class ParticleViewSystem
{
private:
  struct ParticleInstance 
  {
    ParticleComponent*        component;
    Nebulae::ParticleEmitter* emitter;
    bool                      processed;
  };

public:
  typedef std::shared_ptr<Nebulae::FileSystem >     FileArchivePtr;
  typedef std::shared_ptr<Nebulae::RenderSystem >   RenderSystemPtr;
  typedef std::shared_ptr<Nebulae::ParticleSystem > ParticleSystemPtr;
  typedef std::shared_ptr<Nebulae::Camera >         CameraPtr;
  typedef std::shared_ptr<Model >                   ModelPtr;

private:
  FileArchivePtr                 m_fileSystem;       ///< File system interaction object.
  RenderSystemPtr                m_renderDevice;     ///< The rendering context object.
  ParticleSystemPtr              m_particleSystem;   ///< The particle system object.
  CameraPtr                      m_camera;
  ModelPtr                       m_model;            ///< The model object.
  std::vector<ParticleInstance > m_emitters;

  public:
    /** \name Structors */ ///@{
    ParticleViewSystem( const FileArchivePtr& fileSystem, const RenderSystemPtr& renderer, const ParticleSystemPtr& particleSystem, const ModelPtr& model );
    ~ParticleViewSystem();
    //@}

    /** \name Mutators */ ///@{
    void Clear();
    void Init( uint32 count );
    void SetCamera( CameraPtr camera );
    void Process( const std::vector<Entity* >& entities );
    void Update( const uint64 dt );
    void Render() const;
    //@}

  private:
    ParticleInstance* FindInstance( ParticleComponent* component );

};

}

#endif // __SQUIDISH_VIEW_PARICLERENDERER_H__