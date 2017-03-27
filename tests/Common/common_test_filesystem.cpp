
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

#include "gtest/gtest.h"

using namespace Nebulae;


TEST(FileSystem, Open_RequestsValidFile_ShouldReturnFile) 
{
  //arrange
  FileSystem fs;
  fs.Mount( "disk", new DiskFileDevice("../../Samples/Media") );

  //act
  File* file = fs.Open( "disk", "entityTemplates.json", IO_MODE_IN );

  //assert
  ASSERT_NE( nullptr, file );
}