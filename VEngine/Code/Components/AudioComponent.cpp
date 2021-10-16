#include "AudioComponent.h"
#include "Audio/AudioBase.h"
#include "Audio/AudioSystem.h"

AudioComponent::AudioComponent()
{
}

void AudioComponent::Tick(double deltaTime)
{
}

void AudioComponent::Start()
{
    if (playOnStart)
    {
        channelID = audioSystem.PlayAudio(audioFilename);
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
    return props;
}
