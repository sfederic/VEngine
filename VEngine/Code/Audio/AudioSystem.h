#pragma once
#include <xaudio2.h>
#include <string>
#include <map>
#include <vector>

struct AudioChannel;
struct AudioBase;

//Lot of this was inspired by https://gdcvault.com/play/1022061/How-to-Write-an-Audio after 
//fucking around for hours with XAudio2.
struct AudioSystem
{
private:
	uint64_t nextChannelID = 0;

	IXAudio2* audioEngine = nullptr; //Main XAudio2 sound engine
	IXAudio2MasteringVoice* masteringVoice = nullptr; //Main track	

	typedef std::map<std::string, AudioBase*> AudioMap;
	AudioMap loadedAudioMap;

	typedef std::map<uint64_t, AudioChannel*> ChannelMap;
	ChannelMap channelMap;

public:
	void Init();
	void Tick();
	void Cleanup();
	void DeleteLoadedAudioAndChannels();
	AudioChannel* GetChannel(uint64_t channelID);

	//Returns channel ID that audio is playing on so that audio components can work with that data.
	uint64_t PlayAudio(const std::string filename);

	void LoadAudio(const std::string filename);
	void UnloadAudio(const std::string filename);

private:
	HRESULT LoadWAV(const std::string filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer);
	HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);
};

extern AudioSystem audioSystem;
