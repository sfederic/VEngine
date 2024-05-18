#pragma once

#include <string>

struct SequenceEntryData
{
	virtual void Init() {}
};

struct AudioSequenceEntryData : SequenceEntryData
{
	std::string audioFilename;
};
