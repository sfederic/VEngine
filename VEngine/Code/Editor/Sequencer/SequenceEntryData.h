#pragma once

#include <string>
#include "Gameplay/GameUtils.h"
#include "Audio/AudioSystem.h"

struct SequenceEntryData
{
	virtual void Activate() {}
	virtual void Deactivate() {}
};

struct AudioSequenceEntryData : SequenceEntryData
{
	std::string audioFilename;

	void Activate() override
	{
		GameUtils::PlayAudioOneShot(audioFilename);
	}

	void Deactivate() override
	{
		//@Todo: Just for testing. Remove.
		AudioSystem::StopAllAudio();
	}
};
