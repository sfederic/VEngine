#pragma once

#include <xaudio2.h>

//Rough wrapper around IXAudio2SourceVoice to handle deletion and callbacks easier
struct AudioChannel : IXAudio2VoiceCallback
{
	IXAudio2SourceVoice* sourceVoice = nullptr;

	bool isPlaying = false;
	bool isLooping = false;

	AudioChannel() {}
	~AudioChannel();
	void SetVolume(float volume);
	float GetVolume();
	void SetPitch(float pitch);
	float GetPitch();

	// Inherited via IXAudio2VoiceCallback
	virtual void __stdcall OnVoiceProcessingPassStart(UINT32) override {}
	virtual void __stdcall OnVoiceProcessingPassEnd(void) override {}
	virtual void __stdcall OnStreamEnd(void) override {}
	virtual void __stdcall OnBufferStart(void*) override;
	virtual void __stdcall OnBufferEnd(void*) override;
	virtual void __stdcall OnLoopEnd(void*) override {}
	virtual void __stdcall OnVoiceError(void*, HRESULT) override {}
};
