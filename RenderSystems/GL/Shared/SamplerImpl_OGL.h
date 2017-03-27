#ifndef NEBULAE_SAMPLERIMPL_OGL_H__
#define NEBULAE_SAMPLERIMPL_OGL_H__

#include <GL/config.h>

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

		//functions
		virtual bool Load() override;

	};

} 

#endif // NEBULAE_SAMPLERIMPL_OGL_H__