#ifndef __NEBULAE_PLATFORM_iOS_H__
#define __NEBULAE_PLATFORM_iOS_H__

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

#endif // __NEBULAE_PLATFORM_iOS_H__