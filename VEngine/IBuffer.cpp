#include "IBuffer.h"
#include "RenderSystem.h"

void D3D12Buffer::Create()
{

}

void D3D11Buffer::Create()
{
    
}

void* D3D11Buffer::Get()
{
    assert(data);
    return (ID3D11Buffer*)data;
}

void* D3D12Buffer::Get()
{
    assert(data);
    return (ID3D12Resource*)data;
}

