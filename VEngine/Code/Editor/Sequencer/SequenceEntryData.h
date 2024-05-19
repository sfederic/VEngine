#pragma once

#include "Core/Properties.h"
#include "Core/UID.h"

class SequenceEntryData
{
private:
	UID uid = GenerateUID();

public:
	SequenceEntryData(int type) : sequenceEntryType(type) {}

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
	virtual Properties GetProps()
	{
		Properties props;
		props.Add("UID", &uid);
		return props;
	}
};
