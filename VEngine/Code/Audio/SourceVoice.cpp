#include "SourceVoice.h"

bool SourceVoice::CheckNotPlaying()
{
	return (!isPlaying && voice == nullptr);
}

void __stdcall SourceVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
}

void __stdcall SourceVoice::OnVoiceProcessingPassEnd(void)
{
}

void __stdcall SourceVoice::OnStreamEnd(void)
{
}

void __stdcall SourceVoice::OnBufferStart(void* pBufferContext)
{
	isPlaying = true;
}

void __stdcall SourceVoice::OnBufferEnd(void* pBufferContext)
{
	isPlaying = false;

	voice->DestroyVoice();
	voice = nullptr;
}

void __stdcall SourceVoice::OnLoopEnd(void* pBufferContext)
{
}

void __stdcall SourceVoice::OnVoiceError(void* pBufferContext, HRESULT Error)
{
}
