#pragma once

#include <d3d11.h>
#include <wrl.h>

//@Todo: srv wrapper is unused in the engine?
class ShaderResourceView
{
public:
	ShaderResourceView(D3D11_SHADER_RESOURCE_VIEW_DESC desc);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> data;
};
