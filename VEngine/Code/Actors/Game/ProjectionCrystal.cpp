#include "vpch.h"
#include "ProjectionCrystal.h"
#include "Components/MeshComponent.h"
#include "Grid.h"

void ProjectionCrystal::Interact()
{
	isActive = !isActive;
	if (isActive)
	{
		OnLinkActivate();
		Grid::system.GetOnlyActor()->Awake();
	}
	else
	{
		OnLinkDeactivate();
		Grid::system.GetOnlyActor()->Awake();
	}
}

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
