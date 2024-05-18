#include "vpch.h"
#include "Sequencer.h"
#include "ImGui/imgui.h"

void Sequencer::Render()
{
	static int currentFrame = 0;
	static bool expanded = true;
	static int selectedEntry = 0;
	static int firstFrame = 0;

	ImGui::PushItemWidth(130);
	ImGui::InputInt("Frame ", &currentFrame);
	ImGui::SameLine();
	ImGui::InputInt("Frame Min", &_frameMin);
	ImGui::SameLine();
	ImGui::InputInt("Frame Max", &_frameMax);
	ImGui::PopItemWidth();
	if (ImGui::Button("Add Item"))
	{
		Add(0);
	}

	ImSequencer::Sequencer(this, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);

	if (selectedEntry != -1)
	{
		ImGui::Text("I am a %d, please edit me", selectedEntry);
	}
}
