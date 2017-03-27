
#include <Nebulae/Common/Common.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(Logger, Log_EmptyStringParameter_IsNotAddedToLoggedMessages) 
{
  //arrange
  Logger logger;

  //act
  logger.Log( "" );
  
  //assert
  ASSERT_EQ( std::size_t(0), logger.GetMessages().size() );
}


TEST(Logger, Log_NewLineString_IsAddedToLoggedMessages) 
{
  //arrange
  Logger logger;

  //act
  logger.Log( "\n" );
  
  //assert
  ASSERT_EQ( std::size_t(1), logger.GetMessages().size() );
}

TEST(Logger, Log_StandardString_AddedToMessages) 
{
  //arrange
  Logger logger;

  //act
  logger.Log( "Log a message!" );
  
  //assert
  ASSERT_EQ( std::size_t(1), logger.GetMessages().size() );
}

TEST(Logger, Log_StandardStringWithExtraStringWhichIsNotReferenced_UnmodifiedStringAddedToMessages) 
{
  //arrange
  Logger logger;

  //act
  logger.Log( "This message has numbers 123456", "second" );
  
  //assert
  EXPECT_EQ( std::size_t(1), logger.GetMessages().size() );

  if( logger.GetMessages().size() > 0 )
  {
    ASSERT_STREQ( logger.GetMessages()[0].c_str(), "This message has numbers 123456" );
  }
}

TEST(Logger, Log_StandardStringWithExtraString_AddedToMessages) 
{
  //arrange
  Logger logger;

  //act
  logger.Log( "Log a '%s' message!", "second" );
  
  //assert
  EXPECT_EQ( std::size_t(1), logger.GetMessages().size() );

  if( logger.GetMessages().size() > 0 )
  {
    ASSERT_STREQ( logger.GetMessages()[0].c_str(), "Log a 'second' message!" );
  }
}

TEST(Logger, Log_StandardStringWithExtraNumbers_AddedToMessages) 
{
  //arrange
  Logger logger;

  //act
  logger.Log( "This message has numbers %d%d%d", 7, 8, 9  );
  
  //assert
  EXPECT_EQ( std::size_t(1), logger.GetMessages().size() );

  if( logger.GetMessages().size() > 0 )
  {
    ASSERT_STREQ( logger.GetMessages()[0].c_str(), "This message has numbers 789" );
  }
}
