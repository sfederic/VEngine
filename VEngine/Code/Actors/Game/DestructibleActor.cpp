#include "DestructibleActor.h"
#include "Components/MesHComponent.h"

DestructibleActor::DestructibleActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;
}

Properties DestructibleActor::GetProps()
{
	auto props = Actor::GetProps();
	props.title = "DestructibleActor";
	return props;
}
