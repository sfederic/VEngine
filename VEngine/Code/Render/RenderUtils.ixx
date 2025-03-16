#define NOMINMAX

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

export module Render.RenderUtils;

import Render.MeshData;
import Render.MeshDataProxy;
import Core.Camera;
import Components.SpatialComponent;
import Components.MeshComponent;
import <cstdint>;
import <string>;
import <vector>;
import <limits>;
import <algorithm>;

using namespace DirectX;

export namespace RenderUtils
{
	void CreateDefaultBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateDynamicBuffer(uint64_t byteWidth, uint32_t bindFlags, const void* initData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateVertexBuffer(MeshDataProxy& meshData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateIndexBuffer(std::vector<MeshData::indexDataType>& indices, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	void CreateSRVForMeshInstance(ID3D11Buffer* structuredBuffer, uint32_t numBufferElements, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outputSrv);
	void CreateStructuredBuffer(uint32_t byteWidth, uint32_t byteStride, const void* initData, Microsoft::WRL::ComPtr<ID3D11Buffer>& outputBuffer);
	UINT CalcBufferByteSize(UINT byteSize);
	void CreateBlendState(D3D11_BLEND_DESC blendDesc, Microsoft::WRL::ComPtr<ID3D11BlendState>& blendState);
	void CreateRastState(D3D11_RASTERIZER_DESC rastDesc, Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rastState);
	void CreateSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerState);
	void SetResourceName(ID3D11DeviceChild* resource, std::string name);

	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetDeviceContext();

	//This function is done as a template because there are several classes inheriting from MeshComponent.
	template <typename T>
	std::vector<T*> SortMeshesByDistanceToCamera()
	{
		struct MeshPack
		{
			T* mesh;
			float distance = 0.f;
		};
		std::vector<MeshPack> meshPacks;

		const XMVECTOR cameraPos = Camera::GetActiveCamera().GetWorldPositionV();

		for (const auto& mesh : T::system.GetComponents())
		{
			float distance = DirectX::XMVector3Length(cameraPos - mesh->GetWorldPositionV()).m128_f32[0];
			if (mesh->alwaysSortLast)
			{
				distance = std::numeric_limits<float>::max();
			}
			MeshPack pack = { mesh.get(), distance };
			meshPacks.emplace_back(pack);
		}

		const auto DistCompare = [](const MeshPack& leftPack, const MeshPack& rightPack)
		{
			return leftPack.distance > rightPack.distance;
		};
		std::sort(meshPacks.begin(), meshPacks.end(), DistCompare);

		std::vector<T*> sortedMeshes;
		for (auto& pack : meshPacks)
		{
			sortedMeshes.emplace_back(pack.mesh);
		}

		return sortedMeshes;
	}
};
