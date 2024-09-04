#include "vpch.h"
#include "SpherePhysicsActor.h"
#include "Components/MeshComponent.h"

SpherePhysicsActor::SpherePhysicsActor()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);
}

void SpherePhysicsActor::Create()
{
	__super::Create();

	mesh->SetPhysicsShape(PhysicsActorShape::Sphere);
	mesh->SetMeshFilename("sphere.vmesh");
	mesh->SetPhysicsStatic(false);
}

Properties SpherePhysicsActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
