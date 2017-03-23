
#include "AnimationSystem.h"

#include <Model/Model.h>
#include <Model/Component/AnimationComponent.h>
#include <Model/Component/AspectComponent.h>
#include <Model/Core/Entity.h>

#include <Nebulae/Common/Common.h>

#include <Nebulae/Beta/SpriteAnimation/SpriteAnimation.h>
#include <Nebulae/Beta/SpriteAnimation/SpriteAnimationManager.h>
#include <Nebulae/Beta/SpriteAtlas/SpriteAtlas.h>

using namespace Game;
using namespace Nebulae;

  
AnimationSystem::AnimationSystem( ModelPtr model, const SpriteAnimationManagerPtr& animationManager )
: m_model( model ),
  m_animationManager( animationManager ),
  m_secondsPerFrame( 1.f/24.f )
{}


void
AnimationSystem::PlayAnimation( Entity& entity, const std::string& animationName )
///
/// Cause an entity to begin playing a specified animation.
///
/// @param entity
///   The entity to apply the animation to.
///
/// @param animationName
///   The name of the animation to play.
///
/// @return
///   Nothing.
///
{ 
  std::vector<AnimationComponent*> animationComponents; 
  m_model->GetComponents<AnimationComponent>( &entity, &animationComponents );
  if( animationComponents.empty() ) {
    NE_LOG( "The passed entity doesn't contain an Animation component" );
    return;
  }

  Nebulae::SpriteAnimation* animation = m_animationManager->GetByName( animationName );
  if( !animation ) {
    animation = m_animationManager->Create( animationName );
  }

  std::for_each( animationComponents.begin(), animationComponents.end(), [&](AnimationComponent* component){
    component->animation      = animation;
    component->currentFrame   = 0;
    component->currentElapsed = 0.f;
  });
}

void
AnimationSystem::Process( const std::vector<Entity*>& entities, float elapsed ) const
///
/// Updates the position of all entities that have a motion component.
///
/// @return
///   Nothing.
///
{
  std::vector<Entity*>::const_iterator end_entity_it = entities.end();
  for( std::vector<Entity*>::const_iterator entity_it = entities.begin(); entity_it != end_entity_it; ++entity_it ) 
  {
    if( !(*entity_it)->IsEnabled() ) continue;

    int32 identifier = (*entity_it)->GetIdentifier();

    std::vector<AnimationComponent* > animationComponents;
    m_model->GetComponents<AnimationComponent>( (*entity_it), &animationComponents );

    std::vector<AspectComponent* > aspectComponents;
    m_model->GetComponents<AspectComponent>( (*entity_it), &aspectComponents );

    // check if this entity has a valid animation component
    if( animationComponents.empty() ) 
    {
      continue;
    }

    // make sure animation is setup
    if( animationComponents[0]->animation == NULL && animationComponents[0]->animationName.length() > 0 ) 
    {
      animationComponents[0]->animation = m_animationManager->GetByName( animationComponents[0]->animationName );
      if( !animationComponents[0]->animation ) 
      {
        animationComponents[0]->animation = m_animationManager->Create( animationComponents[0]->animationName );
      }
    }

    // unable to find said animation.
    if( !animationComponents[0]->animation ) 
    {
      continue;
    }

    // increment the animation state.    
    animationComponents[0]->currentElapsed += elapsed;
    if( animationComponents[0]->currentElapsed > m_secondsPerFrame ) 
    {
      animationComponents[0]->currentElapsed -= m_secondsPerFrame;
    
      if( ++animationComponents[0]->currentFrame >= animationComponents[0]->animation->GetFrameCount() ) 
      {
        animationComponents[0]->currentFrame = 0;
      
        //@todo invoke a Callback to inform of the end of the animation?
        // if( m_pfnAnimationCompleteCallback ) {
        //   (*m_pfnAnimationCompleteCallback)( m_pCallbackContext );
        // }
      }
    } 

    // update the aspect component
    if( !aspectComponents.empty() ) 
    {
      Nebulae::SpriteAnimation*      animation = animationComponents[0]->animation;
      Nebulae::SpriteAnimationFrame* frame     = animation->GetFrame( animationComponents[0]->currentFrame );
      
      aspectComponents[0]->atlasName = frame->pSpriteAtlas->GetName();
      aspectComponents[0]->frameName = frame->strFrameName;
    }
  }
}