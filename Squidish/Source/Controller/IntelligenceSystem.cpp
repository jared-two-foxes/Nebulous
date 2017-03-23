
#include "IntelligenceSystem.h"

#include <Model/Model.h>
#include <Model/Core/Entity.h>
#include <Model/Component/BehaviourComponent.h>
#include <Model/Systems/MotionSystem.h>

using Game::Entity;
using Game::IntelligenceSystem;
using Game::MotionSystem;

IntelligenceSystem::IntelligenceSystem( ModelPtr modelPtr, MotionSystem* motionSystem )
: m_model( modelPtr ), 
  m_motionSystem( motionSystem )
{}

void 
IntelligenceSystem::Process( const std::vector<Entity* >& entities, Entity* playerEntity )
{
  typedef std::vector<Entity* > EntityList;

  NE_ASSERT( playerEntity != nullptr, "Player entity cannot be null." )();

  for( EntityList::const_iterator it = entities.begin(); it != entities.end(); ++it )
  {
    // Retrieve all of the behaviour components for this entity. 
    std::vector<BehaviourComponent* > components;
    m_model->GetComponents<BehaviourComponent>( (*it), &components );

    if( components.empty() ) continue;

    //@todo draw a coloured line between the entity & the player indicating if its in the "hot" zone or not.

    const Vector4 delta    = m_motionSystem->GetDistanceBetweenEntities( m_model, *(*it), *playerEntity );
    const Real    distance = delta.length();

    // Sum up the strength factors.
    Real attraction = 0;
    std::for_each( components.begin(), components.end(), [&](BehaviourComponent* component) {
      if( distance < component->m_attractionInfluenceRadius ) {
        attraction += component->m_attractionStrength;
      }
    });

    // Apply the attraction force if specified
    if( fabs(attraction) > std::numeric_limits<float>::epsilon() )
    {
      m_motionSystem->ApplyAttraction( (*it), playerEntity, attraction );
    }
  }
}