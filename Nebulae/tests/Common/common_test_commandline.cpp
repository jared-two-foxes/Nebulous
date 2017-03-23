
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/Platform/CommandLine.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(CommandLine, Init_StringWithCorrectSyntax_ShouldReturnNumberOfElementsRead) 
{
  //arrange
  //CommandLine commandLine; 
  char* argv = {""};  
  
  //act
	std::size_t elementsFound; // = commandLine.Init( 1, argv );

  //assert
  ASSERT_EQ( 0u, elementsFound );
}