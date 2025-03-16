#pragma once

#include "SequenceEntryData.h"
import <string>;

class AudioSequenceEntryData : public SequenceEntryData
{
public:
	std::string audioFilename;

	AudioSequenceEntryData() : SequenceEntryData((int)SequenceEntryTypes::Audio) {}

	void Activate() override;

	void Deactivate() override {}

	Properties GetProps() override
	{
		auto props = __super::GetProps();
		props.Add("AudioFilename", &audioFilename);
		return props;
	}
};
