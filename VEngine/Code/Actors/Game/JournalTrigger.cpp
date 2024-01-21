#include "vpch.h"
#include "JournalTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"

JournalTrigger::JournalTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void JournalTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (Input::GetKeyUp("Interact") && !alreadyInteractWith)
	{
		Log("Added to Journal");
		alreadyInteractWith = true;
	}
}

Properties JournalTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Journal Entry Title", &journalEntryTitle);
	props.Add("Journal Entry Text", &journalEntryText);
	props.Add("Journal Entry Image", &journalEntryImage);
	return props;
}
