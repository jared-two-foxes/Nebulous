
#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Beta/Platform/Win32/Win32Platform.h>

#include <Model/Model.h>

#include <View/EntityRenderSystem.h>

#include "gtest/gtest.h"

using namespace Nebulae;
using namespace Game;


class EntityRenderSystemFixture : public ::testing::Test 
{
  private:
    std::shared_ptr<Platform >           m_platform;
    std::shared_ptr<Model >              m_model;
    std::shared_ptr<EntityRenderSystem > m_entityRenderer;

  protected:
    virtual void SetUp() 
    {
      m_platform = std::shared_ptr<Platform>( new Win32Platform() );
      m_platform->Initiate();

      Platform::FileSystemPtr   fileSystem   = m_platform->GetFileSystem();
      Platform::WindowPtr       window       = m_platform->CreateApplicationWindow( 0,0,800,600 );
      Platform::RenderSystemPtr renderDevice = m_platform->CreateRenderSystem( window );

      //m_componentCache.
      std::shared_ptr<TemplateCache > templateCache( new TemplateCache() );
      m_model = std::shared_ptr<Model >( new Model(templateCache) );

      // Todo.  Add extra variables, sprite atlas etc
      //m_entityRenderer = std::shared_ptr<EntityRenderSystem >( new EntityRenderSystem(fileSystem, renderDevice, m_model) );
    }

    virtual void TearDown() 
    {
    
    }

};

//
//TEST_F(EntityRenderSystemFixture, Render_Tint_CanApplyTint) 
//{
//  ASSERT_TRUE( false );
//}