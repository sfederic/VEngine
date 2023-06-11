#include "vpch.h"
#include "OreSmashMachine.h"
#include "Components/BoxTriggerComponent.h"

OreSmashMachine::OreSmashMachine()
{
	oreToCrystalTrigger = CreateComponent<BoxTriggerComponent>("OreTrigger");
	oreToCrystalTrigger->SetLocalPosition(1.f, 0.f, 0.f);
	rootComponent->AddChild(oreToCrystalTrigger);
}

void OreSmashMachine::Interact()
{

}
