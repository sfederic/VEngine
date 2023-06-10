#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Refines a mineable grid actor into a crystal by placing it in the trigger of this machine.
class CrystalRefineryMachine : public GridActor
{
public:
	ACTOR_SYSTEM(CrystalRefineryMachine);

	CrystalRefineryMachine();
	void Interact() override;

private:
	BoxTriggerComponent* materialInputTrigger = nullptr;
	BoxTriggerComponent* crystalOutputTrigger = nullptr;
};
