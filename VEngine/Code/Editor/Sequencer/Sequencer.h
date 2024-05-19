#pragma once

#include "Editor/ImGuizmo/ImSequencer.h"
#include "Editor/Sequencer/SequenceEntryData.h"
#include "Editor/Sequencer/SequenceEntryTypes.h"
#include <vector>
#include <cstdio>

//Ref:https://github.com/CedricGuillemet/ImGuizmo/blob/master/example/main.cpp

struct SequenceItem
{
	SequenceItem(int type) : mType((SequenceEntryTypes)type) {}

	SequenceEntryData* entryData = nullptr;
	SequenceEntryTypes mType = SequenceEntryTypes::Audio;
	int mFrameStart = 0;
	int mFrameEnd = 60;
	bool mExpanded = false;
	bool mIsActive = false;
};

class Sequencer : public ImSequencer::SequenceInterface
{
private:
	//Make sure the ordering here matches with SequenceEntryTypes. Could use VEnum, but this API is very int based.
	inline const static char* SequencerItemTypeNames[] = { "Audio", "Camera" };

public:
	Sequencer() : _frameMin(0), _frameMax(100) {}

	void Tick();
	void Render();

	int GetFrameMin() const override { return _frameMin; }
	int GetFrameMax() const override { return _frameMax; }
	int GetItemCount() const override { return (int)sequencerItems.size(); }

	int GetItemTypeCount() const override { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
	const char* GetItemTypeName(int typeIndex) const override { return SequencerItemTypeNames[typeIndex]; }

	const char* GetItemLabel(int index) const override
	{
		static char tmps[512];
		snprintf(tmps, 512, "[%02d] %s", index, SequencerItemTypeNames[(int)sequencerItems[index].mType]);
		return tmps;
	}

	void Get(int index, int** start, int** end, int* type, unsigned int* color) override
	{
		SequenceItem& item = sequencerItems[index];
		if (color)
		{
			*color = 0xFFAA8080; // same color for everyone, return color based on type
		}
		if (start)
		{
			*start = &item.mFrameStart;
		}
		if (end)
		{
			*end = &item.mFrameEnd;
		}
		if (type)
		{
			*type = static_cast<int>(item.mType);
		}
	}

	void Add(int type) override { sequencerItems.push_back(SequenceItem(type)); };
	void Del(int index) override { sequencerItems.erase(sequencerItems.begin() + index); }
	void Duplicate(int index) override { sequencerItems.push_back(sequencerItems[index]); }

	size_t GetCustomHeight(int index) override { return sequencerItems[index].mExpanded ? 300 : 0; }

	void DoubleClick(int index) override
	{
		if (sequencerItems[index].mExpanded)
		{
			sequencerItems[index].mExpanded = false;
			return;
		}

		for (auto& item : sequencerItems)
		{
			item.mExpanded = false;
		}

		sequencerItems[index].mExpanded = !sequencerItems[index].mExpanded;
	}

	void ActivateSequencer();

private:
	auto& GetSequenceEntry(int index) { return sequencerItems[index]; }

	void WriteCurrentSequenceFileOut();
	void ReadInSequencerFile();

	std::vector<SequenceItem> sequencerItems;

	int _frameMin = 0;
	int _frameMax;

	int currentFrame = 0;
	int selectedEntry = 0;
	int firstFrame = 0;

	bool expanded = true;

	bool isRunning = false;
};

extern Sequencer gSequencer;
