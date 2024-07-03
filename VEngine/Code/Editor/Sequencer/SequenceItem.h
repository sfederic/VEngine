#pragma once

#include "SequenceEntryData.h"
#include "SequenceEntryTypes.h"
#include <memory>

struct SequenceItem
{
	SequenceItem(int type) : mType((SequenceEntryTypes)type) {}
	virtual ~SequenceItem() = default;

	std::unique_ptr<SequenceEntryData> entryData;
	SequenceEntryTypes mType = SequenceEntryTypes::Audio;
	int mFrameStart = 0;
	int mFrameEnd = 60;
	bool mExpanded = false;
	bool mIsActive = false;
};
