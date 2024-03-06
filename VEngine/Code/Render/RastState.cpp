#include "vpch.h"
#include "RastState.h"
#include "Render/RenderUtils.h"

RastState::RastState(std::string_view name_, D3D11_RASTERIZER_DESC desc)
	: name(name_)
{
	RenderUtils::CreateRastState(desc, data);
}
