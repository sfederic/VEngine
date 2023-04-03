#pragma once

#include <DirectXCollision.h>
#include <vector>
#include "Vertex.h"

class Skeleton;

//A pointer structure to a MeshData struct in memory. Each rendered component will have one of these pointing
//to the mesh data on a per-filename basis.
struct MeshDataProxy
{
	//Still copied from MeshData, but needs to be unique for vertex painting.
	std::vector<Vertex> vertices;

	DirectX::BoundingBox* boundingBox = nullptr;

	Skeleton* skeleton = nullptr;

	auto GetVerticesByteWidth()
	{
		return (sizeof(Vertex) * vertices.size());
	}

	std::vector<Vertex>& GetVertices() { return vertices; }
};
