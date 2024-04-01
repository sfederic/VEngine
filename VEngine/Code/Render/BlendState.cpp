#include "vpch.h"
#include "BlendState.h"
#include "Render/RenderUtils.h"

BlendState::BlendState(std::string_view name_, D3D11_BLEND_DESC desc)
	: name(name_)
{
	RenderUtils::CreateBlendState(desc, data);
}

NullBlendState::NullBlendState(std::string_view name_)
	: BlendState(name_, D3D11_BLEND_DESC())
{
	data.Reset();
}
