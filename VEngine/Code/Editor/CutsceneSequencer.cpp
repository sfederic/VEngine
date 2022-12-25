#include "vpch.h"
#include "CutsceneSequencer.h"
#include <fstream>
#include <filesystem>
#include <cassert>
#include "imgui/imgui.h"
#include <qfiledialog.h>
#include "Log.h"

CutsceneSequencer cutsceneSequencer;

char itemNameInput[64] = {};
char conditionArgInput[128] = {};

CutsceneSequencer::CutsceneSequencer()
{
	items.emplace_back(CutsceneSequenceItem());
}

void CutsceneSequencer::PlaybackTick(float deltaTime)
{
	if (playingBack)
	{
		currentFrame++; //Keep in mind whatever FPS here
		playbackTimer += deltaTime;

		if (currentFrame >= frameMax)
		{
			currentFrame = 0;
			playbackTimer = 0.f;

			if (playbackLooping)
			{
				playingBack = false;
			}
		}
	}
}

void CutsceneSequencer::StartPlayback(bool loop)
{
	playingBack = true;
	playbackTimer = 0.f;
	currentFrame = 0;
	playbackLooping = loop;
}

void CutsceneSequencer::UITick(float deltaTime)
{
	//Sequencer frame data
	ImGui::PushItemWidth(130);
	ImGui::InputInt("Current Frame", &currentFrame);
	ImGui::SameLine();
	ImGui::InputInt("Frame Max", &frameMax);
	ImGui::PopItemWidth();

	//Playback buttons
	if (ImGui::Button("Play"))
	{
		playingBack = true;
		currentFrame = 0;
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		playingBack = false;
		currentFrame = 0;
	}

	//Add new sequencer item
	if (ImGui::Button("Add"))
	{
		Add(0);
	}

	ImGui::SameLine();

	//Qt-based save/load code
	if (ImGui::Button("Save"))
	{
		SaveCutsceneFile();
	}	

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		LoadCutsceneFileEditor();
	}

	//@Todo: There's a way to get selectedEntry on ImSequencer::Sequencer() click (it returns a bool) but can't figure it out.
	//Right now currentItemIndex is set in DoubleClick().
	CutsceneSequenceItem& currentItem = items[currentItemIndex];

	//Item data inputs
	ImGui::InputText("Name", itemNameInput, 64);
	currentItem.name = itemNameInput;

	ImGui::Text("FrameStart: %d", currentItem.frameStart);
	ImGui::Text("Frame End: %d", currentItem.frameEnd);
}

void CutsceneSequencer::SaveCutsceneFile()
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
			os << item.conditionArg << "\n";
			os << item.frameStart << "\n";
			os << item.frameEnd << "\n";
		}

		os.close();
		Log("[%s] saved.", saveName.toStdString().c_str());
	}
}

void CutsceneSequencer::LoadCutscene(std::string cutsceneFilename)
{
	std::string path = "Cutscenes/" + cutsceneFilename;
	assert(std::filesystem::exists(path));

	std::ifstream is;
	is.open(path, std::ios_base::in);

	items.clear();

	while (!is.eof())
	{
		const int lineSize = 1024;
		char line[lineSize]{};

		std::string name;
		std::string condition;
		std::string conditionArg;
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
		conditionArg.assign(line);

		is.getline(line, lineSize);
		frameStart.assign(line);

		is.getline(line, lineSize);
		frameEnd.assign(line);

		CutsceneSequenceItem newItem = {};
		newItem.name = name;
		newItem.condition = condition;
		newItem.conditionArg = conditionArg;
		newItem.frameStart = std::stoi(frameStart);
		newItem.frameEnd = std::stoi(frameEnd);

		items.emplace_back(newItem);
	}

	is.close();
}

void CutsceneSequencer::LoadCutsceneFileEditor()
{
	QFileDialog loadDialog;
	QString loadName = loadDialog.getOpenFileName(nullptr, "Open Cutscene File", "Cutscenes/");
	if (!loadName.isEmpty())
	{
		LoadCutscene(loadName.toStdString());

		//Set name widget on load
		if (items.size() > 0)
		{
			strcpy(itemNameInput, items[0].name.c_str());
		}
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
	CutsceneSequenceItem item;
	item.expanded = false;
	item.frameStart = 0;
	item.frameEnd = 25;
	item.type = 0;
	item.name = "New";
	items.emplace_back(item);
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
