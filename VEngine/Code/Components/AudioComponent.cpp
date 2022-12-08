#include "vpch.h"
#include "AudioComponent.h"
#include "Audio/AudioSystem.h"
#include "Audio/AudioChannel.h"

AudioComponent::AudioComponent()
{
    layer = CollisionLayers::None;
}

void AudioComponent::Start()
{
    if (playOnStart && !audioFilename.empty())
    {
        channelID = AudioSystem::PlayAudio(audioFilename, loop);
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

    //@Todo: Ideally you want these as Property Changed events, but it's fine for now.
    channel->SetPitch(pitch);
    channel->SetVolume(volume);
}

void AudioComponent::Create()
{
}

Properties AudioComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "AudioComponent";
    props.Add("Audio Filename", &audioFilename).autoCompletePath = "/Audio/";
    props.Add("Play On Start", &playOnStart);
    props.Add("Volume", &volume);
    props.Add("Pitch", &pitch);
    props.Add("Loop", &loop);
    return props;
}

void AudioComponent::Play()
{
    channelID = AudioSystem::PlayAudio(audioFilename);
}

void AudioComponent::Stop()
{
    auto channel = AudioSystem::GetChannel(channelID);
    channel->sourceVoice->Stop();
}
