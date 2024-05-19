#pragma once

#include "SequenceEntryData.h"
#include <string>

class AudioSequenceEntryData : public SequenceEntryData
{
public:
	std::string audioFilename;

	void Activate() override;

	void Deactivate() override {}

	Properties GetProps() override
	{
		auto props = __super::GetProps();
		props.Add("AudioFilename", &audioFilename);
		return props;
	}
};
