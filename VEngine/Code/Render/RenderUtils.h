#pragma once

#include <cstdint>
#include <wrl.h>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceChild;
struct Sampler;
struct MeshDataProxy;
class Texture2D;

namespace RenderUtils
{
	void CreateDefaultBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateDynamicBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateVertexBuffer(MeshDataProxy& meshData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateSRVForMeshInstance(ID3D11Buffer* structuredBuffer, uint32_t numBufferElements, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outputSrv);
	void CreateStructuredBuffer(uint32_t byteWidth, uint32_t byteStride, const void* initData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateTexture(Texture2D& texture);
	UINT CalcBufferByteSize(UINT byteSize);
	void CreateSampler(Sampler& sampler);
	void SetResourceName(ID3D11DeviceChild* resource, std::string name);
};
