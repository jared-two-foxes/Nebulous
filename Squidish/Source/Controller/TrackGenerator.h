#ifndef CONTROLLER_TRACKGENERATOR_H__
#define CONTROLLER_TRACKGENERATOR_H__

#include <Nebulae/Common/Common.h>

#include <Model/Component/AspectComponent.h>
#include <Model/Component/BehaviourComponent.h>
#include <Model/Component/MotionComponent.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Component/ScoreComponent.h>

namespace Game {

class Entity;
class Model;

class TrackGenerator
///
/// The TrackGenerator class is responsible for spawning all the objects that exist in a run.
/// It uses a list of structs (TrackElement's) which describe a single "object" which should
/// be spawned along with all the information required to create that object, the depth at which
/// it should be created and the depth ahead of the current depth at which to create it.  
///
/// When attempting to spawn an instance of a TrackElement each of the NodeList objects is 
/// iterated an a random index into the NodeList is taken with the selected object spawned.  
/// That is when spawning a single element of each of the registered NodeLists is created.
///
{
public:
  enum Location 
  {
    LOCATION_LEFT    = 1,
    LOCATION_CENTER  = 2,
    LOCATION_RIGHT   = 4,
    LOCATION_TOP     = 8,
    LOCATION_BOTTOM  = 16
  };

  struct ElementNode 
  ///
  /// Defines a single object to be created including the type of Entity to create and values
  /// to assign to Components assigned to that entity. If a component is left null then 
  /// no values for that component type will be copied over, that means either the Entity 
  /// wants to use the default values for that component OR that the entity does not have 
  /// a component of that type.  Setting a component descriptor for an Entity type that does
  /// not have that component will not add that component to the entity.
  ///
  {
    std::string               name;                   ///< The name/identifier for the Entity template class.
    std::vector<ElementNode > children;               ///< List of children to include as part of this Element. 
    AspectComponent           aspectComponent;
    //AnimationComponent        animationComponent;
    BehaviourComponent        behaviourComponent;
    MotionComponent           motionComponent;
    PlacementComponent        placementComponent;
    ScoreComponent            scoreComponent;
  };

  typedef std::vector<ElementNode > NodeList;

  struct TrackElement 
  ///
  /// Describes an instance of a group of objects which are to generated in the world and 
  /// defines when it should be created.
  ///
  {
    Real                   nextDepth;    ///< The next depth at which to create a new Entity of this type.
    Real                   stride;       ///< The distance between instances of this element type.
    Real                   depthAhead;   ///< How far ahead of the current depth at which this object type should be created.
    float                  probability;  ///< probability of this element being added; range from [0,1]
    std::vector<NodeList > nodes;        ///< List of ElementNode lists each representing objects to create for this element.
  };

private:
  std::shared_ptr<Model >               m_model;            ///< Shared reference to Model object.
  std::vector<TrackElement >            m_trackElements;    ///< A list of elements for the track that can be made.
  std::mt19937                          m_generator;        ///< Random number generator
  std::uniform_real_distribution<Real > m_distribution;

  public:
    TrackGenerator( const std::shared_ptr<Model>& model );

    void Clear();
    void Init( Nebulae::File& file );
    void Process( const float distance );
    Entity* CreateEntity( const ElementNode& node, const Vector4& position );
};

}

#endif // CONTROLLER_TRACKGENERATOR_H__