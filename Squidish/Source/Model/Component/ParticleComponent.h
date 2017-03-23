#ifndef MODEL_COMPONENT_PARTICLECOMPONENT_H__
#define MODEL_COMPONENT_PARTICLECOMPONENT_H__

#include <Model/Core/Component.h>

namespace Nebulae {
  class ParticleEmitter;
}

namespace Game {

class ParticleComponent : public Component
///
/// Component which describes a single ParticleEmitter.
///
{
public:
  std::string emitterName;    ///< Name of the Emitter resource to use.
  bool        started;        ///< Has the emitter started emitting.
  bool        looping;        ///< Should the emitter looping.

  public:
    ParticleComponent() 
    : Component(), 
      emitterName( "" ), 
      started(false),
      looping(false)
      {}

    static ComponentType GetType() { return COMPONENT_TYPE_PARTICLE; }
};

}

#endif // MODEL_COMPONENT_ANIMATIONCOMPONENT_H__