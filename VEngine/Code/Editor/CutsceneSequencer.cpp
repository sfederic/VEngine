#include "CutsceneSequencer.h"
#include "imgui/imgui.h"
#include "Gameplay/ConditionSystem.h"

CutsceneSequencer::CutsceneSequencer()
{
	items.push_back(CutsceneSequenceItem());
}

void CutsceneSequencer::Tick()
{
	//Sequencer frame data
	ImGui::PushItemWidth(130);
	ImGui::InputInt("Frame ", &currentFrame);
	ImGui::SameLine();
	ImGui::InputInt("Frame Min", &frameMin);
	ImGui::SameLine();
	ImGui::InputInt("Frame Max", &frameMax);
	ImGui::PopItemWidth();

	//Add new sequencer item
	if (ImGui::Button("New"))
	{
		Add(0);
	}

	//Main Sequencer function
	ImSequencer::Sequencer(this, &currentFrame, &expanded, &selectedEntry, &firstFrame,
		ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);

	//@Todo: There's a way to get selectedEntry on ImSequencer::Sequencer() click (it returns a bool) but can't figure it out.
	//Right now currentItemIndex is set in DoubleClick().
	CutsceneSequenceItem& item = items[currentItemIndex];

	//Item Frame data
	ImGui::Text("Name: %s", item.name.c_str());
	ImGui::Text("FrameStart: %d", item.frameStart);
	ImGui::Text("Frame End: %d", item.frameEnd);

	//Combo box to set ConditionFunction to Sequencer item
	//Ref:https://github.com/ocornut/imgui/issues/1658
	if (ImGui::BeginCombo("Condition Functions", item.condition.c_str()))
	{
		auto& conditionFunctions = conditionSystem.conditions;
		for (auto& conditionFuncPair : conditionFunctions)
		{
			bool selected = false;
			if (ImGui::Selectable(conditionFuncPair.first.c_str(), selected))
			{
				item.condition = conditionFuncPair.first;
			}
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void CutsceneSequencer::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
	CutsceneSequenceItem& item = items[index];
	if (color) *color = 0xFFAA8080;
	if (start) *start = &item.frameStart;
	if (end) *end = &item.frameEnd;
	if (type) *type = item.type;
}

void CutsceneSequencer::Add(int type)
{
	//testing
	CutsceneSequenceItem item;
	item.expanded = false;
	item.frameStart = 0;
	item.frameEnd = 25;
	item.type = 0;
	item.name = "test" + std::to_string(type);
	items.push_back(item);
}

void CutsceneSequencer::DoubleClick(int index)
{
	currentItemIndex = index;

	if (items[index].expanded)
	{
		items[index].expanded = false;
		return;
	}

	for (auto& item : items)
	{
		item.expanded = false;
	}

	items[index].expanded = !items[index].expanded;
}
