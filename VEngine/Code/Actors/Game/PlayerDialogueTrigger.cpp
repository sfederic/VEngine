#include "vpch.h"
#include "PlayerDialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"

PlayerDialogueTrigger::PlayerDialogueTrigger()
{
	trigger = BoxTriggerComponent::system.Add("Trigger", this);
	trigger->renderWireframeColour = XMFLOAT4(0.5f, 0.5f, 1.f, 1.f);
	rootComponent = trigger;
}

void PlayerDialogueTrigger::Start()
{
	__super::Start();

	trigger->SetTargetAsPlayer();
}

void PlayerDialogueTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (trigger->ContainsTarget() && !alreadyActivated)
	{
		auto player = Player::system.GetFirstActor();
		player->QuickThought(playerThoughtText);

		alreadyActivated = true;

		SetActive(false);
	}
}

Properties PlayerDialogueTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.Add("PlayerThoughtText", &playerThoughtText);
	return props;
}
