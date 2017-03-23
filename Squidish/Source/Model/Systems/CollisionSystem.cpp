
#include "CollisionSystem.h"

#include <Nebulae/Common/Common.h>
#include <Nebulae/Alpha/Texture/SubTexture.h>
#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>

#include <Model/Model.h>
#include <Model/Component/AspectComponent.h>
#include <Model/Component/CollisionComponent.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Core/Entity.h>


using namespace Nebulae;
using namespace Game;
  
CollisionSystem::CollisionSystem( ModelPtr model, const SpriteAtlasManagerPtr& atlasManager )
: m_model( model ),
  m_atlasManager( atlasManager ),
  m_playerEntity( NULL )
{}


void
CollisionSystem::SetPlayerEntity( Entity* entity )
/// Sets the Entity object that is assumed to represent the 
{
  NE_ASSERT( entity != NULL, "Passed Entity was NULL" )();
  m_playerEntity = entity;
}


void
CollisionSystem::Process( const std::vector<Entity*>& entities, float elapsed ) const
///
/// Compares all Entities against the players Entity object to determine if the user has 
/// collided into any of them.
///
/// @return
///   Nothing.
///
{
//
// update the collision components of the entities.
//
  std::vector<Entity*> collisionEntities;
  std::vector<Entity*>::const_iterator it, end_it = entities.end();
  for( it = entities.begin(); it != end_it; ++it ) 
  {
    if( !(*it)->IsEnabled() ) continue;

    std::vector<AspectComponent* >    aspectComponents;
    std::vector<CollisionComponent* > collisionComponents;
    std::vector<PlacementComponent* > placementComponents;

    m_model->GetComponents<AspectComponent>( *it, &aspectComponents );
    m_model->GetComponents<CollisionComponent>( *it, &collisionComponents );
    m_model->GetComponents<PlacementComponent>( *it, &placementComponents );

    if( !collisionComponents.empty() ) 
    {
      collisionEntities.push_back( (*it) );
    } 
    else 
    {
      continue;
    }

    if ( !placementComponents.empty() ) 
    {
      for ( std::size_t i = 0; i < aspectComponents.size(); ++i )
      {
        Vector4 rect;
        aspectComponents[i]->GetRect( placementComponents[0]->position, &rect );
     
        float halfWidth  = fabs( rect.z - rect.x ) * 0.5f;
        float halfHeight = fabs( rect.w - rect.y ) * 0.5f;
   
        Vector4 position;
        position.x = rect.x + halfWidth;
        position.y = rect.w + halfHeight;
        position.z = placementComponents[0]->position.z;

        collisionComponents[i]->halfExtents = Vector4( halfWidth, halfHeight, 0, 0 ); 
        collisionComponents[i]->center      = position;
      }
    }
  }

  DoBroadPhaseCollisionCheck( collisionEntities );
}


void 
CollisionSystem::DoBroadPhaseCollisionCheck( const std::vector<Entity*>& collisionEntities ) const 
{
  if( NULL == m_playerEntity ) 
  {
    return;
  }
  
  std::vector<CollisionComponent* > playerCollisionComponents;
  m_model->GetComponents<CollisionComponent>( m_playerEntity, &playerCollisionComponents );
  
//
// Iterate over all of the collision entities to check for broadphase overlaps.
//
  std::vector<Entity*>::const_iterator end_entity_it = collisionEntities.end();
  for( std::vector<Entity*>::const_iterator entity_it = collisionEntities.begin(); entity_it != end_entity_it; ++entity_it ) 
  {
    if( *entity_it != m_playerEntity ) 
    {
      std::vector<CollisionComponent*> collisionComponents;
      m_model->GetComponents<CollisionComponent>( (*entity_it), &collisionComponents );

      NE_ASSERT( !collisionComponents.empty(), "Entity has no Collision components." )();

      std::vector<CollisionComponent* >::iterator playerCollisionItr, entityCollitionItr;

      for( playerCollisionItr = playerCollisionComponents.begin(); playerCollisionItr != playerCollisionComponents.end(); ++playerCollisionItr )
      {
        for( entityCollitionItr = collisionComponents.begin(); entityCollitionItr != collisionComponents.end(); ++entityCollitionItr )
        {
        //
        // Check for bounding box collisions.
        //
          bool x = fabs((*playerCollisionItr)->center.x - (*entityCollitionItr)->center.x) <= ((*playerCollisionItr)->halfExtents.x + (*entityCollitionItr)->halfExtents.x);
          bool y = fabs((*playerCollisionItr)->center.y - (*entityCollitionItr)->center.y) <= ((*playerCollisionItr)->halfExtents.y + (*entityCollitionItr)->halfExtents.y);
          //bool z = fabs(playerCollisionComponent->center.z - collisionComponent->center.z) <= (playerCollisionComponent->halfExtents.z + collisionComponent->halfExtents.z);

          if( x && y /*&& z*/ ) 
          {
            OnBroadPhaseCollision( *m_playerEntity, *(*entity_it) );
          }
        }
      }
    }
  }
}


