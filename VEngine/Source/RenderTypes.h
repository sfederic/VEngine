#pragma once

#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

struct Line
{
	Vertex p1;
	Vertex p2;
};

struct Matrices
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX mvp;
};

struct InstanceData
{
	XMMATRIX model;
};

struct ModelData
{
	unsigned int GetByteWidth()
	{
		return (sizeof(Vertex) * verts.size());
	}

	void DeleteAll()
	{
		verts.clear();
		indices.clear();
	}

	bool CheckForDuplicateVertices(Vertex& vert);

	std::vector<Vertex> verts;
	std::vector<uint16_t> indices;
};

struct BoneWeights
{
	std::vector<float> weights;
	std::vector<uint32_t> boneIndex;
};
