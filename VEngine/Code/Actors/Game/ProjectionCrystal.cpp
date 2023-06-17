#include "vpch.h"
#include "ProjectionCrystal.h"
#include "Components/MeshComponent.h"
#include "Components/Lights/SpotLightComponent.h"
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
	__super::OnLinkActivate();

	if (!CheckComponentExists("TestMesh"))
	{
		auto testMesh = CreateComponent<MeshComponent>("TestMesh");
		testMesh->SetAmbientColour(XMFLOAT4(0.f, 0.f, 1.f, 0.4f));
		testMesh->SetBlendState(BlendStates::Default);
		testMesh->SetMeshFilename("cube.vmesh");
		testMesh->SetLocalPosition(0.f, 0.f, 2.f);
		testMesh->Create();
		rootComponent->AddChild(testMesh);

		auto spotLight = CreateComponent<SpotLightComponent>("SpotLight");
		spotLight->SetColour(XMFLOAT4(0.f, 0.f, 1.f, 1.f));
		spotLight->SetIntensity(50.f);
		rootComponent->AddChild(spotLight);
		spotLight->SetWorldRotation(GetRotationV());
	}
}

void ProjectionCrystal::OnLinkDeactivate()
{
	__super::OnLinkDeactivate();

	DeleteComponent("TestMesh");
	DeleteComponent("SpotLight");
}
