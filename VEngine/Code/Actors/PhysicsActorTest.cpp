#include "vpch.h"
#include "PhysicsActorTest.h"
#include "Components/MeshComponent.h"

PhysicsActorTest::PhysicsActorTest()
{
	mesh = CreateComponent("Mesh", MeshComponent("sphere_high.vmesh", "test.png"));
	mesh->SetCollisionMeshFilename("cone.vmesh");
	rootComponent = mesh;
}

Properties PhysicsActorTest::GetProps()
{
	return __super::GetProps();
}
