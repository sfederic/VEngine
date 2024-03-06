#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>

class BlendState
{
public:
	BlendState(std::string_view name_, D3D11_BLEND_DESC desc);
	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	std::string name;
	Microsoft::WRL::ComPtr<ID3D11BlendState> data;
};
