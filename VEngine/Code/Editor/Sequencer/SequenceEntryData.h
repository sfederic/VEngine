#pragma once

#include "Core/Properties.h"
#include "Core/UID.h"

struct SequenceEntryData
{
	const UID uid = GenerateUID();

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
	virtual Properties GetProps()
	{
		Properties props;
		//props.Add("UID", &uid);
		return props;
	}
};
