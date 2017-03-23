
#include <Model/Model.h>
#include <Model/Core/Entity.h>
#include <Model/Component/CollisionComponent.h>
#include <Model/Component/PlacementComponent.h>
#include <Model/Systems/CollisionSystem.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>

#include "gtest/gtest.h"


using namespace Nebulae;
using namespace Game;


class CollisionSystemTestFixture : public ::testing::Test {
  public:
    struct Colliders {
      const Entity first;
      const Entity second;
      
      Colliders( const Entity& obj1, const Entity& obj2 ) 
        : first(obj1), second(obj2) {}
    };

  protected:
    std::shared_ptr<Platform >           m_platform;
    std::shared_ptr<Model >              m_model;
    std::shared_ptr<SpriteAtlasManager > m_atlasManager;
    std::shared_ptr<CollisionSystem >    m_collisionSystem;
    

    std::vector<Colliders* > m_colliders;
    bool                     m_collisionDetected;

  protected:
    virtual void SetUp() 
    {
      m_platform = std::shared_ptr<Platform>( new Win32Platform() );
      m_platform->Initiate();

      Platform::FileSystemPtr         fileSystem = m_platform->GetFileSystem();
      File*                           is         = fileSystem->Open( "disk", "entityTemplates.json" );
      std::shared_ptr<TemplateCache > templateCache(new TemplateCache());
      if( is != NULL ) 
      {
        templateCache->Load( *is );
      }

      m_model        = std::shared_ptr<Model>( new Model(templateCache) );
      m_atlasManager = std::shared_ptr<SpriteAtlasManager >( new SpriteAtlasManager(fileSystem, NULL) );
      m_model->Init();
      //m_atlasManager->Initiate();
  
      m_collisionSystem = std::shared_ptr<CollisionSystem >( new CollisionSystem(m_model, m_atlasManager) );
      m_collisionSystem->CollidedSignal.connect( boost::bind(&CollisionSystemTestFixture::OnCollision, this, _1, _2) );
      
      m_collisionDetected = false;
    };

    virtual void TearDown() 
    {
      std::for_each( m_colliders.begin(), m_colliders.end(), [](Colliders* collider) { delete collider; });
      m_colliders.clear();
    }

    void CreateSingleEntity(std::vector<Entity* >& entities) 
    {
      Entity* entity = NULL;
      std::vector<PlacementComponent* > placementComponents;
      std::vector<CollisionComponent* > collisionComponents;

      entities.push_back( m_model->CreateEntity("reef") );
      entity = entities.back();
      
      m_model->GetComponents<PlacementComponent >( entity, &placementComponents );
      m_model->GetComponents<CollisionComponent >( entity, &collisionComponents );
      
      placementComponents[0]->position    = Vector4(0,0,0,0);
      collisionComponents[0]->halfExtents = Vector4(10,10,0,0);
    
      m_collisionSystem->SetPlayerEntity( entity );
    }

    void CreateOverlappingEntities(std::vector<Entity* >& entities) 
    {
      Entity* entity = NULL;
      std::vector<PlacementComponent* > placementComponents;
      std::vector<CollisionComponent* > collisionComponents;

      entities.push_back( m_model->CreateEntity("reef") );
      entity = entities.back();
      
      m_model->GetComponents<PlacementComponent >( entity, &placementComponents );
      m_model->GetComponents<CollisionComponent >( entity, &collisionComponents );
      
      placementComponents[0]->position    = Vector4(0,0,0,0);
      collisionComponents[0]->halfExtents = Vector4(10,10,0,0);
      

      entities.push_back( m_model->CreateEntity("reef") );
      entity = entities.back();
      
      placementComponents.clear(); collisionComponents.clear();
      m_model->GetComponents<PlacementComponent >( entity, &placementComponents );
      m_model->GetComponents<CollisionComponent >( entity, &collisionComponents );

      placementComponents[0]->position    = Vector4(5,5,0,0);
      collisionComponents[0]->halfExtents = Vector4(10,10,0,0);

      m_collisionSystem->SetPlayerEntity( entity );
    }

    void OnCollision( const Entity& obj1, const Entity& obj2 ) 
    {
      m_colliders.push_back( new Colliders(obj1,obj2) );
      m_collisionDetected = true;
    }

};


TEST_F(CollisionSystemTestFixture, DoesNotDetectCollisionsWithTheSameEntity) 
{
  //arrange
  std::vector<Entity* > entities;
  CreateSingleEntity( entities );
  
  //act
  m_collisionSystem->Process( entities, 0.0f );

  //assert
  ASSERT_FALSE( m_collisionDetected );
}


TEST_F(CollisionSystemTestFixture, DoesNotDetectCollisionsWithTheDuplicateEntity) 
{
  //arrange
  std::vector<Entity* > entities;
  CreateSingleEntity( entities );
  entities.push_back( entities.back() );
  
  //act
  m_collisionSystem->Process( entities, 0.0f );

  //assert
  ASSERT_FALSE( m_collisionDetected );
}

TEST_F(CollisionSystemTestFixture, DoesDetectsCollisionWithPlayer) 
{
  //arrange
  std::vector<Entity* > entities;
  CreateOverlappingEntities( entities );

  //act
  m_collisionSystem->Process( entities, 0.0f );

  //assert
  ASSERT_TRUE( m_collisionDetected );
}
