#pragma once
#include <vector>
#include <xaudio2.h>

struct VoiceCallback : public IXAudio2VoiceCallback
{
	HANDLE hBufferEndEvent;
	VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~VoiceCallback() { CloseHandle(hBufferEndEvent); }

	//Called when the voice has just finished playing a contiguous audio stream.
	void OnStreamEnd() { SetEvent(hBufferEndEvent); }

	void OnVoiceProcessingPassEnd() {}
	void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	void OnBufferEnd(void* pBufferContext) {}
	void OnBufferStart(void* pBufferContext) {}
	void OnLoopEnd(void* pBufferContext) {}
	void OnVoiceError(void* pBufferContext, HRESULT Error) {}
};

struct AudioBase
{
	VoiceCallback callback;

	void SetPitch(float ratio)
	{
		if (ratio <= 0.f) { ratio = 1.f; }
		sourceVoice->SetFrequencyRatio(ratio);
	}

	WAVEFORMATEXTENSIBLE waveFormat;
	XAUDIO2_BUFFER buffer;

	IXAudio2SourceVoice* sourceVoice;

	bool bIsPlaying;
};

struct AudioChunk : AudioBase
{
	//bool CreateAudio(const char* filename);
};

struct AudioStream : AudioBase
{

};

struct AudioSystem
{
	IXAudio2* audioEngine; //Main XAudio2 sound engine
	IXAudio2MasteringVoice* masteringVoice; //Main track	

	std::vector<AudioChunk*> audioChunks;

	void Init();
	void PlayAudio(AudioChunk* chunk);
	HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	HRESULT LoadWAV(const char* filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer);
	bool CreateAudio(const char* filename, AudioChunk* chunk);
};
