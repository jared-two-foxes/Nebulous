
#include <Nebulae/Common/Common.h>
#include <Nebulae/Common/FileSystem/FileSystem.h>

#include <Nebulae/Audio/AudioBackend.h>


#include "gtest/gtest.h"

using namespace Nebulae;



class AudioBackendFixture : public ::testing::Test {
  protected:
    std::shared_ptr<FileSystem > fileSystem;
    std::shared_ptr<AudioBackend >      audioBackend;

  protected:
    virtual void SetUp() 
    {
      fileSystem   = std::shared_ptr<FileSystem >( new FileSystem() );
      audioBackend = std::shared_ptr<AudioBackend >( new AudioBackend(fileSystem) );
    };

    virtual void TearDown() 
    {
      fileSystem.reset();
    }

};

