#include "vpch.h"
#include "JournalEntryTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"

JournalEntryTrigger::JournalEntryTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void JournalEntryTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (Input::GetKeyUp("Interact") && !alreadyInteractWith)
	{
		Log("Added to Journal");
		alreadyInteractWith = true;
	}
}

Properties JournalEntryTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Journal Entry Title", &journalEntryTitle);
	props.Add("Journal Entry Text", &journalEntryText);
	props.Add("Journal Entry Image", &journalEntryImage);
	return props;
}
