#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

class JournalEntryTrigger : public Actor
{
public:
	ACTOR_SYSTEM(JournalEntryTrigger);

	JournalEntryTrigger();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	std::wstring journalEntryText;
	std::wstring journalEntryTitle;
	std::string journalEntryImage;

	//@Todo: placeholder. will need to check against a gameplay std::map holding which triggers have been done.
	bool alreadyInteractWith = false;
};
