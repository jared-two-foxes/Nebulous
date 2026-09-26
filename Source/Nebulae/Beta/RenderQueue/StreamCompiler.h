#ifndef NEBULAE_BETA_RENDERQUEUE_STREAMCOMPILER_H_
#define NEBULAE_BETA_RENDERQUEUE_STREAMCOMPILER_H_

#include <Nebulae/Beta/RenderQueue/DrawItemList.h>
#include <Nebulae/Beta/RenderQueue/UniformProvider.h>
#include <Nebulae/Alpha/RenderStream/RenderStream.h>

namespace Nebulae
{
class Camera;

/// Creates one immutable-to-the-interpreter command stream for a single view.
UniformProvider MakeCameraProvider( const Camera* camera );

class StreamCompiler
{
public:
  void Compile( DrawItemList items, const Camera* camera, RenderStream& stream );
};
} // namespace Nebulae

#endif
