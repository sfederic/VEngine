#include "vpch.h"
#include "AudioComponent.h"
#include "Audio/AudioSystem.h"
#include "Audio/AudioChannel.h"
#include "Actors/Game/Player.h"

AudioComponent::AudioComponent()
{
	layer = CollisionLayers::None;
}

void AudioComponent::Start()
{
	channelID = AudioSystem::LoadAudio(audioFilename, loop);

	if (playOnStart)
	{
		AudioSystem::PlayAudio(channelID);
	}
}

void AudioComponent::Tick(float deltaTime)
{
	auto channel = AudioSystem::GetChannel(channelID);

	const float fadeSpeed = 0.4f;

	switch (fade)
	{
	case FadeValue::Out:
		if (volume > 0.f) volume -= fadeSpeed * deltaTime;
		else fade = FadeValue::None;
		break;
	case FadeValue::In:
		if (volume < 1.f) volume += fadeSpeed * deltaTime;
		else fade = FadeValue::None;
		break;
	}

	SetVolumeToPlayerPositionAgainstRadius();

	//@Todo: Ideally you want these as Property Changed events, but it's fine for now.
	channel->SetPitch(pitch);
	channel->SetVolume(volume);
}

Properties AudioComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Audio Filename", &audioFilename).autoCompletePath = "/Audio/";
	props.Add("Play On Start", &playOnStart);
	props.Add("Volume", &volume);
	props.Add("Volume Attenuation", &volumeAttenuation);
	props.Add("Pitch", &pitch);
	props.Add("Loop", &loop);
	return props;
}

void AudioComponent::Play()
{
	auto channel = AudioSystem::GetChannel(channelID);
	//@Todo: this is still not right. The audio channel is only paused, not restarted.
	channel->sourceVoice->Start();
}

void AudioComponent::Stop()
{
	auto channel = AudioSystem::GetChannel(channelID);
	channel->sourceVoice->Stop();
}

void AudioComponent::SetVolumeToPlayerPositionAgainstRadius()
{
	const auto playerPos = Player::system.GetOnlyActor()->GetPositionV();
	const float length = XMVector3Length(playerPos - GetWorldPositionV()).m128_f32[0];
	volume = (1.0f / length) * volumeAttenuation;
}
