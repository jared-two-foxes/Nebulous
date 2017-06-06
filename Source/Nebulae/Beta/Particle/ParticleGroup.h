#ifndef __PARTICLEGROUP_H__
#define __PARTICLEGROUP_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Beta/Particle/ParticlePool.h>

namespace Nebulae
{
  
class Camera;
class InputLayout;
class HardwareBuffer;
class Material;
class ParticleSystem;
class RenderSystem;
class SpriteAtlasManager;
class SubTexture;


class ParticleGroup
///
/// Encapsulates a single type of particle; defining what the particle looks like, 
/// how it will be rendered, as well as how long it should live and its basic motion.
///
{
public:
  enum ScreenAlignment
  {
    FacingCameraPosition, ///< Particles will rotate to face the camera position, but will ignore camera rotation.
    Square,               ///< Uniform scale (forced to the X setting), facing the camera.
    Rectangle,            ///< Non-uniform scale, facing the camera.
    Velocity              ///< Orient the particle towards both the camera and the direction the particle is moving. Non-uniform scaling is allowed.
  };  

public:
  static const uint32 DEFAULT_CAPACITY = 512; ///< @brief the default capacity of a Pool

public:
  typedef std::shared_ptr<RenderSystem >       RenderSystemPtr;
  typedef std::shared_ptr<SpriteAtlasManager > SpriteAtlasManagerPtr;

private:
  std::string              m_name;            ///< Filename for the descriptor file.  Doubles as the name of this object.
  RenderSystemPtr          m_renderDevice;    ///< The rendering device.
  SpriteAtlasManagerPtr    m_atlasManager;    ///< The sprite atlas container.
  ParticlePool             m_pool;            ///< Pool of particles available for emission.
  std::vector<Particle* >  m_particles;       ///< List of currently 'live' particles.
  Distribution<Real >      m_template_life;   ///< The base amount of life each particle will have at emission.
  Distribution<Real >      m_template_scale;  ///< The original scale that will be applied to each particle at emission.
  ScreenAlignment          m_screenAlignment;
  Material*                m_pMaterial;       ///< Material used to render this Particle grouping.
  HardwareBuffer*          m_pVertexBuffer;   ///< The vertex buffer. (6 verts, 2 tris)
  InputLayout*             m_pInputLayout;    ///< The input layout which describes the vertex layout.
  SubTexture*              m_pTexture;        ///< SubTexture which is mapped to this particle.

  public:
    ParticleGroup( RenderSystemPtr renderer, SpriteAtlasManagerPtr atlasManager, const std::string& name, uint32 capacity = DEFAULT_CAPACITY );
    ~ParticleGroup();

    bool Load( File& is );
    Particle* SpawnParticle();
    void Update( const uint64 elapsed );
    void PreRender( Camera* camera );
    void Render( Camera* camera ) const;

    const char* GetName() const;
		std::size_t GetParticleCount() const;
    const Particle* GetParticle( std::size_t idx ) const;

  private:
    void RenderParticle( RenderSystemPtr renderer, Camera* camera, const Particle& particle ) const;

}; //ParticleGroup

} //Nebulae

#endif // __PARTICLEGROUP_H__