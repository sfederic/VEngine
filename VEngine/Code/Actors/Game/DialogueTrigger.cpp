#include "DialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/DialogueComponent.h"
#include "Gameplay/GameUtils.h"

DialogueTrigger::DialogueTrigger()
{
	trigger = BoxTriggerComponent::system.Add(this);
	dialogueComponent = DialogueComponent::system.Add(this);
}

void DialogueTrigger::Start()
{
	trigger->target = (Actor*)GameUtils::GetPlayer();
}

void DialogueTrigger::Tick(float deltaTime)
{
	/*if (trigger->ContainsTarget() && walkInToActivate)
	{
		dialogueComponent->ShowTextAtActor();
		//Disable triggercomponent
	}*/
}

Properties DialogueTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.AddProp(dialogueFile);
	props.AddProp(walkInToActivate);
	return props;
}
