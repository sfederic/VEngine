#pragma once
#include <string>
#include <vector>
#include "ImGuizmo/ImSequencer.h"

struct CutsceneSequenceItem
{
	std::string name;
	int type;
	int frameStart, frameEnd;
	bool expanded;
};

//Editor using ImGuizmo for timelining cutscene events
struct CutsceneSequencer : ImSequencer::SequenceInterface
{
	bool focused = false;
	int frameMin = 0;
	int frameMax = 250;

	int selectedEntry = -1;
	int firstFrame = 0;
	bool expanded = true;
	int currentFrame = 0;

	std::vector<CutsceneSequenceItem> items;

	CutsceneSequencer();

	void Tick();

	virtual int GetFrameMin() const { return frameMin; }
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
	virtual void Duplicate(int index) { items.push_back(items[index]); }

	virtual void Copy() {}
	virtual void Paste() {}

	virtual size_t GetCustomHeight(int index) { return 0; }
	virtual void DoubleClick(int index);

	virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect) {}
	virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect) {}
};
