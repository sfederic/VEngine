#include "vpch.h"
#include "JournalSystem.h"
#include "JournalEntry.h"
#include "Core/Serialiser.h"

void JournalSystem::AddJournalEntry(JournalEntry& journalEntry)
{
	journalEntries.emplace(journalEntry.title, journalEntry);
}

bool JournalSystem::DoesJournalEntryExist(std::wstring journalEntryTitle)
{
	return journalEntries.find(journalEntryTitle) != journalEntries.end();
}

void JournalSystem::SaveAllJournalEntriesToFile()
{
	Serialiser s("journal_entries.sav", OpenMode::Out);

	const size_t numOfJournalEntries = journalEntries.size();
	s.WriteLine(numOfJournalEntries);

	for (auto& [title, entry] : journalEntries)
	{
		auto props = entry.GetProps();
		s.Serialise(props);

		s.WriteLine("next");
	}

	s.WriteLine("end");
}

void JournalSystem::LoadJournalEntriesFromFile()
{
	Deserialiser d("journal_entries.sav", OpenMode::In);

	size_t numOfJournalEntries = 0;
	d.ReadLine(numOfJournalEntries);

	std::map<std::wstring, JournalEntry> temp;

	for (size_t i = 0; i < numOfJournalEntries; i++)
	{
		JournalEntry entry;
		auto props = entry.GetProps();
		d.Deserialise(props);

		temp.emplace(entry.title, entry);
	}
}
