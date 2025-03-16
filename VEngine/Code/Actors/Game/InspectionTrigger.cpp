
#include "InspectionTrigger.h"
#include "Components/BoxTriggerComponent.h"

InspectionTrigger::InspectionTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	AddChildToRoot(boxTrigger);
}

void InspectionTrigger::Start()
{
	__super::Start();

	boxTrigger->SetTargetAsPlayer();
}

bool InspectionTrigger::ContainsPlayer()
{
	return boxTrigger->ContainsTarget();
}
