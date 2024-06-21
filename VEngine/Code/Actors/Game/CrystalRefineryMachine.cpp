#include "vpch.h"
#include "CrystalRefineryMachine.h"
#include "Components/BoxTriggerComponent.h"
#include "Mineable.h"
#include "ProjectionCrystal.h"

CrystalRefineryMachine::CrystalRefineryMachine()
{
	materialInputTrigger = CreateComponent<BoxTriggerComponent>("MaterialTrigger");
	materialInputTrigger->AddLocalPosition(GetForwardVectorV());
	AddChildToRoot(materialInputTrigger);

	crystalOutputTrigger = CreateComponent<BoxTriggerComponent>("CrystalTrigger");
	crystalOutputTrigger->AddLocalPosition(-GetForwardVectorV());
	crystalOutputTrigger->renderWireframeColour = XMFLOAT4(0.1f, 0.1f, 0.9f, 1.f); //Blue
	AddChildToRoot(crystalOutputTrigger);
}

void CrystalRefineryMachine::Interact()
{
	__super::Interact();

	for (auto& mineable : Mineable::system.GetActors())
	{
		if (materialInputTrigger->Contains(mineable->GetPositionV()))
		{
			mineable->Remove();

			Transform transform;
			transform.position = crystalOutputTrigger->GetWorldPosition();
			auto crystal = ProjectionCrystal::system.Add(transform);
			crystal->Create();
			crystal->CreateAllComponents();
			crystal->Start();

			return; //Make sure this only works for one Mineable actor
		}
	}
}
