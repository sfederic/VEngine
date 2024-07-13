#pragma once

#include "Core/Properties.h"
#include "Core/UID.h"
#include "SequenceEntryTypes.h"

class SequenceEntryData
{
private:
	UID uid = GenerateUID();
	int sequenceEntryType = 0;

public:
	SequenceEntryData(int type) : sequenceEntryType(type) {}
	virtual ~SequenceEntryData() = default;

	virtual void Tick(float deltaTime) {}
	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual Properties GetProps()
	{
		Properties props;
		//Make sure the type is at the "front" of the props map as it'll be used to create entries.
		props.Add(" EntryType", &sequenceEntryType);
		props.Add("UID", &uid);
		return props;
	}
};
