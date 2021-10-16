#pragma once
#include <xaudio2.h>
#include <string>

//Base class for audio data
struct AudioBase : IXAudio2VoiceCallback
{
	WAVEFORMATEXTENSIBLE waveFormat = {};
	XAUDIO2_BUFFER buffer = {};
	std::string audioFilename;
	IXAudio2SourceVoice* sourceVoice = nullptr;
	bool isPlaying = false;

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

	//Called when the voice has just finished playing a contiguous audio stream.
	void OnStreamEnd() 
	{ 
	}

	void OnVoiceProcessingPassEnd() {}
	void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	void OnBufferEnd(void* pBufferContext) 
	{
		isPlaying = false;
	}
	void OnBufferStart(void* pBufferContext) {}
	void OnLoopEnd(void* pBufferContext) {}
	void OnVoiceError(void* pBufferContext, HRESULT Error) {}
};
