#include "vpch.h"
#include "Sequencer.h"
#include "Editor/ImGui/imgui.h"
#include "Editor/ImGui/misc/cpp/imgui_stdlib.h"
#include "Core/Core.h"
#include "Core/Serialiser.h"
#include "Core/Deserialiser.h"
#include "AudioSequenceEntryData.h"

Sequencer gSequencer;

void Sequencer::Tick()
{
	if (!isRunning)
	{
		return;
	}

	if (currentFrame >= _frameMax)
	{
		isRunning = false;
	}

	if (currentFrame < _frameMax)
	{
		currentFrame++;

		for (auto& item : sequencerItems)
		{
			if (item.mFrameStart <= currentFrame)
			{
				if (!item.mIsActive)
				{
					item.mIsActive = true;
					item.entryData->Activate();
				}
			}

			/*if (item.mFrameEnd >= currentFrame)
			{
				if (item.mIsActive)
				{
					item.mIsActive = false;
					item.entryData->Deactivate();
				}
			}*/
		}
	}
}

void Sequencer::Render()
{
	if (ImGui::Button("Save File"))
	{
		WriteCurrentSequenceFileOut();
	}

	if (ImGui::Button("Open File"))
	{
		ReadInSequencerFile();
	}

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
		auto& entry = sequencerItems.back();
		entry.entryData = new AudioSequenceEntryData();
	}

	if (selectedEntry >= 0 && !sequencerItems.empty())
	{
		const auto& entry = GetSequenceEntry(selectedEntry);

		switch (entry.mType)
		{
		case SequenceEntryTypes::Audio:
			auto entryData = dynamic_cast<AudioSequenceEntryData*>(entry.entryData);
			ImGui::InputText("Audio Filename", &entryData->audioFilename);
			break;
		}
	}

	ImSequencer::Sequencer(this, &currentFrame, &expanded, &selectedEntry, &firstFrame,
		ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL |
		ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
}

void Sequencer::ActivateSequencer()
{
	if (isRunning)
	{
		return;
	}

	isRunning = true;
	currentFrame = _frameMin;
}

void Sequencer::WriteCurrentSequenceFileOut()
{
	Serialiser s("sequencer_test.vseq", OpenMode::Out);

	const size_t numberOfItems = sequencerItems.size();
	s.WriteLine(numberOfItems);

	for (auto& item : sequencerItems)
	{
		auto props = item.entryData->GetProps();
		s.Serialise(props);
		s.WriteLine(L"next");
	}
}

void Sequencer::ReadInSequencerFile()
{
	sequencerItems.clear();

	Deserialiser d("sequencer_test.vseq", OpenMode::In);

	size_t numberOfItems = 0;
	d.ReadLine(numberOfItems);

	for (size_t itemIndex = 0; itemIndex < numberOfItems; itemIndex++)
	{
		//Manually read in the type to create it, as there are no systems for entries.
		std::wstring typeName;
		d.ReadLine(typeName);
		int entryType = -1;
		d.ReadLine(entryType);
		assert(entryType != -1);

		switch (entryType)
		{
		case (int)SequenceEntryTypes::Audio:
			sequencerItems.push_back(SequenceItem(entryType));
			auto& entry = sequencerItems.back();
			entry.entryData = new AudioSequenceEntryData();
			break;
		}

		auto& entry = sequencerItems.back();
		auto entryProps = entry.entryData->GetProps();
		d.Deserialise(entryProps);
	}
}
