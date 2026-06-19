#ifndef NEBULAE_BETA_SCENE_SCENEOBJECTCONTROLLER_H_
#define NEBULAE_BETA_SCENE_SCENEOBJECTCONTROLLER_H_

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{
// Forward Declerations.
class SceneObject;

/** SceneObjectController.
 */
class SceneObjectController
{
protected:
  SceneObject* m_pObject;

public:
  explicit SceneObjectController( SceneObject* pObj ) : m_pObject( pObj ) {}
  virtual ~SceneObjectController() {}

  // Is this going to be Needed?
  // virtual void PrepareRenderSystem() {}
  virtual void Enter() {};
  virtual void Update( float fElapsed ) = 0;

}; // SceneObjectController

} // namespace Nebulae

#endif // NEBULAE_BETA_SCENE_SCENEOBJECTCONTROLLER_H_