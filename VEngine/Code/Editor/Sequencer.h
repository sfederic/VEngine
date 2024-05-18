#pragma once

#include "ImGuizmo/ImSequencer.h"
#include <vector>
#include <cstdio>

//Ref:https://github.com/CedricGuillemet/ImGuizmo/blob/master/example/main.cpp

enum class SequenceEntryTypes : int
{
	Camera,
	Audio,
};

class Sequencer : public ImSequencer::SequenceInterface
{
private:
	inline const static char* SequencerItemTypeNames[] = { "Camera", "Audio" };

public:
	Sequencer() : _frameMin(0), _frameMax(100) {}

	void Render();

	int GetFrameMin() const override { return _frameMin; }
	int GetFrameMax() const override { return _frameMax; }
	int GetItemCount() const override { return (int)sequencerItems.size(); }

	int GetItemTypeCount() const override { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
	const char* GetItemTypeName(int typeIndex) const override { return SequencerItemTypeNames[typeIndex]; }

	const char* GetItemLabel(int index) const override
	{
		static char tmps[512];
		snprintf(tmps, 512, "[%02d] %s", index, SequencerItemTypeNames[sequencerItems[index].mType]);
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

	void Add(int type) override { sequencerItems.push_back(SequenceItem{ static_cast<SequenceEntryTypes>(type), 0, 10, false }); };
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

private:
	struct SequenceItem
	{
		SequenceEntryTypes mType;
		int mFrameStart, mFrameEnd;
		bool mExpanded;
	};

	std::vector<SequenceItem> sequencerItems;

	int _frameMin;
	int _frameMax;
};
