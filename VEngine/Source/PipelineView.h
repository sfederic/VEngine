#pragma once

class Buffer;
class Sampler;
class RasterizerState;
class Texture;
class ShaderResourceView;
class BlendState;

//PipelineView is essentially trying to mimic D3D12's graphics pipeline object that attaches
//to each actor system. This sort of structure should make it easier to port graphics shit later on.
struct PipelineView
{
	PipelineView();

	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	Buffer* instanceBuffer;
	Sampler* samplerState;
	RasterizerState* rastState;
	Texture* texture;
	ShaderResourceView* srv;
	BlendState* blendState;
	Buffer* instancedDataStructuredBuffer;
	ShaderResourceView* instancedDataSrv;
};
