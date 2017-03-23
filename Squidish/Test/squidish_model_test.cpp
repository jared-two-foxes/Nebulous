
#include "Model/Model.h"
#include "Model/Core/Entity.h"

#include <Nebulae/Common/FileSystem/DiskFileDevice.h>

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>

#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


class ModelTestFixture : public ::testing::Test {
  protected:
    std::shared_ptr<Platform > m_platform;
    std::shared_ptr<Model >    m_model;

  protected:
    virtual void SetUp() 
    {
      m_platform = std::shared_ptr<Platform>( new Win32Platform() );
      m_platform->Initiate();

      Platform::FileSystemPtr fileSystem = m_platform->GetFileSystem();
      fileSystem->Mount( "disk", new DiskFileDevice("../UncompressedAssets") );
      File*                   is         = fileSystem->Open( "disk", "entityTemplates.json" );
      
      std::shared_ptr<TemplateCache> templateCache(new TemplateCache());
      if( is != NULL ) {
        templateCache->Load( *is );
      }

      m_model = std::shared_ptr<Model >( new Model(templateCache) );

      m_model->Init();
    };

};


TEST_F(ModelTestFixture, CreateEntity_NoArguments_ShouldCreateEntity) 
{
  //act
  Entity* entity = m_model->CreateEntity( "" );

  //assert
  ASSERT_TRUE( entity != NULL );
}


TEST_F(ModelTestFixture, CreateEntity_NoUniqueName_ShouldBeEmptyIfNotSet) 
{
  //act
  Entity* entity = m_model->CreateEntity( "static" );

  //assert
  ASSERT_STREQ( entity->GetUniqueName(), "" );
}


TEST_F(ModelTestFixture, CreateEntity_UniqueName_ShouldSetUniqueIdentifier) 
{
  //act
  const char* identifier = "identifier";
  Entity*     entity     = m_model->CreateEntity( "static", identifier );

  //assert
  ASSERT_STREQ( entity->GetUniqueName(), identifier );
}

TEST_F(ModelTestFixture, CreateEntity_ValidTemplateName_ShouldAddTemplateComponents) 
{
  //act
  Entity* entity = m_model->CreateEntity("static");

  //assert
  int32 count = entity->GetLinkedComponentIndices( COMPONENT_TYPE_PLACEMENT, NULL );
  ASSERT_GT( count, 0 );
  count = entity->GetLinkedComponentIndices( COMPONENT_TYPE_ASPECT, NULL );
  ASSERT_GT( count, 0 );
}


TEST_F(ModelTestFixture, FindEntityByName_EmptyString_ShouldReturnNull) 
{
  //act
  Entity* entity = m_model->FindEntityByName( "" );

  //assert
  ASSERT_TRUE( entity == NULL );
}


TEST_F(ModelTestFixture, FindEntityByName_InvalidString_ShouldReturnNull) 
{
  //act
  const char* identifier = "identifier";
  Entity*     entity     = m_model->FindEntityByName( identifier );

  //assert
  ASSERT_TRUE( entity == NULL );
}


TEST_F(ModelTestFixture, FindEntityByName_ValidIdentifier_ShouldReturnEntity) 
{
  // arrange  
  const char* identifier = "identifier";
  m_model->CreateEntity( "static", identifier );

  //act
  Entity* entity = m_model->FindEntityByName( identifier );

  //assert
  ASSERT_TRUE( entity != NULL );
  ASSERT_STREQ( entity->GetUniqueName(), identifier );
}


TEST_F(ModelTestFixture, Clear_ShouldDestroyAllEntities) 
{
  // arrange  
  m_model->CreateEntity( "static" );
  m_model->CreateEntity( "static" );
  m_model->CreateEntity( "static" );

  //act
  m_model->Clear();

  //assert
  ASSERT_TRUE( m_model->GetEntities().size() == 0 );
}


//TEST_F(ModelTestFixture, LoadSceneFileHandlesEmptyPathString) {
//  //act
//  m_model->LoadSceneFile("");
//
//  //assert
//  ASSERT_EQ( 0, m_model->GetEntities().size() );
//}


//TEST(Model, LoadSceneFileHandlesValidPathString) {
//  // arrange
//  Model model;
//  model.Init();
//
//  //act
//  //model.LoadSceneFile("level/main.json");
//
//  //assert
//  ASSERT_EQ( 1, model.GetEntities().size() );
//}