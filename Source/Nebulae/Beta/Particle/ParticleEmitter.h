#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae
{

class ParticleGroup;
class ParticleSystem;

class ParticleEmitter
///
/// Encapsulates an object which emits a number of Particles of various types, 
/// where each different type is defined by a ParticleGroup, and a location 
/// in 3-Dimensional space from which the particles are spawned.
///
/// The primary responsibility of an Emitter is to determine the rate at which
/// particles are to be emitted into the game universe from its various particle
/// reservoir's.
///
{
private:
  struct ReservoirElement
  {
    ParticleGroup*  m_group;
    uint32          m_tank;
    Real            m_flow;
    Real            m_fraction;
    Vector4         m_initialscale;
  };

  
private:
  std::string                    m_name;                 ///< An identifier for the emitter.
  bool                           m_active;               ///< Is the emitter currently emitting particles.
  Vector4                        m_position;             ///< Position of the emitter.
  Quaternion                     m_rotation;             ///< Orientation of the emitter.
  Distribution<Vector4 >         m_emissionForce;        ///< Distribution of valid forces to apply to particles that are emitted.
  std::vector<ReservoirElement > m_reservoir;            ///< List of elements currently residing in the reservoir.

  public:
    ParticleEmitter( const std::string& name );
    ~ParticleEmitter();

    //Mutators
    void Clear();
    void SetPosition( const Vector4& position );
    void SetRotation( const Quaternion& rotation );
    void SetEmissiveForce( const Distribution<Vector4 >& force );
    void Start();
    void Pause();
    void AddParticlesToReservoir( ParticleGroup* group, uint32 count, Real flow );
    void Update( const uint64 elapsed );

    //Accessors
    const Vector4&    GetPosition() const;
    const Quaternion& GetRotation() const;
    bool              IsEmpty() const;
    bool              IsActive() const;

}; //ParticleEmitter

}

#endif // __PARTICLEEMITTER_H__