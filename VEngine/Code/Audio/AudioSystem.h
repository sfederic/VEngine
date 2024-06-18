#pragma once

#include <string>
#include "Core/UID.h"

struct AudioChannel;

//@Todo: Ogg Vorbis loader if file size becomes an issue https://www.gamedev.net/forums/topic/496350-xaudio2-and-ogg/
//Lot of this was inspired by https://gdcvault.com/play/1022061/How-to-Write-an-Audio
namespace AudioSystem
{
	void Init();
	void Tick();
	void Cleanup();
	void DeleteLoadedAudioAndChannels();
	void StopPersistentTracks();
	AudioChannel* GetChannel(UID channelID);
	void MuteAllAudio();
	void StopAllAudio();
	void StartAllAudio();
	void UnmuteAllAudio();
	void FadeOutAllAudio();
	void FadeInAllAudio();
	//Returns channel ID that audio is playing on so that audio components can work with that data.
	UID LoadAudio(const std::string filename, bool loopAudio = false);
	void PlayAudio(UID channelID);
	void InnerLoadAudio(const std::string filename);
	void UnloadAudio(const std::string filename);
	void PlayPersistentAudio(std::string filename);
};
