#include "vpch.h"
#include "BloomSeedSoil.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "BloomSeed.h"

BloomSeedSoil::BloomSeedSoil()
{
	moundMesh = CreateComponent<MeshComponent>("MoundMesh");
	SetRootComponent(moundMesh);

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	AddChildToRoot(boxTrigger);
}

void BloomSeedSoil::Create()
{
	__super::Create();

	moundMesh->SetMeshFilename("mound.vmesh");

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

Properties BloomSeedSoil::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
