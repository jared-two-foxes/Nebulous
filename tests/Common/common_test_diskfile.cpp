
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFile.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(DiskFile, Read_ValidFileAndSize_ShouldReturnSize) 
{
  //arrange
  DiskFile file( "../../Samples/Media/entityTemplates.json" );
    
  //act
  char buffer[8]; 
  size_t amountRead = file.Read( buffer, 8 );

  //assert
  ASSERT_EQ( std::size_t(8), amountRead );
}

TEST(DiskFile, Read_RequestToMuchData_ShouldReturnSizeStreamed) 
{
  //arrange
  DiskFile file( "../../Samples/Media/entityTemplates.json" );
  file.SeekToEnd();
	std::size_t size = file.Tell();
  file.Seek( 0 );  

  //act
  char buffer[512]; 
  EXPECT_GT( 512u, size );
	std::size_t amountRead = file.Read( buffer, 512 );
   
  //assert
  ASSERT_EQ( size, amountRead );
}

TEST(DiskFile, Write_WritesDataToFile_ShouldReturnSizeStreamed) 
{
  //arrange
  DiskFile file( "../../Samples/Media/test_output.json", false );
    
  //act
  char buffer[256];
  sprintf( buffer, "%s", "This is a test string" ); 
	std::size_t length = strlen( &buffer[0] );
	std::size_t amountWriten = file.Write( buffer, length );

  //assert
  ASSERT_EQ( length, amountWriten );
}