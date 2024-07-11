#include "vpch.h"
#include "Sequencer.h"
#include <QFileDialog>
#include "Asset/AssetBaseFolders.h"
#include "Editor/ImGui/imgui.h"
#include "Editor/ImGui/misc/cpp/imgui_stdlib.h"
#include "Core/Core.h"
#include "Core/Serialiser.h"
#include "Core/Deserialiser.h"
#include "AudioSequenceEntryData.h"
#include "CameraSequenceEntryData.h"

Sequencer gSequencer;

void Sequencer::Add(int type)
{
	auto item = SequenceItem(type);

	switch (type)
	{
	case (int)SequenceEntryTypes::Audio:
		item.entryData = std::make_unique<AudioSequenceEntryData>();
		break;

	case (int)SequenceEntryTypes::Camera:
		item.entryData = std::make_unique<CameraSequenceEntryData>();
		break;
	}

	sequencerItems.emplace_back(std::move(item));
}

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
			if (currentFrame >= item.mFrameStart)
			{
				if (!item.mIsActive)
				{
					item.mIsActive = true;
					item.entryData->Activate();
				}
			}

			if (currentFrame >= item.mFrameEnd)
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
	}

	if (selectedEntry >= 0 && !sequencerItems.empty())
	{
		const auto& entry = GetSequenceEntry(selectedEntry);

		switch (entry.mType)
		{
		case SequenceEntryTypes::Audio:
		{
			auto entryData = dynamic_cast<AudioSequenceEntryData*>(entry.entryData.get());
			assert(entryData);
			ImGui::InputText("Audio Filename", &entryData->audioFilename);
			break;
		}
		case SequenceEntryTypes::Camera:
		{
			auto entryData = dynamic_cast<CameraSequenceEntryData*>(entry.entryData.get());
			assert(entryData);
			//Todo: there's some imgui assert error here.
			ImGui::InputText("Camera", &entryData->cameraName);
			break;
		}
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
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::AnyFile);
	const QString filePath = dialog.getSaveFileName(nullptr, "Save sequencer file",
		QString::fromStdString(AssetBaseFolders::sequencerFiles), nullptr, nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (filePath.isEmpty())
	{
		return;
	}

	Serialiser s(filePath.toStdString(), OpenMode::Out);

	const size_t numberOfItems = sequencerItems.size();
	s.WriteLine(numberOfItems);

	for (auto& item : sequencerItems)
	{
		auto props = item.entryData->GetProps();
		s.Serialise(props);
		s.WriteLine(L"next");
	}

	Log("%s sequencer file saved.", filePath.toStdString().c_str());
}

void Sequencer::ReadInSequencerFile()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::AnyFile);
	const QString filePath = dialog.getOpenFileName(nullptr, "Load sequencer file",
		QString::fromStdString(AssetBaseFolders::sequencerFiles), nullptr, nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (filePath.isEmpty())
	{
		return;
	}

	sequencerItems.clear();

	Deserialiser d(filePath.toStdString(), OpenMode::In);

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
			Add((int)SequenceEntryTypes::Audio);
			break;
		}

		auto& entry = sequencerItems.back();
		auto entryProps = entry.entryData->GetProps();
		d.Deserialise(entryProps);
	}

	Log("%s sequencer file loaded.", filePath.toStdString().c_str());
}
