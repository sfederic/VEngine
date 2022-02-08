#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"

struct Buffer;
struct MeshDataProxy;

//Taken from 'Mathematics for 3D Game Programming and Computer Graphics', Chapter 9.3.3: Polyboards.
//Polyboard is a line of 'planes' facing the camera for line-like effects (beams, lightning, etc.)
struct Polyboard : SpatialComponent
{
	COMPONENT_SYSTEM(Polyboard);

	Buffer* vertexBuffer = nullptr;
	Buffer* indexBuffer = nullptr;

	TextureData textureData;

	std::vector<Vertex> vertices;
	std::vector<MeshData::indexDataType> indices;

	XMFLOAT3 startPoint;
	XMFLOAT3 endPoint;

	float radius = 1.0f;
	float size = 2.0f;

	Polyboard();
	virtual Properties GetProps() override;
	void GenerateVertices();
	void CalcVertices();
};
