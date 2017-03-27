#ifndef __NEBULAE_GLES2SAMPLERIMPL_H__
#define __NEBULAE_GLES2SAMPLERIMPL_H__

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

		//functions
		virtual bool Load() override;

	}; //GLES2SamplerImpl

} //Nebulae

#endif // __NEBULAE_GLES2SAMPLERIMPL_H__