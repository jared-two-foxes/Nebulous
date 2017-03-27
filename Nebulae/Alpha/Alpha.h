#ifndef NEBULAE_ALPHA_H__
#define NEBULAE_ALPHA_H__

#include <Nebulae/Common/Common.h>

enum RenderSystemType
{
	OPENGL_3,
	OPENGL_4_0,
	VULCAN,
	GLES_2_0
};


namespace Nebulae {
	class RenderSystem;
}


std::shared_ptr<Nebulae::RenderSystem > 
CreateRenderSystem( RenderSystemType type, std::shared_ptr<Nebulae::FileSystem > fileSystem, std::shared_ptr<Nebulae::Window > window );


#endif //NEBULAE_ALPHA_H__