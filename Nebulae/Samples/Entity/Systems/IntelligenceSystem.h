#ifndef __ENTITY_SYSTEMS_INTELLIGENCESYSTEM_H__
#define __ENTITY_SYSTEMS_INTELLIGENCESYSTEM_H__

#include <Nebulae/Common/Common.h>

namespace Sample
{

class Entity;

class IntelligenceSystem
{
public:
  IntelligenceSystem();

  void Process( Real elapsed, std::vector<Entity*>& entities );

private:
  
  //Steering behaviours
  Vector4 Seek( Entity& entity, const Vector4& position ) const;
  Vector4 Arrive( Entity& entity, const Vector4& position ) const;
  Vector4 Flee( Entity& entity, const Vector4& position ) const;
  Vector4 Wander( Entity& entity ) const;

  //Group Behaviours
  Vector4 Alignment( Entity& entity, const std::vector<Entity*>& neighbors ) const;
  Vector4 Cohesion( Entity& entity, const std::vector<Entity*>& neighbors ) const;
  Vector4 Separation( Entity& entity, const std::vector<Entity*>& neighbors ) const;

};

}

#endif // __ENTITY_SYSTEMS_INTELLIGENCESYSTEM_H__