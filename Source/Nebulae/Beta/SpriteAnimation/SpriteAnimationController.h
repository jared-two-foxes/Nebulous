#ifndef __SPRITEANIMATIONCONTROLLER_H__
#define __SPRITEANIMATIONCONTROLLER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {

// Forward Decelerations
class Material;
class RenderSystem;
class SceneNode;
class SceneObject;
class SpriteAnimation;
struct SpriteAnimationFrame;

/** SpriteAnimationController.
 */
class SpriteAnimationController
{
public:
  typedef std::shared_ptr<RenderSystem > RenderSystemPtr;

private:
  RenderSystemPtr          m_renderDevice;               ///< Weak reference to the render device that controls all renderer objects.
  SceneNode*               m_pNode;                      ///< Weak reference to the node that the animation is to be attached to.
  SceneObject*             m_pObject;                    ///< Weak reference to the SceneObject being used to display the animation frames.		
  const SpriteAnimation*   m_pCurrentAnimation;          ///< Reference to the currently executing animation.
  int                      m_iDisplayFlags;              ///< The current flags being used to display the frames.
	std::size_t              m_iCurrentStep;               ///< Current step in the animation.
  float                    m_fCurrentElapsed;            ///< Time which the frame has been visible.
  float                    m_fSecondsPerFrame;           ///< Max time to display each frame for.
  std::function<void () >  m_animationCompleteCallback;  ///< Callback function to be invoked when the animation is complete.
  Material*                m_material;                   ///< Material used to render the animation.

  public:
    /** \name Structors */ ///@{
    SpriteAnimationController( RenderSystemPtr renderDevice, Material* material, SceneNode* pObj );
    virtual ~SpriteAnimationController();
    //@}

    /** \name Accessors */ ///@{
    SpriteAnimationFrame* GetCurrentFrame() const;
    //@}

    /** \name Mutators */ ///@{
    void Update( float fElapsed );
    void Play( const SpriteAnimation* pAnimation, int iDisplayFlags = 0 );
    void Stop();
    void SetCompleteCallback( std::function<void () >& pAnimationCompleteCallback, void* pCallbackContext );
    void SetPlaybackSpeed( float seconds_per_frame );
    //@}

}; //SpriteAnimationController

} //Nebulae

#endif // __SPRITEANIMATIONCONTROLLER_H__