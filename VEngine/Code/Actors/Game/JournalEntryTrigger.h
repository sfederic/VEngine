#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/JournalEntry.h"

class BoxTriggerComponent;
class SpriteSheet;

class JournalEntryTrigger : public Actor
{
public:
	ACTOR_SYSTEM(JournalEntryTrigger);

	JournalEntryTrigger();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	JournalEntry journalEntry;
	std::string journalEntryFilename;

	SpriteSheet* journalIconSprite = nullptr;

	bool journalEntryAlreadyExists = false;
};
