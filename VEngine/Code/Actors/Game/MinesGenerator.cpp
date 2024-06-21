#include "vpch.h"
#include "MinesGenerator.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "PowerCrystal.h"
#include "Gameplay/GameInstance.h"

MinesGenerator::MinesGenerator()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);

	crystalTrigger = CreateComponent<BoxTriggerComponent>("CrystalTrigger");
	AddChildToRoot(crystalTrigger);
}

void MinesGenerator::Create()
{
	__super::Create();

	mesh->SetMeshFilename("cube.vmesh");

	crystalTrigger->SetLocalPosition(1.f, 0.f, 0.f);
}

void MinesGenerator::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (isGeneratorOn)
	{
		return;
	}

	for (auto& crystal : PowerCrystal::system.GetActors())
	{
		if (crystalTrigger->Contains(crystal->GetPositionV()))
		{
			PowerOnGenerator();
			return;
		}
	}
}

Properties MinesGenerator::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Generator On", &isGeneratorOn);
	return props;
}

void MinesGenerator::PowerOnGenerator()
{
	isGeneratorOn = true;
}
