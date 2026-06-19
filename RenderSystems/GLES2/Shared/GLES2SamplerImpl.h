#ifndef NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2SAMPLERIMPL_H_
#define NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2SAMPLERIMPL_H_

#include <GLES2/config.h>
#include <Nebulae/Alpha/Sampler/Sampler.h>

namespace Nebulae
{

/**
 *	GLES2SamplerImpl.
 */
class GLES2SamplerImpl : public Sampler::Impl
{
public:
  GLES2SamplerImpl();
  virtual ~GLES2SamplerImpl();

  // functions
  virtual bool Load() override;

}; // GLES2SamplerImpl

} // namespace Nebulae

#endif // NEBULAE_RENDERSYSTEMS_GLES2_SHARED_GLES2SAMPLERIMPL_H_