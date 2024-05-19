#pragma once

#include "SequenceEntryData.h"
#include <string>

struct AudioSequenceEntryData : SequenceEntryData
{
	std::string audioFilename;

	void Activate() override;

	void Deactivate() override {}

	Properties GetProps() override
	{
		auto props = __super::GetProps();
		//props.Add("AudioFilename", &audioFilename);
		return props;
	}
};