bool
CollisionSystem::DoesEntityHaveTexture( const Entity& obj ) const 
{
  std::vector<AspectComponent* > aspects;
  m_model->GetComponents<AspectComponent>( &obj, &aspects );
  
  std::vector<AspectComponent* >::iterator aspectItr = aspects.begin();
  for( ; aspectItr != aspects.end(); ++aspectItr )
  {
    const SpriteAtlas* atlas = m_atlasManager->GetByName( (*aspectItr)->atlasName );
    if( atlas != NULL ) 
    {
      const SubTexture* subTexture = atlas->FindModuleSubTexture( (*aspectItr)->frameName );
      if( subTexture != NULL ) 
      {
        return true;
      }
    }
  }

  return false;
}


bool
CollisionSystem::DoNarrowPhaseCollisionCheck( Entity& obj1, Entity& obj2 ) const 
{
//
// Assume narrow phase passes if the entities do not have valid Textures.
//
  if( !DoesEntityHaveTexture(obj1) && !DoesEntityHaveTexture(obj2) ) 
  {
    return true;
  }


//
// Determine the area of the textures that actually overlaps.
//
  std::vector<AspectComponent* >    obj1Aspects;
  std::vector<AspectComponent* >    obj2Aspects;
  std::vector<CollisionComponent* > obj1Collisions;
  std::vector<CollisionComponent* > obj2Collisions;

  m_model->GetComponents<AspectComponent>( &obj1, &obj1Aspects );
  m_model->GetComponents<AspectComponent>( &obj2, &obj2Aspects );
  m_model->GetComponents<CollisionComponent>( &obj1, &obj1Collisions );
  m_model->GetComponents<CollisionComponent>( &obj2, &obj2Collisions );

  if( obj1Aspects.empty() || obj2Aspects.empty() || obj1Collisions.empty() || obj2Collisions.empty() ) return false;

  Vector4 a = obj1Collisions[0]->center - obj1Collisions[0]->halfExtents;
  Vector4 b = obj1Collisions[0]->center + obj1Collisions[0]->halfExtents;
  Vector4 c = obj2Collisions[0]->center - obj2Collisions[0]->halfExtents;
  Vector4 d = obj2Collisions[0]->center + obj2Collisions[0]->halfExtents;

  float x5 = std::max<Real >(a.x, c.x);
  float y5 = std::max<Real >(a.y, c.y);
  float x6 = std::min<Real >(b.x, d.x);
  float y6 = std::min<Real >(b.y, d.y);

  NE_ASSERT( x5 < x6 && y5 < y6, "Check for inconsitancy." )( x5, x6, y5, y6 );

  Vector4 min( x5, y5, 0 );
  Vector4 max( x6, y6, 0 );


//
// Get the sub textures corresponding with the intersecting area.
//
  SpriteAtlas* atlas1 = m_atlasManager->GetByName( obj1Aspects[0]->atlasName );
  SpriteAtlas* atlas2 = m_atlasManager->GetByName( obj2Aspects[0]->atlasName );
  if( atlas1 == NULL || atlas2 == NULL ) {
    return false;
  }

  SubTexture* subTexture1 = atlas1->FindModuleSubTexture( obj1Aspects[0]->frameName );
  SubTexture* subTexture2 = atlas2->FindModuleSubTexture( obj2Aspects[0]->frameName );
  if( subTexture1 == NULL || subTexture2 == NULL ) {
    return false;
  }

  Real    width1  = 1.0f / subTexture1->GetWidth();
  Real    height1 = 1.0f / subTexture1->GetHeight();
  Vector4 min1    = min - a;
  Vector4 max1    = max - a;
  SubTexture area1( subTexture1->GetTexture(), min1.x * width1, min1.y * height1, max1.x * width1, max1.y * height1 );

  Real    width2  = 1.0f / subTexture2->GetWidth();
  Real    height2 = 1.0f / subTexture2->GetHeight();
  Vector4 min2    = min - c;
  Vector4 max2    = max - c;
  SubTexture area2( subTexture2->GetTexture(), min2.x * width2, min2.y * height2, max2.x * width2, max2.y * height2 );

  return Nebulae::SpriteAtlasUtils::CheckForPixelCollision( area1, obj1Aspects[0]->flags, area2, obj2Aspects[0]->flags, 128 );
}


void
CollisionSystem::OnBroadPhaseCollision( Entity& obj1, Entity& obj2 ) const 
{
  // Do a finer collision, Pixel test for example to determine if the squid has actually collided with the object,
  // specifically the reefs as they have a large area of alpha on them.
  if( DoNarrowPhaseCollisionCheck(obj1, obj2) ) 
  {
    CollidedSignal(obj1, obj2);
  }
}