#include "vpch.h"
#include "Sampler.h"
#include "Render/RenderUtils.h"

void Sampler::Create()
{
	RenderUtils::CreateSamplerState(data);
}
