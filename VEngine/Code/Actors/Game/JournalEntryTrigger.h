#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/JournalEntry.h"

class BoxTriggerComponent;

class JournalEntryTrigger : public Actor
{
public:
	ACTOR_SYSTEM(JournalEntryTrigger);

	JournalEntryTrigger();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	JournalEntry journalEntry;

	bool journalEntryAlreadyExists = false;
};