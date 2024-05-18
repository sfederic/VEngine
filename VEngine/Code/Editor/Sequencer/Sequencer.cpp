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
		auto& entry = GetSequenceEntry((int)SequenceEntryTypes::Audio);
		entry.entryData = new AudioSequenceEntryData();
	}

	if (selectedEntry >= 0 && !sequencerItems.empty())
	{
		switch (selectedEntry)
		{
		case (int)SequenceEntryTypes::Audio:
			const auto& entry = GetSequenceEntry(selectedEntry);
			auto entryData = dynamic_cast<AudioSequenceEntryData*>(entry.entryData);
			static char audioFilenameInput[512];
			ImGui::InputText("Audio Filename", audioFilenameInput, sizeof(audioFilenameInput));
			entryData->audioFilename = audioFilenameInput;
			break;
		}
	}

	ImSequencer::Sequencer(this, &currentFrame, &expanded, &selectedEntry, &firstFrame,
		ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL |
		ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
}
