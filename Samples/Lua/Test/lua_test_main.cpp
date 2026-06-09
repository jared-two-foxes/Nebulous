
#include "gtest/gtest.h"
#include <iostream>

int main( int argc, char* argv[] )
{
  _chdir( "../../Samples/Squidish/Assets" );
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}