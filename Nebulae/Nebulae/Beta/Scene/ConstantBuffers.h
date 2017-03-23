#ifndef CONSTANTBUFFERS_H__
#define CONSTANTBUFFERS_H__

#include <Nebulae/Common/Base/Base.h>

namespace Nebulae
{
	struct ObjectBuffer
	{
		Real World[16];									//<< World Matrix
		//Real InverseWorld[16];
	};

	struct DirectionalLight
	{
		Vector4	Colour;
		Vector4 Direction;
	};

	struct LightBuffer
	{
		DirectionalLight Lights[8];
		int LightCount;
		int padding[3];
	};

	struct SpriteBuffer
	{
		Vector4 uv;											///< [x,y] = min point uv coordinates; [z,w] = max point uv coordinates
		Vector4 dim;										///< [x,y] = min point offset offset; [z,w] = sprite size
	};

} //Nebulous

#endif // CONSTANTBUFFERS_H__