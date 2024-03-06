#include "vpch.h"
#include "Sampler.h"
#include "Render/RenderUtils.h"

void Sampler::Create(D3D11_SAMPLER_DESC desc)
{
	RenderUtils::CreateSamplerState(desc, data);
}
