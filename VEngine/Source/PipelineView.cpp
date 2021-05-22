#include "PipelineView.h"
#include "Buffer.h"
#include "Sampler.h"
#include "RasterizerState.h"
#include "Texture.h"
#include "ShaderResourceView.h"

PipelineView::PipelineView()
{
	vertexBuffer = new Buffer();
	indexBuffer = new Buffer();
	samplerState = new Sampler();
	rastState = new RasterizerState();
	texture = new Texture();
	srv = new ShaderResourceView();
}
