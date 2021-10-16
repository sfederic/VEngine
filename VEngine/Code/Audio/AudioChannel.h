#pragma once
#include <xaudio2.h>

//Rough wrapper around IXAudio2SourceVoice to handle deletion and callbacks easier
struct AudioChannel : IXAudio2VoiceCallback
{
	IXAudio2SourceVoice* voice = nullptr;

	bool isPlaying = false;

	~AudioChannel();
	// Inherited via IXAudio2VoiceCallback
	virtual void __stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired) override;
	virtual void __stdcall OnVoiceProcessingPassEnd(void) override;
	virtual void __stdcall OnStreamEnd(void) override;
	virtual void __stdcall OnBufferStart(void* pBufferContext) override;
	virtual void __stdcall OnBufferEnd(void* pBufferContext) override;
	virtual void __stdcall OnLoopEnd(void* pBufferContext) override;
	virtual void __stdcall OnVoiceError(void* pBufferContext, HRESULT Error) override;
};
