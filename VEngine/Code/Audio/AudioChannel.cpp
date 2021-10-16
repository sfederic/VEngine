#include "AudioChannel.h"

AudioChannel::~AudioChannel()
{
	voice->DestroyVoice();
}

void __stdcall AudioChannel::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
}

void __stdcall AudioChannel::OnVoiceProcessingPassEnd(void)
{
}

void __stdcall AudioChannel::OnStreamEnd(void)
{
}

void __stdcall AudioChannel::OnBufferStart(void* pBufferContext)
{
	isPlaying = true;
}

void __stdcall AudioChannel::OnBufferEnd(void* pBufferContext)
{
	isPlaying = false;
}

void __stdcall AudioChannel::OnLoopEnd(void* pBufferContext)
{
}

void __stdcall AudioChannel::OnVoiceError(void* pBufferContext, HRESULT Error)
{
}
