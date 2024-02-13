#include "vpch.h"
#include "JournalEntryTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/JournalSystem.h"
#include "Asset/AssetPaths.h"
#include "Particle/SpriteSheet.h"
#include "UI/Game/PopupWidget.h"
#include "UI/UISystem.h"

JournalEntryTrigger::JournalEntryTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;

	journalIconSprite = CreateComponent<SpriteSheet>("JournalIcon");
	rootComponent->AddChild(journalIconSprite);
}

void JournalEntryTrigger::Create()
{
	__super::Create();

	journalIconSprite->SetupSpriteData("UI/journal_icon.png", 1, 1, true);
	journalIconSprite->SetLocalScale(0.5f);
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
		journalEntry = JournalSystem::Get().LoadSingleJournalEntryFromFile(
			AssetBaseFolders::journalEntry + "/" + journalEntryFilename);

		JournalSystem::Get().AddJournalEntry(journalEntry);

		Log("Added to Journal");
		journalEntryAlreadyExists = true;

		auto popup = UISystem::CreateWidget<PopupWidget>();
		popup->SetPopupText(journalEntry.title + L" added to journal.");
		popup->OnceOffAddToViewport(2.5f);
	}
}

Properties JournalEntryTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Journal Entry Filename", &journalEntryFilename).autoCompletePath = "/" + AssetBaseFolders::journalEntry;
	return props;
}
