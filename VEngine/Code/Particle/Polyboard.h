#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/MeshData.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Physics/HitResult.h"

//Taken from 'Mathematics for 3D Game Programming and Computer Graphics', Chapter 9.3.3: Polyboards.
//Polyboard is a line of 'planes' facing the camera for line-like effects (beams, lightning, etc.)
class Polyboard : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(Polyboard);

	Polyboard();
	void Create() override;
	void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void CalcVertices();

	void SetStartPoint(const XMVECTOR start);
	void SetEndPoint(const XMVECTOR end);
	bool RaycastFromStartToEndPoints(HitResult& hit) const;
	void SetDestroyTimer(float timerMax);

	auto& GetVertexBuffer() { return vertexBuffer; }
	auto& GetIndexBuffer() { return indexBuffer; }

	auto& GetVertices() { return vertices; }
	auto& GetIndices() { return indices; }

	void SetTextureFilename(const std::string_view filename) { textureData.filename = filename; }
	auto GetTextureFilename() const { return textureData.filename; }

protected:
	void GenerateVertices();

	TextureData textureData;

	std::vector<Vertex> vertices;
	std::vector<MeshData::indexDataType> indices;

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	XMFLOAT3 startPoint;
	XMFLOAT3 endPoint;

	float radius = 0.8f;

	float destroyTimer = 0.f;
	float destroyTimerMax = 0.f;

	bool setToDestroy = false;
};
