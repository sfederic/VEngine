#include <d3d11.h>
#include <wrl.h>

export module Render.RastState;

import Render.RenderUtils;
import <string>;

export class RastState
{
public:
	RastState(std::string_view name_, D3D11_RASTERIZER_DESC desc)
		: name(name_)
	{
		RenderUtils::CreateRastState(desc, data);
	}

	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }
	auto GetName() { return name; }

private:
	std::string name;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> data;
};
