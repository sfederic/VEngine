#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

struct MaterialShaderData;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

//The actual data for each loaded mesh. Each loaded mesh file will have one of these per its filename.
struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

//A pointer structure to a MeshData struct in memory. Each rendered component will have one of these pointing
//to the mesh data on a per-filename basis.
struct MeshDataProxy
{
	std::vector<Vertex>* vertices = nullptr;
	std::vector<uint32_t>* indices = nullptr;

	uint64_t GetVerticesByteWidth()
	{
		return (sizeof(Vertex) * vertices->size());
	}

	uint64_t GetIndicesByteWidth()
	{
		return (sizeof(uint32_t) * indices->size());
	}

	void Clear()
	{
		vertices->clear();
		indices->clear();
	}

	bool CheckDuplicateVertices(Vertex& vert)
	{
		auto pos = XMLoadFloat3(&vert.pos);

		const int size = vertices->size();
		for (int i = 0; i < size; i++)
		{
			XMVECTOR p = XMLoadFloat3(&vertices->at(i).pos);
			if (XMVector3Equal(p, pos))
			{
				return true;
			}
		}

		return false;
	}

	//Duplicate checks for indices only return true if the index is present in the array
	//more than once. Eg. For {2, 1, 0}, {3, 1, 2}, 2 and 1 are the duplicates.
	bool CheckDuplicateIndices(uint32_t index)
	{
		int duplicateCounter = 0;

		for (int i = 0; i < indices->size(); i++)
		{
			if (index == indices->at(i))
			{
				duplicateCounter++;
				if (duplicateCounter >= 2)
				{
					return true;
				}
			}
		}

		return false;
	}
};

struct ShaderMatrices
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX mvp;
	XMMATRIX texMatrix;

	void Create(float aspectRatio);
	void MakeTextureMatrix(MaterialShaderData* shaderData);
	void MakeModelViewProjectionMatrix();
};

struct InstanceData
{
	XMMATRIX world;
};
