#include "vpch.h"
#include "JournalEntryTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/JournalSystem.h"

JournalEntryTrigger::JournalEntryTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void JournalEntryTrigger::Start()
{
	__super::Start();

	if (JournalSystem::Get().DoesJournalEntryExist(journalEntry.title))
	{
		alreadyInteractWith = true;
	}
}

void JournalEntryTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (Input::GetKeyUp("Interact") && !alreadyInteractWith)
	{
		Log("Added to Journal");
		alreadyInteractWith = true;
		JournalSystem::Get().AddJournalEntry(journalEntry);
	}
}

Properties JournalEntryTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Journal Entry Title", &journalEntry.title);
	props.Add("Journal Entry Text", &journalEntry.text);
	props.Add("Journal Entry Image", &journalEntry.image);
	return props;
}
