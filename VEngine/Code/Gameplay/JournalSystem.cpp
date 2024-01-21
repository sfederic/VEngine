#include "vpch.h"
#include "JournalSystem.h"
#include "JournalEntry.h"

void JournalSystem::AddJournalEntry(JournalEntry& journalEntry)
{
	journalEntries.emplace(journalEntry.title, journalEntry);
}

bool JournalSystem::DoesJournalEntryExist(std::wstring journalEntryTitle)
{
	return journalEntries.find(journalEntryTitle) != journalEntries.end();
}
