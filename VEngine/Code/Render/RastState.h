#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>

class RastState
{
public:
	RastState(std::string_view name_, D3D11_RASTERIZER_DESC desc);
	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }
	auto GetName() { return name; }

private:
	std::string name;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> data;
};

