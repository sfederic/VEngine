#include "vpch.h"
#include "AudioActor.h"
#include "Components/AudioComponent.h"

AudioActor::AudioActor()
{
    audioComponent = AudioComponent::system.Add("Audio", this);
    rootComponent = audioComponent;
}

Properties AudioActor::GetProps()
{
    auto props = Actor::GetProps();
    props.title = "AudioActor";
    return props;
}
