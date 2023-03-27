#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;
struct ConditionComponent;
struct InteractWidget;

struct EntranceTrigger : Actor
{
	ACTOR_SYSTEM(EntranceTrigger)

	BoxTriggerComponent* trigger = nullptr;
	ConditionComponent* conditionComponent = nullptr;

	InteractWidget* interactWidget = nullptr;

	std::string levelToMoveTo;

	//text to show when entrance is open
	std::wstring openText = L"Open";

	//text to show when entrance is locked
	std::wstring lockedText = L"Locked";

	bool isEntranceActive = true;
	bool isEntranceLocked = false;

	//Whether the player has used the entrance to move between worlds.
	bool entranceInteractedWith = false;

	EntranceTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	bool CheckIfWorldExists(std::string& worldName);

	void UnlockEntrance();
	void LockEntrance();

private:
	void SetCameraZoomFocusAndPopupWidget(std::string popupText);
};
