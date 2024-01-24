#include "vpch.h"
#include "JournalEntryTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/JournalSystem.h"
#include "Asset/AssetPaths.h"

JournalEntryTrigger::JournalEntryTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void JournalEntryTrigger::Start()
{
	__super::Start();

	boxTrigger->SetTargetAsPlayer();
}

void JournalEntryTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (boxTrigger->ContainsTarget() && Input::GetKeyUp("Interact") && !journalEntryAlreadyExists)
	{
		Log("Added to Journal");
		journalEntryAlreadyExists = true;

		journalEntry = JournalSystem::Get().LoadSingleJournalEntryFromFile(
			AssetBaseFolders::journalEntry + "/" + journalEntryFilename);
	}
}

Properties JournalEntryTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Journal Entry Filename", &journalEntryFilename).autoCompletePath = "/" + AssetBaseFolders::journalEntry;
	return props;
}
