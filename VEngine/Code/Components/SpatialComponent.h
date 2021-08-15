#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Component.h"
#include "Transform.h"

using namespace DirectX;

struct SpatialComponent : Component
{
	Transform transform;

	BoundingOrientedBox boundingBox;

	Component* parent = nullptr;
	std::vector<Component*> children;
};
