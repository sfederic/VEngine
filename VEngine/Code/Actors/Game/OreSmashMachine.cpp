#include "vpch.h"
#include "OreSmashMachine.h"
#include "Components/BoxTriggerComponent.h"
#include "Ore.h"
#include "ProjectionCrystal.h"

OreSmashMachine::OreSmashMachine()
{
	oreToCrystalTrigger = CreateComponent<BoxTriggerComponent>("OreTrigger");
	oreToCrystalTrigger->SetLocalPosition(1.f, 0.f, 0.f);
	rootComponent->AddChild(oreToCrystalTrigger);
}

void OreSmashMachine::Interact()
{
	__super::Interact();

	for (auto& ore : Ore::system.GetActors())
	{
		if (oreToCrystalTrigger->Contains(ore->GetPositionV()))
		{
			Transform t;
			t.position = oreToCrystalTrigger->GetWorldPosition();
			auto crystal = ProjectionCrystal::system.Add(t);
			crystal->Create();
			crystal->CreateAllComponents();
			crystal->Start();

			ore->Remove();

			return;
		}
	}
}
