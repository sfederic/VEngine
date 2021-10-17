#include "AudioComponent.h"
#include "Audio/AudioBase.h"
#include "Audio/AudioSystem.h"
#include "Audio/AudioChannel.h"

AudioComponent::AudioComponent()
{
}

void AudioComponent::Tick(double deltaTime)
{
    auto channel = audioSystem.GetChannel(channelID);

    //Ideally you want these as Property Changed events, but it's fine for now.
    channel->SetPitch(pitch);
    channel->SetVolume(volume);
}

void AudioComponent::Start()
{
    if (playOnStart)
    {
        channelID = audioSystem.PlayAudio(audioFilename, loop);
    }
}

void AudioComponent::Create()
{
}

Properties AudioComponent::GetProps()
{
    Properties props("AudioComponent");
    props.Add("Audio Filename", &audioFilename);
    props.Add("Play On Start", &playOnStart);
    props.Add("Volume", &volume);
    props.Add("Pitch", &pitch);
    props.Add("Loop", &loop);
    return props;
}

void AudioComponent::Play()
{
    channelID = audioSystem.PlayAudio(audioFilename);
}

void AudioComponent::Stop()
{
    auto channel = audioSystem.GetChannel(channelID);
    channel->sourceVoice->Stop();
}
