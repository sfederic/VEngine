
#include "AudioSequenceEntryData.h"
#include "Gameplay/GameUtils.h"

void AudioSequenceEntryData::Activate()
{
	GameUtils::PlayAudioOneShot(audioFilename);
}
