#pragma once
#include <xaudio2.h>
#include <string>
#include <map>

//TODO: wav files might be a bit too big with git, look into .ogg 
//https://github.com/nothings/stb/blob/master/stb_vorbis.c

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

//Base class for audio data
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

	bool isPlaying;
};

struct AudioSystem
{
	IXAudio2* audioEngine; //Main XAudio2 sound engine
	IXAudio2MasteringVoice* masteringVoice; //Main track	

	//Maps audio filename to AudioChunk
	std::map<std::string, AudioBase*> loadedAudioFilesMap;

	void Init();
	void PlayAudio(AudioBase* audio);

	//Finds an audio file by filename and creates the audio if the map doesn't contain the filename.
	AudioBase* FindAudio(const std::string filename);

private:
	HRESULT LoadWAV(const std::string filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer);
	HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);
};

extern AudioSystem audioSystem;
