#pragma once

#include <map>
#include "JournalEntry.h"

//@Todo: this class needs reworking. There's a discrprency between how the assetdock and this journal_entries.sav
//files are working with this class.
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

	JournalEntry LoadSingleJournalEntryFromFile(const std::string filename);

	auto& GetJournalEntries() { return journalEntries; }

private:
	//Keep this as a std::map, make sure the entries are sorted for widget UI display.
	std::map<std::wstring, JournalEntry> journalEntries;
};
