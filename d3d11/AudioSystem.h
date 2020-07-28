#pragma once

#include <xaudio2.h>
#include "Audio.h"
#include <vector>

class AudioSystem
{
public:

	void Init();
	void PlayAudio(AudioChunk* chunk);
	bool CreateAudio(const char* filename, AudioChunk* chunk);
	HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	HRESULT LoadWAV(const char* filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer);

	IXAudio2* audioEngine; //Main XAudio2 sound engine
	IXAudio2MasteringVoice* masteringVoice; //Main track	

	std::vector<AudioChunk*> audioChunks;
};

extern AudioSystem gAudioSystem;