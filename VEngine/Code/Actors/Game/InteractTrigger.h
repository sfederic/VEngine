#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"
#include <string>

struct BoxTriggerComponent;
struct MemoryComponent;
struct Widget;
struct InteractWidget;

//Used to open up a quick prompt/action when the player overlaps
struct InteractTrigger : Actor
{
	ACTOR_SYSTEM(InteractTrigger)

	BoxTriggerComponent* trigger = nullptr;

	MemoryComponent* memoryComponent = nullptr;

	//Widget shown on interact
	InteractWidget* interactWidget = nullptr;

	//text to show on PrimaryAction() player input
	std::wstring interactText;

	//text to show when memory condition has passed as true
	std::wstring interactKnown;

	//Text to show when player is inside the trigger
	std::wstring overlapText = L"Examine";

	std::string targetActorName;

	//Sound to play on interact
	std::string soundEffect;

	bool isBeingInteractedWith = false;

	InteractTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
