#pragma once

#include "Editor/ImGuizmo/ImSequencer.h"
#include "Editor/Sequencer/SequenceItem.h"
#include "Editor/Sequencer/SequenceEntryData.h"
#include "Editor/Sequencer/SequenceEntryTypes.h"
import <vector>;
#include <cstdio>

//Todo: I don't like Imguizmo's sequencer too much, there are no nodes per timeline.
//Look into replacing it https://gitlab.com/GroGy/im-neo-sequencer

//Ref:https://github.com/CedricGuillemet/ImGuizmo/blob/master/example/main.cpp

class Sequencer : public ImSequencer::SequenceInterface
{
private:
	//Make sure the ordering here matches with SequenceEntryTypes. Could use VEnum, but this API is very int based.
	inline const static char* SequencerItemTypeNames[] = { "Audio", "Camera", "ActiveCameraLerp" };

public:
	Sequencer() : _frameMin(0), _frameMax(100) {}

	void Tick();
	void Render();

	int GetFrameMin() const override { return _frameMin; }
	int GetFrameMax() const override { return _frameMax; }
	int GetItemCount() const override { return (int)_sequencerItems.size(); }

	int GetItemTypeCount() const override { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
	const char* GetItemTypeName(int typeIndex) const override { return SequencerItemTypeNames[typeIndex]; }

	const char* GetItemLabel(int index) const override
	{
		static char tmps[512];
		snprintf(tmps, 512, "[%02d] %s", index, SequencerItemTypeNames[(int)_sequencerItems[index].mType]);
		return tmps;
	}

	void Get(int index, int** start, int** end, int* type, unsigned int* color) override
	{
		SequenceItem& item = _sequencerItems[index];
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

	void Add(int type) override;
	void Del(int index) override { _sequencerItems.erase(_sequencerItems.begin() + index); }

	size_t GetCustomHeight(int index) override { return _sequencerItems[index].mExpanded ? 300 : 0; }

	void DoubleClick(int index) override
	{
		if (_sequencerItems[index].mExpanded)
		{
			_sequencerItems[index].mExpanded = false;
			return;
		}

		for (auto& item : _sequencerItems)
		{
			item.mExpanded = false;
		}

		_sequencerItems[index].mExpanded = !_sequencerItems[index].mExpanded;
	}

	void ActivateSequencer(const std::string sequenceFileName);

private:
	auto& GetSequenceEntry(int index) { return _sequencerItems[index]; }

	void WriteCurrentSequenceFileOutFromDialog();
	void ReadInSequencerFileFromDialog();

	void ReadInSequencerFile(const std::string sequenceFileName);

	void Reset();

	std::vector<SequenceItem> _sequencerItems;

	int _frameMin = 0;
	int _frameMax;

	int _currentFrame = 0;
	int _selectedEntry = 0;
	int _firstFrame = 0;

	bool _expanded = true;

	bool _isRunning = false;
};

extern Sequencer gSequencer;
