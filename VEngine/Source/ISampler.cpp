#include "ISampler.h"
#include <assert.h>
#include "RenderSystem.h"

D3D11Sampler::D3D11Sampler()
{

}

void D3D11Sampler::Create()
{
    gRenderSystem->CreateSamplerState(this);
}

void* D3D11Sampler::Get()
{
    assert(data);
    return data;
}

D3D12Sampler::D3D12Sampler()
{
}

void D3D12Sampler::Create()
{
}

void* D3D12Sampler::Get()
{
    assert(data);
    return data;
}
