
#include <Nebulae/Common/Platform/Platform.h>
#include <Nebulae/Common/Platform/Win32/Win32Platform.h>

#include <Lua/LuaInterpreter.h>

#include "gtest/gtest.h"

using namespace Nebulae;


class LuaInterpreterFixture : public ::testing::Test 
{
  protected:
    LuaInterpreter* m_interpreter;

    protected:
      virtual void SetUp() 
      {
        m_interpreter = new LuaInterpreter();
        m_interpreter->Initialize();
      }

      virtual void TearDown() 
      {
        delete m_interpreter;
        m_interpreter = NULL;
      }

};


TEST_F(LuaInterpreterFixture, DoString_Parameter_EmptyStringReturnsTrue) 
{
  //arrange
  std::string emptyBuffer = "";

  //act
  bool result = m_interpreter->DoString( emptyBuffer.c_str() );

  //assert
  ASSERT_TRUE( result );
}

TEST_F(LuaInterpreterFixture, DoString_Parameter_EmptyStringCausesNoError) 
{
  //arrange
  std::string emptyBuffer = "";

  //act
  m_interpreter->DoString( emptyBuffer.c_str() );
  const std::string errorMessage = m_interpreter->GetErrorMessage();

  //assert
  ASSERT_EQ( errorMessage.length(), 0 );
}