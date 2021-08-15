#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Transform.h"

using namespace DirectX;

struct Actor
{
	Transform transform;

	BoundingOrientedBox boundingBox;
	BoundingSphere boundingSphere;

	Actor* parent;
	std::vector<Actor*> children;

	Actor();
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
};
