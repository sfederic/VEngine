#pragma once

#include <map>
#include "JournalEntry.h"

class JournalSystem
{
public:
	static JournalSystem& Get()
	{
		static JournalSystem instance;
		return instance;
	}

	void AddJournalEntry(JournalEntry& journalEntry);
	bool DoesJournalEntryExist(std::wstring journalEntryTitle);

	//@Todo: these functions work, but they feel funny with how they're wedging into Properties.
	//Maybe think about some sort of JSON asset to add to the engine instead.
	void SaveAllJournalEntriesToFile();
	void LoadJournalEntriesFromFile();

	auto& GetJournalEntries() { return journalEntries; }

private:
	std::map<std::wstring, JournalEntry> journalEntries;
};
