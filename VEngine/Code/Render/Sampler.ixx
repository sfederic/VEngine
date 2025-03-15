export module Render.Sampler;

#include <d3d11.h>
#include <wrl.h>

import Render.RenderUtils;

export class Sampler
{
public:
	void Create() const
	{
		RenderUtils::CreateSamplerState(data);
	}

	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> data;
};
