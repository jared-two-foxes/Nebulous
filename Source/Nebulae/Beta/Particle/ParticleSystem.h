#ifndef NEBULAE_BETA_PARTICLE_PARTICLESYSTEM_H_
#define NEBULAE_BETA_PARTICLE_PARTICLESYSTEM_H_

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class Camera;
class FileSystem;
class ParticleEmitter;
class ParticleGroup;
class RenderSystem;
class SpriteAtlasManager;


class ParticleSystem
///
/// Encapsulates all of the particles in the universe.
///
{
public:
  typedef std::shared_ptr<FileSystem> FileArchivePtr;
  typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
  typedef std::shared_ptr<SpriteAtlasManager> AtlasManagerPtr;

private:
  FileArchivePtr m_fileSystem;
  RenderSystemPtr m_renderDevice;
  AtlasManagerPtr m_atlasManager;
  std::vector<ParticleGroup*> m_groups;     ///< List of all the ParticleGroups known to the system.
  std::vector<ParticleEmitter*> m_emitters; ///< List of all the ParticleEmitters known to the system.
  Camera* m_camera;                         ///< Camera used to render the particles.

public:
  ParticleSystem( FileArchivePtr fileSystem, RenderSystemPtr renderer, AtlasManagerPtr atlasManager );
  ~ParticleSystem();

  void Clear();
  void Update( const uint64 elapsed );
  void Render();
  ParticleGroup* CreateGroup( const std::string& filename );
  ParticleEmitter* CreateEmitter( const std::string& filename );
  void DestroyEmitter( ParticleEmitter* emitter );
  void SetCamera( Camera* camera );

  Camera* GetCamera() const;
};

} // namespace Nebulae

#endif // NEBULAE_BETA_PARTICLE_PARTICLESYSTEM_H_