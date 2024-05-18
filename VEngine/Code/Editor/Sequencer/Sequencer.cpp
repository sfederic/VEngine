#include "vpch.h"
#include "Sequencer.h"
#include "Editor/ImGui/imgui.h"

void Sequencer::Render()
{
	ImGui::PushItemWidth(130);
	ImGui::InputInt("Frame ", &currentFrame);
	ImGui::SameLine();
	ImGui::InputInt("Frame Min", &_frameMin);
	ImGui::SameLine();
	ImGui::InputInt("Frame Max", &_frameMax);
	ImGui::PopItemWidth();

	if (ImGui::Button("Camera"))
	{
		Add((int)SequenceEntryTypes::Camera);
	}
	if (ImGui::Button("Audio"))
	{
		Add((int)SequenceEntryTypes::Audio);
	}

	ImSequencer::Sequencer(this, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);

	if (selectedEntry >= 0)
	{
		switch (selectedEntry)
		{
		case (int)SequenceEntryTypes::Audio:
			auto& entry = GetSequenceEntry(selectedEntry);

			break;
		}
	}
}
