#include "vpch.h"
#include "Sequencer.h"
#include <QFileDialog>
#include "Asset/AssetBaseFolders.h"
#include "Editor/ImGui/imgui.h"
#include "Editor/ImGui/misc/cpp/imgui_stdlib.h"
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Serialiser.h"
#include "Core/Deserialiser.h"
#include "AudioSequenceEntryData.h"
#include "CameraSequenceEntryData.h"
#include "ActiveCameraLerpToOffsetPosition.h"

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

	case (int)SequenceEntryTypes::ActiveCameraLerpToOffsetPosition:
		item.entryData = std::make_unique<ActiveCameraLerpToOffsetPosition>();
		break;
	}

	_sequencerItems.emplace_back(std::move(item));
}

void Sequencer::Tick()
{
	if (!_isRunning)
	{
		return;
	}

	if (_currentFrame >= _frameMax)
	{
		Reset();
	}

	if (_currentFrame < _frameMax)
	{
		_currentFrame++;

		for (int sequenceItemIndex = 0; sequenceItemIndex < _sequencerItems.size(); sequenceItemIndex++)
		{
			auto& item = _sequencerItems[sequenceItemIndex];

			//Activate the entry item
			if (_currentFrame >= item.mFrameStart)
			{
				if (!item.mIsActive)
				{
					item.mIsActive = true;
					item.entryData->Activate();
				}
			}

			//Entry item tick
			if (item.mIsActive)
			{
				item.entryData->Tick(Core::GetDeltaTime());
			}

			//Deactivate entry item
			if (_currentFrame >= item.mFrameEnd)
			{
				if (item.mIsActive)
				{
					item.mIsActive = false;
					item.entryData->Deactivate();
					_sequencerItems.erase(_sequencerItems.begin() + sequenceItemIndex);
				}
			}
		}
	}
}

void Sequencer::Render()
{
	if (ImGui::Button("Save File"))
	{
		WriteCurrentSequenceFileOutFromDialog();
	}

	if (ImGui::Button("Open File"))
	{
		ReadInSequencerFileFromDialog();
	}

	ImGui::PushItemWidth(130);
	ImGui::InputInt("Frame ", &_currentFrame);
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
	if (ImGui::Button("Camera Lerp"))
	{
		Add((int)SequenceEntryTypes::ActiveCameraLerpToOffsetPosition);
	}

	if (_selectedEntry >= 0 && !_sequencerItems.empty())
	{
		const auto& entry = GetSequenceEntry(_selectedEntry);

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
		case SequenceEntryTypes::ActiveCameraLerpToOffsetPosition:
		{
			auto entryData = dynamic_cast<ActiveCameraLerpToOffsetPosition*>(entry.entryData.get());
			assert(entryData);
			if (ImGui::Button("Camera Lerp To Position Snapshot"))
			{
				entryData->SetPositionToLerpTo();
			}
			break;
		}
		}
	}

	ImSequencer::Sequencer(this, &_currentFrame, &_expanded, &_selectedEntry, &_firstFrame,
		ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL |
		ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
}

void Sequencer::ActivateSequencer(const std::string sequenceFileName)
{
	if (_isRunning)
	{
		return;
	}

	ReadInSequencerFile(sequenceFileName);

	_isRunning = true;
	_currentFrame = _frameMin;
}

void Sequencer::WriteCurrentSequenceFileOutFromDialog()
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

	const size_t numberOfItems = _sequencerItems.size();
	s.WriteLine(numberOfItems);

	for (auto& item : _sequencerItems)
	{
		auto props = item.entryData->GetProps();
		s.Serialise(props);
		s.WriteLine(L"next");
	}

	Log("%s sequencer file saved.", filePath.toStdString().c_str());
}

void Sequencer::ReadInSequencerFileFromDialog()
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

	_sequencerItems.clear();

	ReadInSequencerFile(filePath.toStdString());
}

void Sequencer::ReadInSequencerFile(const std::string sequenceFileName)
{
	Deserialiser d(sequenceFileName, OpenMode::In);

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

		auto& entry = _sequencerItems.back();
		auto entryProps = entry.entryData->GetProps();
		d.Deserialise(entryProps);
	}

	Log("%s sequencer file loaded.", sequenceFileName.c_str());
}

void Sequencer::Reset()
{
	_sequencerItems.clear();
	_isRunning = false;
}
