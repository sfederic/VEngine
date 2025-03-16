
#include "SpherePhysicsActor.h"
import Components.MeshComponent;

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
