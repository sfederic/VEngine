#include "vpch.h"
#include "InteractActor.h"
#include "Components/MeshComponent.h"

InteractActor::InteractActor()
{
	mesh = CreateComponent(MeshComponent(), "Mesh");
	rootComponent = mesh;
}

void InteractActor::Start()
{
}

void InteractActor::Tick(float deltaTime)
{
}

Properties InteractActor::GetProps()
{
	return __super::GetProps();
}

void InteractActor::Interact()
{
	Log("interacted");
}
