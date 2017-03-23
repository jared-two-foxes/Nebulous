
#include "TrackGenerator.h"

#include <Model/Model.h>

#include <Model/Component/AspectComponent.h>
#include <Model/Component/BehaviourComponent.h>
#include <Model/Component/MotionComponent.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Component/ScoreComponent.h>

#include <Model/Core/Entity.h>

#include <Nebulae/Beta/Scene/SpriteAtlasUtils.h>

using namespace Game;
using namespace Nebulae;


class TrackElementSerializer
{
  public:
    bool Read( const Json::Value& value, std::vector<TrackGenerator::TrackElement >& elementsOut )
    {
      Json::Value elements = value["elements"];

      for( int i = 0, n = elements.size(); i < n; ++i )
      {
        elementsOut.push_back( TrackGenerator::TrackElement() );
        ReadElement( elements[i], elementsOut.back() );
      }

      return true;
    }

    bool ReadElement( const Json::Value& value, TrackGenerator::TrackElement& elementOut )
    {
      elementOut.nextDepth   = json_cast_with_default<Real >( value["next_depth"], 512.0f );
      elementOut.stride      = json_cast_with_default<Real >( value["stride"], 512.0f );
      elementOut.depthAhead  = json_cast_with_default<Real >( value["depth_ahead"], 512.0f );
      elementOut.probability = json_cast_with_default<Real >( value["probability"], 1.0f );

      Json::Value nodes = value["nodes"];
      NE_ASSERT( nodes.isArray(), "Expected an array of an array of nodes." )();

      for( int i = 0, n = nodes.size(); i<n; ++i )
      {
        NE_ASSERT( nodes[i].isArray(), "Expected an array of nodes" )();

        elementOut.nodes.push_back( TrackGenerator::NodeList() );
        ReadNodeList( nodes[i], elementOut.nodes.back() );
      }

      return true;
    }

    bool ReadNodeList( const Json::Value& value, TrackGenerator::NodeList& nodeListOut )
    {
      for( int i = 0, n = value.size(); i<n; ++i )
      {
        nodeListOut.push_back( TrackGenerator::ElementNode() );       
        ReadNode( value[i], nodeListOut.back() );
      }      

      return false;
    }

    template <class T >
    void LoadComponent( const Json::Value& value, T& component )
    {
      const Json::Value& data = value[ component_loader<T >::component_name() ];
      if( !data.isNull() )
      {
        component_loader<T >::load( data, component );
      }
    }

    bool ReadNode( const Json::Value& value, TrackGenerator::ElementNode& node )
    {   
      node.name = json_cast_with_default<std::string >( value["name"], "" );

      // Load Component data.
      LoadComponent( value, node.aspectComponent );
      LoadComponent( value, node.behaviourComponent );
      LoadComponent( value, node.motionComponent );
      LoadComponent( value, node.placementComponent );
      LoadComponent( value, node.scoreComponent );

      Json::Value children = value["children"];

      for( int j = 0, m = children.size(); j < m; ++j )
      {
        node.children.push_back( TrackGenerator::ElementNode() );
        ReadNode( children[j], node.children.back() );
      }
   
      return false;
    }

};


// -----------------------------------------------------------------------------------------------------

TrackGenerator::TrackGenerator( const std::shared_ptr<Model>& model )
: m_model( model ),
  m_distribution(0,1.0f)
{}

void
TrackGenerator::Clear() 
{
  m_trackElements.clear();
}

void
TrackGenerator::Init( File& file ) 
{
  Json::Value root;
  ParseJSON( file, &root );

  TrackElementSerializer serializer;
  serializer.Read( root, m_trackElements );
}

void
TrackGenerator::Process( const float distance ) 
{
//
// Check if we should create a track element.
//
  std::vector<TrackElement >::iterator it, it_end = m_trackElements.end();
  for( it = m_trackElements.begin(); it != it_end; ++it ) 
  {
    TrackElement& element = (*it);

    while( distance - element.depthAhead < element.nextDepth ) 
    {
      for( std::vector<NodeList >::iterator node_itr = (*it).nodes.begin(); node_itr != (*it).nodes.end(); ++node_itr )
      {
        float p = m_distribution( m_generator );
        
        if( p <= element.probability )
        { 
          int32 index = 0;

          if( (*node_itr).size() > 0 )
          {
            // Randomly select the ElementNode for the NodeList to create.
            std::uniform_int_distribution<int32 > distribution( 0, (*node_itr).size() - 1 );
            index = distribution(m_generator);
          }
          
          CreateEntity( (*node_itr)[index], Vector4(0, element.nextDepth, 0, 0) );
        }
      }

      //@todo [jared.watt 22/11/2013] Need a better algorithm for placement of next element.
      element.nextDepth -= element.stride;
    }
  }
}

Entity* 
TrackGenerator::CreateEntity( const ElementNode& node, const Vector4& position )
{
  Entity* entity = m_model->CreateEntity( node.name.c_str() );
  NE_ASSERT( entity, "Unable to create an Entity of template type %s" )( node.name.c_str() );
  if( entity ) 
  {
    // Set the appearance.
    std::vector<AspectComponent* >    aspectComponents;    
    if( m_model->GetComponents<AspectComponent>(entity, &aspectComponents) )
    {
	  aspectComponents[0]->alignment = node.aspectComponent.alignment;
      aspectComponents[0]->atlasName = node.aspectComponent.atlasName;
      aspectComponents[0]->frameName = node.aspectComponent.frameName;
      aspectComponents[0]->flags     = node.aspectComponent.flags;
    }
      
    // Set the position & rotation.
    std::vector<PlacementComponent* > placementComponents; 
    if( m_model->GetComponents<PlacementComponent>(entity, &placementComponents) )
    {
      placementComponents[0]->rotation = node.placementComponent.rotation; 
      placementComponents[0]->position = node.placementComponent.position + position;
    }
    
    // Apply a MotionComponent if any are described      
    std::vector<MotionComponent* > motionComponents; 
    if( m_model->GetComponents<MotionComponent>(entity, &motionComponents) )
    {
      motionComponents[0]->velocity        = node.motionComponent.velocity;
      motionComponents[0]->maximumVelocity = node.motionComponent.maximumVelocity;
    }
    
    // Apply a ScoreComponent if any are described      
    std::vector<ScoreComponent* > scoreComponents; 
    if( m_model->GetComponents<ScoreComponent>(entity, &scoreComponents) )
    {
      scoreComponents[0]->bonus = node.scoreComponent.bonus;
    }
    
    std::vector<BehaviourComponent* > behaviourComponents; 
    if( m_model->GetComponents<BehaviourComponent>(entity, &behaviourComponents) )
    {
      behaviourComponents[0]->m_attractionStrength        = node.behaviourComponent.m_attractionStrength;
      behaviourComponents[0]->m_attractionInfluenceRadius = node.behaviourComponent.m_attractionInfluenceRadius;
    }
    
    // Create any children nodes.
    if( !node.children.empty() )
    {
      std::for_each( node.children.begin(), node.children.end(), [&](const ElementNode& node){
        CreateEntity( node, position );
      });
    }

    //@todo add in some way of filtering appearances so that we can swap between the left and right flags for the reefs.
  }

  return entity;
}