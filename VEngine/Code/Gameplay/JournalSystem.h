#pragma once

#include <map>
#include "JournalEntry.h"

class JournalSystem
{
public:
	JournalSystem& Get()
	{
		static JournalSystem instance;
		return instance;
	}

	void Init();

private:
	std::map<std::wstring, JournalEntry> journalEntries;
};
