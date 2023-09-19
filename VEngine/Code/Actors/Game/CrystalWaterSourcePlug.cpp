#include "vpch.h"
#include "CrystalWaterSourcePlug.h"
#include "Components/BoxTriggerComponent.h"
#include "CrystalWaterSource.h"

CrystalWaterSourcePlug::CrystalWaterSourcePlug()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void CrystalWaterSourcePlug::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	ActivateCrystalWaterSourceInTrigger();
}

Properties CrystalWaterSourcePlug::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void CrystalWaterSourcePlug::ActivateCrystalWaterSourceInTrigger()
{
	for (auto& crystalWaterSource : CrystalWaterSource::system.GetActors())
	{
		if (boxTrigger->Contains(crystalWaterSource->GetPositionV()))
		{
			crystalWaterSource->Activate();
		}
		else
		{
			crystalWaterSource->Deactivate();
		}
	}
}
