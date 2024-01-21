#include "vpch.h"
#include "JournalSystem.h"
#include "JournalEntry.h"

void JournalSystem::Init()
{
	journalEntries.emplace(L"Test", JournalEntry{ .title = L"Test", .text = L"Test text", .image = "test.png" });
}
