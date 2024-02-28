#include "vpch.h"
#include "PlayerDialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/UISystem.h"
#include "UI/Game/QuickThoughtWidget.h"

PlayerDialogueTrigger::PlayerDialogueTrigger()
{
	trigger = CreateComponent<BoxTriggerComponent>("Trigger");
	trigger->renderWireframeColour = XMFLOAT4(0.5f, 0.5f, 1.f, 1.f);
	rootComponent = trigger;
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
		UISystem::CreateWidget<QuickThoughtWidget>()->AddToViewport();
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
