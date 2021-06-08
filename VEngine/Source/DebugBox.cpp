#include "DebugBox.h"

DebugBox debugBox;

DebugBox::DebugBox()
{
	modelName = "cube.fbx";
	shaderName = "shaders.hlsl";
}

void DebugBox::Tick(float deltaTime)
{
}

void DebugBox::Start()
{
	Init<Actor>(1);

	//Just create a structured buffer that'll hold a good number
	const int sbSize = 1024;
	instancedDataStructuredBuffer = gRenderSystem.CreateStructuredBuffer(sizeof(InstanceData) * sbSize, sizeof(InstanceData), new InstanceData());

	D3D11_SHADER_RESOURCE_VIEW_DESC sbDesc = {};
	sbDesc.Format = DXGI_FORMAT_UNKNOWN;
	sbDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	sbDesc.BufferEx.NumElements = sbSize;
	HR(gRenderSystem.device->CreateShaderResourceView(instancedDataStructuredBuffer, &sbDesc, &instancedDataSrv));
}
