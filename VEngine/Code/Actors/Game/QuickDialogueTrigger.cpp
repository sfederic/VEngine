
#include "QuickDialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/Game/DialogueWidget.h"
import Core.VMath;

QuickDialogueTrigger::QuickDialogueTrigger()
{
	_boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	SetRootComponent(_boxTrigger);

	_dialogueComponent = CreateComponent<DialogueComponent>("Dialogue");
}

void QuickDialogueTrigger::Start()
{
	__super::Start();

	_boxTrigger->SetTargetAsPlayer();
}

void QuickDialogueTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (!_hasBeenActivated)
	{
		if (_boxTrigger->ContainsTarget())
		{
			_hasBeenActivated = true;
			SetDialogueComponentText();
		}
	}
	else
	{
		if (_dialogueComponent->dialogueWidget->IsInViewport())
		{
			const auto widgetWorldPos = VMath::ConvertToHomogenousPosition(XMLoadFloat3(&_dialogueWorldPosition));
			_dialogueComponent->dialogueWidget->SetWorldPosition(widgetWorldPos);
		}
	}
}

Properties QuickDialogueTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Text", &_quickDialogueText);
	props.Add("Text Spawn Pos", &_dialogueWorldPosition);
	props.Add("Previously Activated", &_hasBeenActivated);
	return props;
}

void QuickDialogueTrigger::SetDialogueComponentText()
{
	_dialogueComponent->dialogueWidget->ResetAllValues();
	_dialogueComponent->dialogueWidget->SetText(_quickDialogueText);

	const auto widgetWorldPos = VMath::ConvertToHomogenousPosition(XMLoadFloat3(&_dialogueWorldPosition));
	_dialogueComponent->dialogueWidget->SetWorldPosition(widgetWorldPos);

	_dialogueComponent->dialogueWidget->AddToViewport(4.f);
}
