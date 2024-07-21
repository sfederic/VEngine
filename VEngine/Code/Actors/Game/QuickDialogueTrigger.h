#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class DialogueComponent;

//Use this trigger for when you need a dialogue popup that isn't attached to an NPC or the player,
//which instead just spawns at a set location in the world.
class QuickDialogueTrigger : public Actor
{
public:
	ACTOR_SYSTEM(QuickDialogueTrigger);
	QuickDialogueTrigger();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void SetDialogueComponentText();

	std::wstring _quickDialogueText;
	BoxTriggerComponent* _boxTrigger = nullptr;
	DialogueComponent* _dialogueComponent = nullptr;
	DirectX::XMFLOAT3 _dialogueWorldPosition = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	bool _hasBeenActivated = false;
};
