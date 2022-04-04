#include "CutsceneSequencer.h"
#include <fstream>
#include "imgui/imgui.h"
#include <qfiledialog.h>
#include "Gameplay/ConditionSystem.h"
#include "Log.h"

char itemNameInput[64] = {};

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
	if (ImGui::Button("Add"))
	{
		Add(0);
	}
	ImGui::SameLine();
	//Qt-based save/load code
	if (ImGui::Button("Save"))
	{
		QFileDialog saveDialog;
		QString saveName = saveDialog.getSaveFileName(nullptr, "Save Cutscene File", "Cutscenes/");
		if (!saveName.isEmpty())
		{
			std::ofstream os;
			os.open(saveName.toStdString(), std::ios_base::out);

			for (auto& item : items)
			{
				os << item.name << "\n";
				os << item.condition << "\n";
				os << item.frameStart << "\n";
				os << item.frameEnd << "\n";
			}

			os.close();
			Log("[%s] saved.", saveName.toStdString().c_str());
		}
	}	
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		QFileDialog loadDialog;
		QString loadName = loadDialog.getOpenFileName(nullptr, "Open Cutscene File", "Cutscenes/");
		if (!loadName.isEmpty())
		{
			std::ifstream is;
			is.open(loadName.toStdString(), std::ios_base::in);

			items.clear();

			while (!is.eof())
			{
				const int lineSize = 1024;
				char line[lineSize]{};

				std::string name;
				std::string condition;
				std::string frameStart;
				std::string frameEnd;

				is.getline(line, lineSize);
				name.assign(line);
				if (name.empty())
				{
					break;
				}

				is.getline(line, lineSize);
				condition.assign(line);

				is.getline(line, lineSize);
				frameStart.assign(line);

				is.getline(line, lineSize);
				frameEnd.assign(line);

				CutsceneSequenceItem newItem = {};
				newItem.name = name;
				newItem.condition = condition;
				newItem.frameStart = std::stoi(frameStart);
				newItem.frameEnd = std::stoi(frameEnd);

				items.push_back(newItem);
			}

			is.close();
		}
	}

	//@Todo: There's a way to get selectedEntry on ImSequencer::Sequencer() click (it returns a bool) but can't figure it out.
	//Right now currentItemIndex is set in DoubleClick().
	CutsceneSequenceItem& currentItem = items[currentItemIndex];

	//Item Frame data
	ImGui::InputText("Name", itemNameInput, 64);
	currentItem.name = itemNameInput;

	ImGui::Text("FrameStart: %d", currentItem.frameStart);
	ImGui::Text("Frame End: %d", currentItem.frameEnd);

	//Combo box to set ConditionFunction to Sequencer item
	//Ref:https://github.com/ocornut/imgui/issues/1658
	if (ImGui::BeginCombo("Condition Functions", currentItem.condition.c_str()))
	{
		auto& conditionFunctions = conditionSystem.conditions;
		for (auto& conditionFuncPair : conditionFunctions)
		{
			bool selected = false;
			if (ImGui::Selectable(conditionFuncPair.first.c_str(), selected))
			{
				currentItem.condition = conditionFuncPair.first;
			}
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	//Main Sequencer function
	ImSequencer::Sequencer(this, &currentFrame, &expanded, &selectedEntry, &firstFrame,
		ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
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

	strcpy(itemNameInput, items[currentItemIndex].name.c_str());

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
