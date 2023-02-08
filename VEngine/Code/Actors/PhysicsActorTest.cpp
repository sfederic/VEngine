#include "vpch.h"
#include "PhysicsActorTest.h"
#include "Components/MeshComponent.h"

PhysicsActorTest::PhysicsActorTest()
{
	mesh = CreateComponent(MeshComponent("sphere_high.vmesh", "test.png"), "Mesh");
	mesh->SetCollisionMeshFilename("ico_sphere.vmesh");
	rootComponent = mesh;
}

Properties PhysicsActorTest::GetProps()
{
	return __super::GetProps();
}
