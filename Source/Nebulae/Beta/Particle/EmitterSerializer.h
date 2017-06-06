#ifndef NEBULAE_PARTICLE_EMITTERSERIALIZER_H__
#define NEBULAE_PARTICLE_EMITTERSERIALIZER_H__

#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Base/Types/DistributionSerializer.h>

namespace Nebulae {

class ParticleEmitter;
class ParticleSystem;

class EmitterSerializer
{
  public:
    bool Load( File& is, ParticleSystem* particleSystem, ParticleEmitter& emitter )
    {
    //
    // Read in the material file.
    //
      Json::Value  root;
      if( !ParseJSON(is, &root) )
      {
        return false;
      }
        
    //
    // Iterate each pass and load it from data
    //
      const Json::Value& tankcontents = root["tank"];
      for( int i = 0, n = tankcontents.size(); i < n; ++i ) 
      {
        const Json::Value& tank_data = tankcontents[i];

        //Extract from file tank set contents.
        ParticleGroup* group = particleSystem->CreateGroup( tank_data["group"].asCString() );
        int32          count = tank_data["count"].asInt();
        float          flow  = tank_data["flow"].asFloat();

        emitter.AddParticlesToReservoir( group, count, flow );
      }

    //
    // Setup the emission values.
    //
      DistributionSerializer serializer;
      Distribution<Vector4 > force_distribution;
      serializer.Serialize<Vector4 >( root["force"], force_distribution );

      emitter.SetEmissiveForce( force_distribution );

      // Return
      return true;
    }
};

}

#endif // NEBULAE_PARTICLE_EMITTERSERIALIZER_H__
