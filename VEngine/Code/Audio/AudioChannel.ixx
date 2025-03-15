export module Audio.AudioChannel;

import Core.Debug;

#include <xaudio2.h>

//Rough wrapper around IXAudio2SourceVoice to handle deletion and callbacks easier
export struct AudioChannel : IXAudio2VoiceCallback
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
