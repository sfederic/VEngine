#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "Render/Vertex.h"

struct MeshDataProxy;

class VertexBuffer
{
public:
	void CreateDefault(MeshDataProxy& meshDataProxy);
	void CreateDynamic(std::vector<Vertex>& vertices);
	void Destroy();

	auto GetDataAddress() { return data.GetAddressOf(); }
	auto GetData() { return data.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;
};
