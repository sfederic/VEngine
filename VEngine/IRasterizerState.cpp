#include "IRasterizerState.h"
#include <assert.h>

void D3D11RasterizerState::Create()
{
}

void* D3D11RasterizerState::Get()
{
    assert(data);
    return data;
}

void D3D12RasterizerState::Create()
{

}

void* D3D12RasterizerState::Get()
{
    assert(data);
    return data;
}
