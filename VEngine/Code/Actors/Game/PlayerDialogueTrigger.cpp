
#include "PlayerDialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
import Actors.Game.Player;

PlayerDialogueTrigger::PlayerDialogueTrigger()
{
	trigger = CreateComponent<BoxTriggerComponent>("Trigger");
	trigger->renderWireframeColour = XMFLOAT4(0.5f, 0.5f, 1.f, 1.f);
	SetRootComponent(trigger);
}

void PlayerDialogueTrigger::Start()
{
	__super::Start();

	if (alreadyActivated)
	{
		SetActive(false);
	}
	else
	{
		trigger->SetTargetAsPlayer();
	}
}

void PlayerDialogueTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (trigger->ContainsTarget() && !alreadyActivated)
	{
		alreadyActivated = true;
		CreateQuickThoughtWidget();
		SetActive(false);
	}
}

Properties PlayerDialogueTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("PlayerThoughtText", &playerThoughtText);
	return props;
}

void PlayerDialogueTrigger::CreateQuickThoughtWidget()
{
	Player::system.GetOnlyActor()->SetQuickThought(playerThoughtText);
}
