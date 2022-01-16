#include "MemoryWeapon.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MemoryComponent.h"
#include "UI/InteractWidget.h"
#include "Input.h"

MemoryWeapon::MemoryWeapon()
{
	memoryTrigger = BoxTriggerComponent::system.Add(this);
	rootComponent->AddChild(memoryTrigger);
}

void MemoryWeapon::Start()
{
	__super::Start();

	memoryTrigger->SetTargetAsPlayer();

	interactWidget = CreateWidget<InteractWidget>();
	interactWidget->interactText = L"Gain weapon from memory";
}

void MemoryWeapon::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (memoryTrigger->ContainsTarget()) 
	{
		interactWidget->AddToViewport();

		if (Input::GetKeyUp(Keys::Down) && !isBeingInteractedWith) 
		{
			isBeingInteractedWith = true;
			memoryComponent->CreateMemory(this->name);

			interactWidget->RemoveFromViewport();
		}
		else if (isBeingInteractedWith)
		{
			isBeingInteractedWith = false;
		}
	} 
	else 
	{
		interactWidget->RemoveFromViewport();
	}
}

Properties MemoryWeapon::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MemoryWeapon";
	props.Add("WeaponEnergy", &weaponData.energyCost);
	props.Add("WeaponAttack", &weaponData.attackPoints);
	props.Add("WeaponWeight", &weaponData.weight);
	return props;
}
