#pragma once

#include <d3d11.h>
#include <wrl.h>

class Sampler
{
public:
	void Create(D3D11_SAMPLER_DESC desc);
	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> data;
};
