
#include "ProjectionCrystal.h"
import Components.MeshComponent;
#include "Components/Lights/SpotLightComponent.h"
#include "Grid.h"
#include "Render/BlendStates.h"

ProjectionCrystal::ProjectionCrystal()
{
	projectionMesh = CreateComponent<MeshComponent>("TestMesh");
	AddChildToRoot(projectionMesh);

	projectionSpotLight = CreateComponent<SpotLightComponent>("SpotLight");
	AddChildToRoot(projectionSpotLight);
}

void ProjectionCrystal::Create()
{
	__super::Create();

	_mesh->SetMeshFilename("crystal.vmesh");

	projectionMesh->canBeLinkedTo = false;
	projectionMesh->SetAmbientColour(XMFLOAT3(0.f, 0.f, 1.f));
	projectionMesh->SetAlpha(0.4f);
	projectionMesh->SetBlendState(BlendStates::Default);
	projectionMesh->SetMeshFilename("cube.vmesh");
	projectionMesh->SetLocalPosition(0.f, 0.f, 2.f);
	projectionMesh->Create();

	projectionSpotLight->SetColour(XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	projectionSpotLight->SetIntensity(50.f);
	projectionSpotLight->SetWorldRotation(GetRotationV());
}

void ProjectionCrystal::Interact()
{
	__super::Interact();

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
