#ifndef NEBULAE_RENDERSYSTEMS_GL_INCLUDES_SAMPLERIMPL_OGL_H_
#define NEBULAE_RENDERSYSTEMS_GL_INCLUDES_SAMPLERIMPL_OGL_H_

#include <Config.h>

#include <Nebulae/Alpha/Sampler/Sampler.h>

namespace Nebulae
{

/** SamplerImpl_OGL.
 */
class SamplerImpl_OGL : public Sampler::Impl
{
protected:
public:
  SamplerImpl_OGL();
  virtual ~SamplerImpl_OGL();

  // functions
  virtual bool Load() override;
};

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GL_INCLUDES_SAMPLERIMPL_OGL_H_