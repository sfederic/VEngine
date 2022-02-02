#pragma once
#include "Unit.h"

struct DialogueWidget;

//Actor that can be talked to and interactied with.
struct NPC : Unit
{
	ACTOR_SYSTEM(NPC)

	bool isQuickDialogueActive = false;

	//Quick dialogue to show when NPC spawns. If empty, nothing shows.
	std::wstring spawnText;
	DialogueWidget* spawnTextWidget = nullptr;

	//Text to display on battle start
	std::wstring battleStartText;

	//Have to use a Tick() timer with NPCs for quicktalk because of null destroyed npcs
	float quickTalkTimer = 0.f;

	NPC();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//A once off dialogue popup used like interact popup is.
	void QuickTalkTo();
	void BattleStartDialogue();
	void EndQuickTalkTo();
};
