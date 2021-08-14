#pragma once
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

	Actor()
	{
		mesh->filename = "Meshes/cube.fbx";
	}

	void PushTransformToMesh()
	{
		mesh->transform = transform;
	}
};
