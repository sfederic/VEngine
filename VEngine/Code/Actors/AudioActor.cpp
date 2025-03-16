
#include "AudioActor.h"
#include "Components/AudioComponent.h"

AudioActor::AudioActor()
{
	audioComponent = AudioComponent::system.Add("Audio", this);
	SetRootComponent(audioComponent);
}

Properties AudioActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
