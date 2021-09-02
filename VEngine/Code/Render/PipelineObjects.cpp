#include "PipelineObjects.h"
#include "Renderer.h"

RastState::RastState(std::string name_, D3D11_RASTERIZER_DESC desc_, ID3D11RasterizerState* data_)
{
	name = name_;
	desc = desc_;
	data = data_;
}
