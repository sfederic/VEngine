#include "IRasterizerState.h"
#include <assert.h>

void D3D11RasterizerState::Create()
{
}

void* D3D11RasterizerState::Get()
{
    if (data)
    {
        return data;
    }

    return nullptr;
}

void D3D12RasterizerState::Create()
{

}

void* D3D12RasterizerState::Get()
{
    if (data)
    {
        return data;
    }

    return nullptr;
}
