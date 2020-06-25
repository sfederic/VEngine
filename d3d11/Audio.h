#pragma once

#include <xaudio2.h>
#include <Windows.h>

class VoiceCallback : public IXAudio2VoiceCallback
{
public: 

    HANDLE hBufferEndEvent;
    VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
    ~VoiceCallback() { CloseHandle(hBufferEndEvent); }

    //Called when the voice has just finished playing a contiguous audio stream.
    void OnStreamEnd() { SetEvent(hBufferEndEvent); }

    //Unused methods are stubs
    void OnVoiceProcessingPassEnd() { }
    void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
    void OnBufferEnd(void* pBufferContext) { }
    void OnBufferStart(void* pBufferContext) {    }
    void OnLoopEnd(void* pBufferContext) {    }
    void OnVoiceError(void* pBufferContext, HRESULT Error) { }
};

class AudioBase
{
public:

	WAVEFORMATEXTENSIBLE waveFormat;
	XAUDIO2_BUFFER buffer;

	IXAudio2SourceVoice* sourceVoice;

	bool bIsPlaying;
};

class AudioChunk : public AudioBase
{
public: 

	//bool CreateAudio(const char* filename);
};

class AudioStream : public AudioBase
{
public:

};
