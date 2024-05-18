#include "vpch.h"
#include "DestructibleActor.h"
#include "Components/DestructibleMeshComponent.h"

DestructibleActor::DestructibleActor()
{
	destructibleMesh = DestructibleMeshComponent::system.Add("Destructible", this,
		DestructibleMeshComponent("destructible_cube.fbx", "test.png"));
	rootComponent = destructibleMesh;
}

Properties DestructibleActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
