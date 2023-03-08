#pragma once

#include <string>
#include <vector>
#include "ImGuizmo/ImSequencer.h"

struct CutsceneSequenceItem
{
	std::string condition;
	std::string conditionArg;
	std::string name;
	int type = 0;
	int frameStart = 0;
	int frameEnd = 0;
	bool expanded = false;
	bool active = false;
};

//Editor using ImGuizmo for timelining cutscene events
class CutsceneSequencer : public ImSequencer::SequenceInterface
{
public:
	CutsceneSequencer();

	//Seperate out the ticks for handling UI and Cutscene playback logic
	void UITick(float deltaTime);
	void PlaybackTick(float deltaTime);

	void LoadCutsceneFileEditor();
	void LoadCutscene(std::string cutsceneFilename);
	void SaveCutsceneFile();

	void StartPlayback(bool loop);

	//ImSequencer::SequenceInterface virtual functions
	virtual int GetFrameMin() const { return 0; }
	virtual int GetFrameMax() const { return frameMax; };
	virtual int GetItemCount() const { return items.size(); };

	virtual void BeginEdit(int index) {}
	virtual void EndEdit() {}
	virtual int GetItemTypeCount() const { return 0; }
	virtual const char* GetItemTypeName(int typeIndex) const { return items[typeIndex].name.c_str(); }
	virtual const char* GetItemLabel(int index) const { return items[index].name.c_str(); }

	virtual void Get(int index, int** start, int** end, int* type, unsigned int* color);
	virtual void Add(int type);

	virtual void Del(int index) { items.erase(items.begin() + index); }
	virtual void Duplicate(int index) { items.emplace_back(items[index]); }

	virtual void Copy() {}
	virtual void Paste() {}

	virtual size_t GetCustomHeight(int index) { return 0; }
	virtual void DoubleClick(int index);

	virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect) {}
	virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect) {}

private:
	bool focused = false;
	int frameMax = 250;

	int selectedEntry = -1;
	int firstFrame = 0;
	bool expanded = true;
	int currentFrame = 0;

	int currentItemIndex = 0;

	float playbackTimer = 0.f;
	bool playingBack = false;
	bool playbackLooping = false;

	std::vector<CutsceneSequenceItem> items;
};

extern CutsceneSequencer cutsceneSequencer;
