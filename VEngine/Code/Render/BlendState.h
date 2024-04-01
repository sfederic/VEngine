#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>

class BlendState
{
public:
	BlendState(std::string_view name_, D3D11_BLEND_DESC desc);
	virtual ID3D11BlendState* GetData() { return data.Get(); }

protected:
	std::string name;
	Microsoft::WRL::ComPtr<ID3D11BlendState> data;
};

//@Todo: Using a null blend state to get around transparency issues with point and spot lights.
//Not sure on the 'bug', most likely has to do with blend state settings. Keep a todo here to delete this.
class NullBlendState : public BlendState
{
public:
	NullBlendState(std::string_view name_);
	ID3D11BlendState* GetData() override { return nullptr; }
};
