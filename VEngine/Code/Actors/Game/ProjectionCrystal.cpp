#include "vpch.h"
#include "ProjectionCrystal.h"
#include "Components/MeshComponent.h"

void ProjectionCrystal::OnLinkActivate()
{
	auto testMesh = CreateComponent<MeshComponent>("TestMesh");
	testMesh->SetMeshFilename("cube.vmesh");
	testMesh->Create();
	testMesh->SetLocalPosition(0.f, 0.f, 2.f);
	rootComponent->AddChild(testMesh);
}

void ProjectionCrystal::OnLinkDeactivate()
{
	DeleteComponent("TestMesh");
}
