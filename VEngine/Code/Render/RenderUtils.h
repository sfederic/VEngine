#pragma once

#include <string>

struct MeshDataProxy;
class Texture2D;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct Sampler;

namespace RenderUtils
{
	extern ID3D11Device* device;
	extern ID3D11DeviceContext* context;
	extern Sampler* defaultSampler;

	ID3D11Buffer* CreateDefaultBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData);
	ID3D11Buffer* CreateDynamicBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData);
	ID3D11Buffer* CreateVertexBuffer(MeshDataProxy& meshData);
	ID3D11ShaderResourceView* CreateSRVForMeshInstance(ID3D11Buffer* structuredBuffer, uint32_t numBufferElements);
	ID3D11Buffer* CreateStructuredBuffer(uint32_t byteWidth, uint32_t byteStride, const void* initData);
	void CreateTexture(Texture2D& texture);
	Sampler* CreateSampler();
	Sampler* GetDefaultSampler();
};
