#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Transform.h"
#include "Components/MeshComponent.h"

using namespace DirectX;

struct Actor
{
	Transform transform;

	BoundingOrientedBox boundingBox;
	BoundingSphere boundingSphere;

	MeshComponent* mesh = MeshComponent::system.Add();

	Actor* parent;
	std::vector<Actor*> children;

	Actor();
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
};
