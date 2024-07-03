#pragma once

#include "SequenceEntryData.h"
#include "SequenceEntryTypes.h"

class CameraSequenceEntryData : public SequenceEntryData
{
public:
	CameraSequenceEntryData() : SequenceEntryData((int)SequenceEntryTypes::Camera) {}
	void Activate() override {}
	void Deactivate() override {}
	Properties GetProps() override
	{
		auto props = __super::GetProps();
		return props;
	}
};
