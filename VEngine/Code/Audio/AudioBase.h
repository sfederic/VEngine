#pragma once
#include <xaudio2.h>
#include <string>

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

	AudioBase(std::string filename)
	{
		audioFilename = filename;
	}

	~AudioBase()
	{
		sourceVoice->DestroyVoice();
	}

	void SetPitch(float ratio)
	{
		if (ratio <= 0.f) { ratio = 1.f; }
		sourceVoice->SetFrequencyRatio(ratio);
	}

	WAVEFORMATEXTENSIBLE waveFormat;
	XAUDIO2_BUFFER buffer;
	std::string audioFilename;
	IXAudio2SourceVoice* sourceVoice;
	bool isPlaying;
};
