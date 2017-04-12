
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/DiskFileDevice.h>
#include <Nebulae/Common/FileSystem/ZipFileDevice.h>

#include <Nebulae/Alpha/Texture/Image.h>

#include "gtest/gtest.h"

using namespace Nebulae;



class ImageFixture : public ::testing::Test 
{
protected:
  FileSystem m_fileSystem;

  protected:
    virtual void SetUp() 
    {
      m_fileSystem.Mount( "disk", new DiskFileDevice("../../tests/Assets") );
    };

};

TEST_F(ImageFixture, Load_ValidStreamParameter_ShouldContainPixelData) 
{
  //arrange
  Image      image;

  //act
  File* is = m_fileSystem.Open( "disk", "white.png" );
  EXPECT_TRUE( is != NULL );
  image.load( *is );
  
  //assert
  ASSERT_TRUE( image.getData() != NULL );
}

TEST_F(ImageFixture, Load_UninitializedStreamParameter_ShouldNotContainPixelData) 
{
  //arrange
  Image image;
  File  is;

  //act
  image.load( is );
  
  //assert
  ASSERT_TRUE( image.getData() == NULL );
}

TEST_F(ImageFixture, Load_InvalidStreamParameter_ShouldNotContainPixelData) 
{
  //arrange
  Image      image;

  //act
  File* is = m_fileSystem.Open( "disk", "fonts/consola.ttf" );
  EXPECT_TRUE( is != NULL );
  image.load( *is );
  
  //assert
  ASSERT_TRUE( image.getData() == NULL );
}

TEST_F(ImageFixture, GetColourAt_ValidParameters_ShouldReturnWhiteColour) 
{
  //arrange
  File*      is = m_fileSystem.Open( "disk", "white.png" );
  EXPECT_TRUE( is != NULL );
  Image      image;
  image.load( *is );
  
  //act
  Colour colour = image.getColourAt(80,30); ///< The center of the image is white.  Has a curved edge with a blueish border.

  //assert
  ASSERT_EQ( CLR_WHITE, colour );
}

TEST_F(ImageFixture, GetColourAt_OutOfBoundsParameters_ShouldReturnZeroColour) 
{
  //arrange
  File*      is = m_fileSystem.Open( "disk", "white.png" );
  EXPECT_TRUE( is != NULL );
  Image      image;
  image.load( *is );
  
  //act
  Colour colour = image.getColourAt(600,600);

  //assert
  ASSERT_EQ( CLR_ZERO, colour );
}

TEST_F(ImageFixture, GetColourAt_ValidParameters_ShouldReturnAlphaValue) 
{
  //arrange
  File*      is = m_fileSystem.Open( "disk", "white.png" );
  EXPECT_TRUE( is != NULL );
  Image      image;
  image.load( *is );
  
  //act
  Colour colour = image.getColourAt(80,30);

  //assert
  ASSERT_GT( colour.a, 0 );
}

TEST_F(ImageFixture, GetColourAt_ValidParameters_ShouldReturnNoAlphaValue) 
{
  //arrange
  File*      is = m_fileSystem.Open( "disk", "white.png" );
  EXPECT_TRUE( is != NULL );
  Image      image;
  image.load( *is );
  
  //act
  Colour colour = image.getColourAt(0,0);

  //assert
  ASSERT_EQ( 0, colour.a );
}

TEST_F(ImageFixture, GetColourAt_Gradient_TopCoordinateShouldReturnWhite) 
{
  //arrange
  File*      is = m_fileSystem.Open( "disk", "gradient.png" );
  EXPECT_TRUE( is != NULL );
  Image      image;
  image.load( *is );
  
  //act
  Colour colour = image.getColourAt(0,399);

  //assert
  ASSERT_EQ( CLR_WHITE, colour );
}

TEST_F(ImageFixture, GetColourAt_Gradient_BottomCoordinatesShouldReturnBlack) 
{
  //arrange
  File*      is = m_fileSystem.Open( "disk", "gradient.png" );
  EXPECT_TRUE( is != NULL );
  Image      image;
  image.load( *is );
  
  //act
  Colour colour = image.getColourAt(0,0);

  //assert
  ASSERT_EQ( CLR_BLACK, colour );
}