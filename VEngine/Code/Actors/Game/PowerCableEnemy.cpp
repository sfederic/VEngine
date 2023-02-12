#include "vpch.h"
#include "PowerCableEnemy.h"
#include "Components/MeshComponent.h"
#include "Actors/MeshSplitActor.h"

PowerCableEnemy::PowerCableEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent->AddChild(mesh);
}

void PowerCableEnemy::Start()
{
}

void PowerCableEnemy::Tick(float deltaTime)
{
}

Properties PowerCableEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
