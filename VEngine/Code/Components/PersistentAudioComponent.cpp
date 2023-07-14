#include "vpch.h"
#include "PersistentAudioComponent.h"
#include "Audio/AudioSystem.h"

void PersistentAudioComponent::Start()
{
	AudioSystem::PlayPersistentAudio(audioFilename);
}

Properties PersistentAudioComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
