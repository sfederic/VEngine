#include "AudioChannel.h"
#include "Debug.h"

AudioChannel::AudioChannel()
{
}

AudioChannel::~AudioChannel()
{
	HR(sourceVoice->Stop());
	sourceVoice->DestroyVoice();
	sourceVoice = nullptr;
}

void AudioChannel::SetVolume(float volume)
{
	sourceVoice->SetVolume(volume);
}

float AudioChannel::GetVolume()
{
	float volume = 0.f;
	sourceVoice->GetVolume(&volume);
	return volume;
}

void AudioChannel::SetPitch(float pitch)
{
	sourceVoice->SetFrequencyRatio(pitch);
}

float AudioChannel::GetPitch()
{
	float pitch = 0.f;
	sourceVoice->GetFrequencyRatio(&pitch);
	return pitch;
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
