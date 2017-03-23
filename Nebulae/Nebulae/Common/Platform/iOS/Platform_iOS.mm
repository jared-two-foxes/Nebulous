
#include "Platform_iOS.h"

#include <Nebulae/Common/Window/iOS/Window_iOS.h>
#include <Nebulae/Common/FileSystem/IOSFileSystemArchive.h>


using namespace Nebulae;


Platform_iOS::Platform_iOS()
  : Platform()
{

}

Platform_iOS::~Platform_iOS()
{

}

bool
Platform_iOS::Initiate()
{
  // @note [jared.watt] This is here rather than in Platform::Intiate so that we dont have to call
  // Platform::Initiate() as thats an ugly way of coding and I cant think of a better solution 
  // right now :D
  m_fileSystem = new IOSFileSystemArchive();

  return true;
}

void
Platform_iOS::Destroy()
{

}

Window*
Platform_iOS::CreateApplicationWindow(int x, int y, int w, int h, Window* pParent )
{
  Window* pWindow = new Window_iOS();
  if( pWindow ) {
    pWindow->Initiate();
  }

  return pWindow;
}
