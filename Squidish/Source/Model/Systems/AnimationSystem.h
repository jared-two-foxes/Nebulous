#ifndef CONTROLLER_SYSTEMS_ANIMATIONSYSTEM_H__
#define CONTROLLER_SYSTEMS_ANIMATIONSYSTEM_H__

#include <Nebulae/Nebulae.h>

namespace Nebulae {
  class SpriteAnimationManager;
}

namespace Game
{
  //Forward Deceleration
  class Entity;
  class Model;

	/** AnimationSystem.
	 */
	class AnimationSystem
	{
  private:
    typedef std::shared_ptr<Model >                           ModelPtr;
    typedef std::shared_ptr<Nebulae::SpriteAnimationManager > SpriteAnimationManagerPtr;

  private:
    ModelPtr                  m_model;            ///< Pointer to the model object. 
    SpriteAnimationManagerPtr m_animationManager; ///< Factory for creation of Animations.
    const float               m_secondsPerFrame;  ///< Max time to display each frame for.

    public:
      /** \name Structors */ ///@{
      AnimationSystem( ModelPtr model, const SpriteAnimationManagerPtr& spriteAnimationManager );
      //@}

      /** \name Mutators */ ///@{
      void PlayAnimation( Entity& entity, const std::string& animationName );
      void Process( const std::vector<Entity*>& entities, float elapsed ) const;
      //@}

	}; //AnimationSystem

} //Nebulae

#endif // CONTROLLER_SYSTEMS_ANIMATIONSYSTEM_H__