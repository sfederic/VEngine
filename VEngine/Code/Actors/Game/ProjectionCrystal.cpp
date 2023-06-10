#include "vpch.h"
#include "ProjectionCrystal.h"
#include "Components/MeshComponent.h"
#include "Grid.h"
#include "Render/BlendStates.h"

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
	testMesh->SetAmbientColour(XMFLOAT4(0.f, 0.f, 1.f, 0.4f));
	testMesh->SetBlendState(BlendStates::Default);
	testMesh->SetMeshFilename("cube.vmesh");
	testMesh->SetLocalPosition(0.f, 0.f, 2.f);
	testMesh->Create();
	rootComponent->AddChild(testMesh);
}

void ProjectionCrystal::OnLinkDeactivate()
{
	DeleteComponent("TestMesh");
}
