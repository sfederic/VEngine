#pragma once
#include <xaudio2.h>

struct SourceVoice : IXAudio2VoiceCallback
{
	IXAudio2SourceVoice* voice = nullptr;

	bool isPlaying = false;

	bool CheckNotPlaying();

	// Inherited via IXAudio2VoiceCallback
	virtual void __stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired) override;
	virtual void __stdcall OnVoiceProcessingPassEnd(void) override;
	virtual void __stdcall OnStreamEnd(void) override;
	virtual void __stdcall OnBufferStart(void* pBufferContext) override;
	virtual void __stdcall OnBufferEnd(void* pBufferContext) override;
	virtual void __stdcall OnLoopEnd(void* pBufferContext) override;
	virtual void __stdcall OnVoiceError(void* pBufferContext, HRESULT Error) override;
};
