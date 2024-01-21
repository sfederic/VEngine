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

private:
	std::map<std::wstring, JournalEntry> journalEntries;
};
