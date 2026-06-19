#ifndef NEBULAE_COMMON_PLATFORM_IOS_PLATFORM_IOS_H_
#define NEBULAE_COMMON_PLATFORM_IOS_PLATFORM_IOS_H_

#include <Nebulae/Beta/Platform/Platform.h>
#include <Nebulae/Common/Base/Base.h>


namespace Nebulae
{

class Platform_iOS : public Platform
{
public:
  Platform_iOS();
  virtual ~Platform_iOS();

  virtual bool Initiate();
  virtual void Destroy();

  virtual Window* CreateApplicationWindow( int x, int y, int w, int h, Window* pParent = NULL );
};

} // namespace Nebulae

#endif // NEBULAE_COMMON_PLATFORM_IOS_PLATFORM_IOS_H_