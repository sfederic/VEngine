#include "vpch.h"
#include "Crusher.h"
#include "Components/MeshComponent.h"

Crusher::Crusher()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);
}

void Crusher::Create()
{
	__super::Create();


}

void Crusher::Tick(float deltaTime)
{
	__super::Tick(deltaTime);


}

Properties Crusher::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
