#pragma once
#include <d3d11.h>
#include "PipelineObjects.h"

struct MeshDataProxy;

namespace RenderUtils
{
	extern ID3D11Device* device;
	extern ID3D11DeviceContext* context;
	extern Sampler* defaultSampler;

	ID3D11Buffer* CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData);
	ID3D11Buffer* CreateDynamicBuffer(UINT byteWidth, UINT bindFlags, const void* initData);
	ID3D11Buffer* CreateVertexBuffer(MeshDataProxy* meshData);
	ID3D11Buffer* CreateIndexBuffer(MeshDataProxy* meshData);
	ID3D11ShaderResourceView* CreateSRVForMeshInstance(ID3D11Buffer* structuredBuffer, UINT numBufferElements);
	ID3D11Buffer* CreateStructuredBuffer(UINT byteWidth, UINT byteStride, const void* initData);
	Texture2D* CreateTexture(std::string textureFilename);
	Sampler* CreateSampler();
	Sampler* GetDefaultSampler();
};
