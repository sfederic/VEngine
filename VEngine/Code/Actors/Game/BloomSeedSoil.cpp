#include "vpch.h"
#include "BloomSeedSoil.h"
#include "Components/BoxTriggerComponent.h"
#include "BloomSeed.h"

BloomSeedSoil::BloomSeedSoil()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void BloomSeedSoil::Create()
{
	__super::Create();

	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	SetMeshFilename("mound.vmesh");

	boxTrigger->AddLocalPosition(XMVectorSet(0.f, 1.f, 0.f, 1.f));
}

void BloomSeedSoil::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	for (auto& bloomSeed : BloomSeed::system.GetActors())
	{
		if (boxTrigger->Contains(bloomSeed->GetPositionV()))
		{
			bloomSeed->Plant();
		}
	}
}
