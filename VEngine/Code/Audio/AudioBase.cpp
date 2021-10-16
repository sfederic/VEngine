#include "AudioBase.h"

AudioBase::AudioBase(std::string filename)
{
	audioFilename = filename;
}

AudioBase::~AudioBase()
{
	for (auto voice : sourceVoicesPlaying)
	{
		voice->DestroyVoice();
	}
}

void AudioBase::OnBufferEnd(void* pBufferContext)
{
	auto voice = (IXAudio2SourceVoice*)pBufferContext;
	for (int i = 0; i < sourceVoicesPlaying.size(); i++)
	{
		if (sourceVoicesPlaying[i] == voice)
		{
			voice->DestroyVoice();
			sourceVoicesPlaying.erase(sourceVoicesPlaying.begin() + i);
			break;
		}
	}
}

void __stdcall AudioBase::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
}

void __stdcall AudioBase::OnVoiceProcessingPassEnd(void)
{
}

void __stdcall AudioBase::OnStreamEnd(void)
{
}

void __stdcall AudioBase::OnBufferStart(void* pBufferContext)
{
}

void __stdcall AudioBase::OnLoopEnd(void* pBufferContext)
{
}

void __stdcall AudioBase::OnVoiceError(void* pBufferContext, HRESULT Error)
{
}
