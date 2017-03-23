#ifndef CONTROLLER_SYSTEMS_COLLISIONSYSTEM_H__
#define CONTROLLER_SYSTEMS_COLLISIONSYSTEM_H__

#include <Nebulae/Nebulae.h>

#include <boost/signals2.hpp>

namespace Game {

//Forward Deceleration
class Entity;
class Model;

/** CollisionSystem.
 *
 *  This system is responsible for determining when collisions have occured between 
 *  game entities
 */
class CollisionSystem
{
public:
  typedef boost::signals2::signal<void (const Entity&, const Entity&)> CollidedSignalType;    ///< emitted when the list box is cleared

  typedef std::shared_ptr<Model >                           ModelPtr;
  typedef std::shared_ptr<Nebulae::SpriteAtlasManager >     SpriteAtlasManagerPtr;

private:
  ModelPtr              m_model;          ///< Reference to the component cache.
  SpriteAtlasManagerPtr m_atlasManager;   ///< Manager object for all of the sprite atlas's.
  Entity*               m_playerEntity;   ///< The entity that represents the player.
  
  public:
    /** \name Structors */ ///@{
    CollisionSystem( ModelPtr model, const SpriteAtlasManagerPtr& atlasManager );
    //@}

    /** \name Mutators */ ///@{
    void SetPlayerEntity( Entity* entity );
    void Process( const std::vector<Entity*>& entities, float elapsed ) const;
    //@}

    mutable CollidedSignalType CollidedSignal; ///< The entity collision detected signal.

  private:
    void DoBroadPhaseCollisionCheck( const std::vector<Entity*>& collisionEntities ) const;
    bool DoesEntityHaveTexture( const Entity& obj ) const;
    bool DoNarrowPhaseCollisionCheck( Entity& obj1, Entity& obj2 ) const;

    void OnBroadPhaseCollision( Entity& obj1, Entity& obj2 ) const;

}; //CollisionSystem

} //Nebulae

#endif // CONTROLLER_SYSTEMS_COLLISIONSYSTEM_H__