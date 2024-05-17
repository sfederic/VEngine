#include "vpch.h"
#include "AudioChannel.h"
#include "Core/Debug.h"

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

void __stdcall AudioChannel::OnBufferStart(void*)
{
	isPlaying = true;
}

void __stdcall AudioChannel::OnBufferEnd(void*)
{
	isPlaying = false;
}
