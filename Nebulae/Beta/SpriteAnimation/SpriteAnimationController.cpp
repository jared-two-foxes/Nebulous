
#include "SpriteAnimationController.h"

#include <Nebulae/Beta/Scene/SceneNode.h>
#include <Nebulae/Beta/Scene/SceneObject.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimation.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlasManager.h>

using namespace Nebulae;

	
SpriteAnimationController::SpriteAnimationController( RenderSystemPtr renderDevice, Material* material, SceneNode* pNode )
	: m_renderDevice( renderDevice ),
    m_pNode( pNode ),
    m_pObject( nullptr ),
    m_pCurrentAnimation( nullptr ),
	  m_iDisplayFlags( 0 ),
	  m_iCurrentStep( 0 ),
	  m_fCurrentElapsed( 0 ),
	  m_fSecondsPerFrame( 1.0f/24 ),
	  m_animationCompleteCallback(),
    m_material( material )
{}
	
SpriteAnimationController::~SpriteAnimationController()
{}

SpriteAnimationFrame* 
SpriteAnimationController::GetCurrentFrame() const
{
  return ( m_pCurrentAnimation ? m_pCurrentAnimation->GetFrame( m_iCurrentStep ) : NULL );
}
	
void 
SpriteAnimationController::Update( float elapsed )
///
///	Updates the timer on the animations.  Once the required amount of time has been
///	reached it will update m_pObject with the next frame in m_pAnimation.
///
{
  // Sanity Check. If the SceneObject has been not been set up then early out.
  if( !m_pObject ) {
    return;
  }

	if( m_pCurrentAnimation ) {
		m_fCurrentElapsed += elapsed;

		if( m_fCurrentElapsed > m_fSecondsPerFrame ) {
			m_fCurrentElapsed -= m_fSecondsPerFrame;

			if( ++m_iCurrentStep >= m_pCurrentAnimation->GetFrameCount() ) {
				m_iCurrentStep = 0;
				
				// Invoke Callback
				if( m_animationCompleteCallback ) {
					m_animationCompleteCallback();
				}
			}

			// Grab the first frame of the animation.
			SpriteAnimationFrame* pAnimationFrame = m_pCurrentAnimation->GetFrame( m_iCurrentStep );
		
			// Attach the frame to this node.
			SpriteAtlasUtils::SetSpriteFrame( m_renderDevice, m_material, m_pObject, pAnimationFrame->pSpriteAtlas,
        pAnimationFrame->strFrameName, m_iDisplayFlags );
		}	
	}
}

void 
SpriteAnimationController::Stop()
{
	// Clear the animation node if there is currently an animation associated with it.
	if( m_pCurrentAnimation ) 
  {
		m_pNode->Clear();
    m_pObject = nullptr;
  }

	m_pCurrentAnimation = nullptr;
  m_fCurrentElapsed   = 0.0f;
	m_iCurrentStep      = 0;
	m_iDisplayFlags     = 0;
}
	
void 
SpriteAnimationController::Play( const SpriteAnimation* pAnimation, int iDisplayFlags )
{
  NE_ASSERT( pAnimation, "Attempting to play a NULL animation." )();

	if( pAnimation == m_pCurrentAnimation )
	{
  	return;
  }

	if( m_pCurrentAnimation != nullptr )
  {
    Stop();
  }

  m_pCurrentAnimation = pAnimation;
  m_iDisplayFlags     = iDisplayFlags;

	if( m_pCurrentAnimation != nullptr ) 
  {
		SpriteAnimationFrame* animationFrame = m_pCurrentAnimation->GetFrame( m_iCurrentStep );
		SpriteAtlas*          atlas          = animationFrame->pSpriteAtlas;

		m_pObject = SpriteAtlasUtils::AttachFrameToNode( m_renderDevice, m_material, m_pNode, atlas, 
      animationFrame->strFrameName, m_iDisplayFlags );
	}
}

void 
SpriteAnimationController::SetCompleteCallback( std::function<void () >& animationCompleteCallback, void* callbackContext )
{
	m_animationCompleteCallback = animationCompleteCallback;
}

void 
SpriteAnimationController::SetPlaybackSpeed( float seconds_per_frame )
{
  // I dont really like this concept.
	m_fSecondsPerFrame = seconds_per_frame; 
}
