
#include "Platform.h"

namespace impl
{
	std::shared_ptr<Nebulae::Platform > CreatePlatformImpl();
}

#if NEBULAE_PLATFORM == NEBULAE_PLATFORM_WIN32
  #include <Nebulae/Common/Platform/Win32/Win32Platform.h>
  std::shared_ptr<Nebulae::Platform > impl::CreatePlatformImpl()
  { return std::make_shared<Nebulae::Win32Platform>(); }
#elif NEBULAE_PLATFORM == NEBULAE_PLATFORM_APPLE
  #include "OSX/AppDelegate.h"
#elif NEBULAE_PLATFORM == NEBULAE_PLATFORM_APPLE_IOS
#ifdef __OBJC__
  #import <UIKit/UIKit.h>
  #include "iOS/AppDelegate.h"
#endif
#endif


std::shared_ptr<Nebulae::Platform > Nebulae::CreateAndInitiatePlatform()
{ return impl::CreatePlatformImpl(); }