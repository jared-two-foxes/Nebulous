
#include "gtest/gtest.h"

int main( int argc, char* argv[] ) {
  _chdir( "../../Sampless" );
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}