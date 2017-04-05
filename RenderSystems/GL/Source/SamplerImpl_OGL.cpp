#include "SamplerImpl_OGL.h"

using namespace Nebulae;

//constructor
SamplerImpl_OGL::SamplerImpl_OGL()
	: Sampler::Impl()
{

}

//destructor
SamplerImpl_OGL::~SamplerImpl_OGL()
{

}

//Load
bool SamplerImpl_OGL::Load()
{
/*
	//
	//@todo Currently forces the sampler state into a linear state.
	//

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = m_pDevice->CreateSamplerState( &sampDesc, &m_pSampler );

	return SUCCEEDED(hr);
*/

	return true;
}