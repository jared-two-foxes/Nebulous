#ifndef __SCENEOBJECTCONTROLLER_H__
#define __SCENEOBJECTCONTROLLER_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{
	//Forward Declerations.
	class SceneObject;

	/** SceneObjectController.
	 */
	class SceneObjectController
	{
	protected:
		SceneObject* m_pObject;

	public:
		explicit SceneObjectController( SceneObject* pObj ) : m_pObject(pObj) {}
		virtual ~SceneObjectController() {}

		// Is this going to be Needed?
		//virtual void PrepareRenderSystem() {} 
		virtual void Enter() {};
		virtual void Update( float fElapsed ) = 0;

	}; //SceneObjectController

} //Nebulae

#endif // __SCENEOBJECTCONTROLLER_H__