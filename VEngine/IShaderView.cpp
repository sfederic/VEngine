#include "IShaderView.h"

D3D11ShaderView::D3D11ShaderView(ID3D11ShaderResourceView* srv)
{
    data = srv;
}

void D3D11ShaderView::Create()
{
}

void D3D12ShaderView::Create()
{
}
