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

	ID3D11Buffer* CreateDefaultBuffer(uint32_t byteWidth, uint32_t bindFlags, const void* initData);
	ID3D11Buffer* CreateDynamicBuffer(uint32_t byteWidth, uint32_t bindFlags, const void* initData);
	ID3D11Buffer* CreateVertexBuffer(MeshDataProxy* meshData);
	ID3D11Buffer* CreateIndexBuffer(MeshDataProxy* meshData);
	ID3D11ShaderResourceView* CreateSRVForMeshInstance(ID3D11Buffer* structuredBuffer, uint32_t numBufferElements);
	ID3D11Buffer* CreateStructuredBuffer(uint32_t byteWidth, uint32_t byteStride, const void* initData);
	Texture2D* CreateTexture(std::string textureFilename);
	Sampler* CreateSampler();
	Sampler* GetDefaultSampler();
};
