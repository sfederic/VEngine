#include "ISampler.h"
#include <assert.h>

void D3D11Sampler::Create()
{
}

void* D3D11Sampler::Get()
{
    assert(data);
    return data;
}

void D3D12Sampler::Create()
{
}

void* D3D12Sampler::Get()
{
    assert(data);
    return data;
}
