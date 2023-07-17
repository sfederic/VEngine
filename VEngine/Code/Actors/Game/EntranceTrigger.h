#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
struct ConditionComponent;
struct InteractWidget;

class EntranceTrigger : public Actor
{
public:
	ACTOR_SYSTEM(EntranceTrigger)

	EntranceTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void Activate() override { UnlockEntrance(); }
	void Deactivate() override { LockEntrance(); }

	bool CheckIfWorldExists(std::string& worldName);

	void UnlockEntrance();
	void LockEntrance();

	auto GetLevelToMoveTo() { return levelToMoveTo; }
	auto GetEntranceTag() { return entranceTag; }

private:
	BoxTriggerComponent* trigger = nullptr;
	ConditionComponent* conditionComponent = nullptr;

	InteractWidget* interactWidget = nullptr;

	std::string levelToMoveTo;

	//text to show when entrance is open
	std::wstring openText = L"Open";

	//text to show when entrance is locked
	std::wstring lockedText = L"Locked";

	std::string unlockAudio;
	std::string openAudio = "door.wav";

	//Use if there are multiple entrances with matching level filenames to differentiate.
	std::string entranceTag;

	bool isEntranceActive = true;
	bool isEntranceLocked = false;

	//Whether the player has used the entrance to move between worlds.
	bool entranceInteractedWith = false;
};
