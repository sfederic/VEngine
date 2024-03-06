#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "Render/MeshData.h"

class IndexBuffer
{
public:
	void CreateDefault(std::vector<MeshData::indexDataType>& indices);
	void CreateDynamic(std::vector<MeshData::indexDataType>& indices);
	void Destroy();

	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;
};
